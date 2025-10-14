#include "funciones.h"
#include <stdio.h>
#include <stdlib.h>

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
void show_help(void) {
    puts("Comandos disponibles:");
    puts("  lru help            -> Muestra esta ayuda");
    puts("  lru create <size>   -> Crea el cache LRU con tamaño (mínimo 5)");
    puts("  lru exit            -> Salir (libera el cache si existe)");
    puts("");
    puts("Nota: en esta fase solo está implementado 'lru create'.");
}

int create_cache(int size) {
    if (size < 5) {
        fprintf(stderr, "Error: el tamaño del cache debe ser al menos 5 (tamaño solicitado: %d).\n", size);
        return -1;
    }
    if (cache != NULL) {
        fprintf(stderr, "Error: ya existe un cache inicializado. Libérelo antes de crear uno nuevo.\n");
        return -1;
    }
    Cache *c = (Cache *)malloc(sizeof(Cache));
    if (!c) {
        fprintf(stderr, "Error: falló malloc al crear la estructura del cache.\n");
        return -1;
    }
    c->capacity = size;
    c->count = 0;
    c->head = c->tail = NULL;
    /* se puede iniciar inicializar nodos vacíos 
    de momento dejo la lista vacía y lista para añadir en proximos commits */
    cache = c;
    printf("Cache created with size %d\n", size);
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
