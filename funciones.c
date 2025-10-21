#include "funciones.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

static Cache *cache = NULL;

static Node *node_create(char v) {
    Node *n = (Node *)malloc(sizeof(Node));
    if (!n) return NULL;
    n->value = v;
    n->prev = n->next = NULL;
    return n;
}

static void cache_free_internal(Cache *c) {
    if (!c) return;
    Node *cur = c->head;
    while (cur) {
        Node *next = cur->next;
        free(cur);
        cur = next;
    }
    free(c);
}

static void move_to_front(Node *node) {
    if (!cache || cache->head == node) return;

    // Desenlazar
    if (node->prev) node->prev->next = node->next;
    if (node->next) node->next->prev = node->prev;
    if (cache->tail == node) cache->tail = node->prev;

    // Insertar al frente
    node->next = cache->head;
    node->prev = NULL;
    if (cache->head) cache->head->prev = node;
    cache->head = node;

    if (!cache->tail) cache->tail = node;
}

void show_help(void) {
    puts("Comandos disponibles:");
    puts("  lru help            -> Muestra esta ayuda");
    puts("  lru create <size>   -> Crea el cache LRU con tamaño (mínimo 5)");
    puts("  lru add <char>      -> Agrega un elemento (carácter) al cache");
    puts("  lru search <char>   -> Busca un elemento y devuelve su posición");
    puts("  lru get <char>      -> Usa un elemento y lo mueve al frente (MRU)");
    puts("  lru exit            -> Libera el cache y termina el programa");
}

int create_cache(int size) {
    if (size < 5) {
        fprintf(stderr, "Error: el tamaño del cache debe ser al menos 5.\n");
        return -1;
    }
    if (cache != NULL) {
        fprintf(stderr, "Error: ya existe un cache inicializado.\n");
        return -1;
    }
    Cache *c = (Cache *)malloc(sizeof(Cache));
    if (!c) {
        fprintf(stderr, "Error: falló malloc al crear el cache.\n");
        return -1;
    }
    c->capacity = size;
    c->count = 0;
    c->head = c->tail = NULL;
    cache = c;
    printf("Cache creado con capacidad %d\n", size);
    return 0;
}

int free_cache(void) {
    if (!cache) return 1; 
    cache_free_internal(cache);
    cache = NULL;
    return 0;
}

int cache_exists(void) {
    return (cache != NULL) ? 1 : 0;
}

void lru_add(char value) {
    if (!cache) {
        fprintf(stderr, "Error: crea primero un cache con 'lru create <size>'.\n");
        return;
    }

    // Buscar si ya existe
    Node *cur = cache->head;
    while (cur) {
        if (cur->value == value) {
            move_to_front(cur);
            printf("Elemento '%c' ya existía; movido al frente.\n", value);
            lru_print();
            return;
        }
        cur = cur->next;
    }

    // Si está lleno, eliminar el menos usado (cola)
    if (cache->count >= cache->capacity) {
        Node *old = cache->tail;
        cache->tail = old->prev;
        if (cache->tail) cache->tail->next = NULL;
        free(old);
        cache->count--;
        printf("Cache lleno: se eliminó el LRU (último elemento).\n");
    }

    // Crear y añadir al frente
    Node *new_node = node_create(value);
    new_node->next = cache->head;
    if (cache->head) cache->head->prev = new_node;
    cache->head = new_node;
    if (!cache->tail) cache->tail = new_node;
    cache->count++;

    printf("Elemento '%c' agregado al cache.\n", value);
    lru_print();
}

int lru_search(char value) {
    if (!cache) {
        fprintf(stderr, "Error: no hay cache inicializado.\n");
        return -1;
    }
    Node *cur = cache->head;
    int pos = 0;
    while (cur) {
        if (cur->value == value) {
            printf("Elemento '%c' encontrado en posición %d\n", value, pos);
            return pos;
        }
        cur = cur->next;
        pos++;
    }
    printf("Elemento '%c' no encontrado.\n", value);
    return -1;
}

void lru_get(char value) {
    if (!cache) {
        fprintf(stderr, "Error: no hay cache inicializado.\n");
        return;
    }
    Node *cur = cache->head;
    while (cur) {
        if (cur->value == value) {
            move_to_front(cur);
            printf("Elemento '%c' usado; movido al frente (MRU).\n", value);
            lru_print();
            return;
        }
        cur = cur->next;
    }
    printf("Elemento '%c' no encontrado.\n", value);
}

void lru_print(void) {
    if (!cache) {
        printf("[No hay cache creado]\n");
        return;
    }
    Node *cur = cache->head;
    printf("Cache (MRU → LRU): ");
    while (cur) {
        printf("[%c] ", cur->value);
        cur = cur->next;
    }
    printf("\n");
}