#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <stddef.h>

typedef struct Node {
    char value;             
    struct Node *prev;
    struct Node *next;
} Node;

/* Estructura principal del cache LRU */
typedef struct {
    int capacity;             //amaño máximo del cache 
    int count;                // cantidad actual de elementos
    Node *head;               // nodo más recientemente usado (MRU) 
    Node *tail;               // nodo menos recientemente usado (LRU) 
} Cache;

/* Muestra la ayuda de comandos */
void show_help(void);

/* Crea el cache con tamaño 'size'.
 * Devuelve 0 si se creó correctamente, -1 en error (ej. size < 5 o ya creado).
 */
int create_cache(int size);

/* Libera la estructura del cache (si existe). Devuelve 0 si había algo que liberar, 1 si no había cache. */
int free_cache(void);

/* Indica si ya existe un cache inicializado (1) o no (0). */
int cache_exists(void);


/* Funciones de operaciones LRU */
void lru_add(char value);
int lru_search(char value);
void lru_get(char value);
void lru_print(void);

#endif 
