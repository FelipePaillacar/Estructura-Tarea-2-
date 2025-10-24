#include "funciones.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Variable global del cache
static Cache *cache = NULL;

/* ----------------------------------------------------
   Creacion de nodo (valida y normaliza el caracter)
---------------------------------------------------- */
static Node *node_create(char v) {
    unsigned char uc = (unsigned char) v;
    char c = (char) toupper(uc);

    // Validar rango A-Z
    if (c < 'A' || c > 'Z') {
        return NULL;
    }

    Node *n = (Node *)malloc(sizeof(Node));
    if (!n) return NULL;

    n->value = c;
    n->prev = n->next = NULL;
    return n;
}

/* ----------------------------------------------------
   Liberacion interna del cache
---------------------------------------------------- */
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

/* ----------------------------------------------------
   Mover nodo al frente (MRU)
---------------------------------------------------- */
static void move_to_front(Node *node) {
    if (!cache || cache->head == node) return;

    // Desenlazar de su posicion actual
    if (node->prev)
        node->prev->next = node->next;
    else
        cache->head = node->next;

    if (node->next)
        node->next->prev = node->prev;
    else
        cache->tail = node->prev;

    // Insertar al frente
    node->next = cache->head;
    node->prev = NULL;

    if (cache->head)
        cache->head->prev = node;

    cache->head = node;

    if (!cache->tail)
        cache->tail = node;
}

/* ----------------------------------------------------
   Mostrar ayuda
---------------------------------------------------- */
void show_help(void) {
    printf(ANSI_CYAN "\n===== MENU DE COMANDOS LRU CACHE =====\n\n" ANSI_RESET);

    printf(ANSI_YELLOW "Comando" ANSI_RESET "              " ANSI_YELLOW "Descripcion\n" ANSI_RESET);
    printf("-----------------------------------------------\n");

    printf(ANSI_GREEN "lru help" ANSI_RESET "            -> Muestra esta ayuda\n");
    printf(ANSI_GREEN "lru create <size>" ANSI_RESET "   -> Crea el cache LRU con tamano (minimo 5)\n");
    printf(ANSI_GREEN "lru add <char>" ANSI_RESET "      -> Agrega un elemento (caracter A-Z) al cache\n");
    printf(ANSI_GREEN "lru search <char>" ANSI_RESET "   -> Busca un elemento y devuelve su posicion\n");
    printf(ANSI_GREEN "lru get <char>" ANSI_RESET "      -> Usa un elemento y lo mueve al frente (MRU)\n");
    printf(ANSI_GREEN "lru print" ANSI_RESET "           -> Muestra el estado actual del cache\n");
    printf(ANSI_GREEN "lru exit" ANSI_RESET "            -> Libera el cache y termina el programa\n");

}

/* ----------------------------------------------------
   Crear cache
---------------------------------------------------- */
int create_cache(int size) {
    if (size < 5) {
        fprintf(stderr, ANSI_RED "Error: el tamano del cache debe ser al menos 5.\n" ANSI_RESET);
        return -1;
    }

    if (cache != NULL) {
        fprintf(stderr, ANSI_RED "Error: ya existe un cache inicializado.\n" ANSI_RESET);
        return -1;
    }

    Cache *c = (Cache *)malloc(sizeof(Cache));
    if (!c) {
        fprintf(stderr, ANSI_RED "Error: fallo malloc al crear el cache.\n" ANSI_RESET);
        return -1;
    }

    c->capacity = size;
    c->count = 0;
    c->head = c->tail = NULL;
    cache = c;

    printf(ANSI_GREEN "Cache creado con capacidad %d\n" ANSI_RESET, size);
    return 0;
}

/* ----------------------------------------------------
   Liberar cache
---------------------------------------------------- */
int free_cache(void) {
    if (!cache) {
        printf(ANSI_YELLOW "[Aviso] No habia cache para liberar.\n" ANSI_RESET);
        return 1;
    }

    cache_free_internal(cache);
    cache = NULL;

    printf(ANSI_GREEN "Cache liberado correctamente.\n" ANSI_RESET);
    return 0;
}

/* ----------------------------------------------------
   Verificar existencia del cache
---------------------------------------------------- */
int cache_exists(void) {
    return (cache != NULL);
}

/* ----------------------------------------------------
   Agregar elemento (LRU add)
---------------------------------------------------- */
void lru_add(char value) {
    if (!cache) {
        fprintf(stderr, ANSI_RED "Error: crea primero un cache con 'lru create <size>'.\n" ANSI_RESET);
        return;
    }

    unsigned char uc = (unsigned char) value;
    char ch = (char) toupper(uc);
    if (ch < 'A' || ch > 'Z') {
        fprintf(stderr, ANSI_RED "Error: el elemento debe ser una letra A-Z.\n" ANSI_RESET);
        return;
    }

    // Buscar si ya existe
    Node *cur = cache->head;
    while (cur) {
        if (cur->value == ch) {
            move_to_front(cur);
            printf(ANSI_GREEN "Elemento '%c' ya existia; movido al frente.\n" ANSI_RESET, ch);
            lru_print();
            return;
        }
        cur = cur->next;
    }

    // Si esta lleno, eliminar el LRU
    if (cache->count >= cache->capacity) {
        Node *old = cache->tail;
        if (old->prev) {
            cache->tail = old->prev;
            cache->tail->next = NULL;
        } else {
            cache->head = cache->tail = NULL;
        }
        printf(ANSI_YELLOW "Cache lleno: se eliminara el LRU (elemento '%c').\n" ANSI_RESET, old->value);
        free(old);
        cache->count--;
    }

    Node *new_node = node_create(ch);
    if (!new_node) {
        fprintf(stderr, ANSI_RED "Error: no se pudo crear nodo.\n" ANSI_RESET);
        return;
    }

    new_node->next = cache->head;
    if (cache->head)
        cache->head->prev = new_node;
    cache->head = new_node;
    if (!cache->tail)
        cache->tail = new_node;

    cache->count++;
    printf(ANSI_GREEN "Elemento '%c' agregado al cache. (%d/%d)\n" ANSI_RESET, ch, cache->count, cache->capacity);
    lru_print();
}

/* ----------------------------------------------------
   Buscar elemento (LRU search)
---------------------------------------------------- */
int lru_search(char value) {
    if (!cache) {
        fprintf(stderr, ANSI_RED "Error: no hay cache inicializado.\n" ANSI_RESET);
        return -1;
    }

    unsigned char uc = (unsigned char) value;
    char ch = (char) toupper(uc);
    if (ch < 'A' || ch > 'Z') {
        fprintf(stderr, ANSI_RED "Error: el elemento debe ser una letra A-Z.\n" ANSI_RESET);
        return -1;
    }

    Node *cur = cache->head;
    int pos = 1;
    while (cur) {
        if (cur->value == ch) {
            printf(ANSI_GREEN "Elemento '%c' encontrado en posicion %d\n" ANSI_RESET, ch, pos);
            return pos;
        }
        cur = cur->next;
        pos++;
    }

    printf(ANSI_YELLOW "Elemento '%c' no encontrado.\n" ANSI_RESET, ch);
    return -1;
}

/* ----------------------------------------------------
   Obtener elemento (LRU get)
---------------------------------------------------- */
void lru_get(char value) {
    if (!cache) {
        fprintf(stderr, ANSI_RED "Error: no hay cache inicializado.\n" ANSI_RESET);
        return;
    }

    unsigned char uc = (unsigned char) value;
    char ch = (char) toupper(uc);
    if (ch < 'A' || ch > 'Z') {
        fprintf(stderr, ANSI_RED "Error: el elemento debe ser una letra A-Z.\n" ANSI_RESET);
        return;
    }

    Node *cur = cache->head;
    while (cur) {
        if (cur->value == ch) {
            move_to_front(cur);
            printf(ANSI_GREEN "Elemento '%c' usado; movido al frente (MRU).\n" ANSI_RESET, ch);
            lru_print();
            return;
        }
        cur = cur->next;
    }

    printf(ANSI_YELLOW "Elemento '%c' no encontrado.\n" ANSI_RESET, ch);
}

/* ----------------------------------------------------
   Imprimir estado del cache
---------------------------------------------------- */
void lru_print(void) {
    if (!cache) {
        printf(ANSI_YELLOW "[No hay cache creado]\n" ANSI_RESET);
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
