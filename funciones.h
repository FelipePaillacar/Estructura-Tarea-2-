#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

// Colores ANSI
#define ANSI_RED    "\x1b[31m"
#define ANSI_GREEN  "\x1b[32m"
#define ANSI_YELLOW "\x1b[33m"
#define ANSI_CYAN   "\x1b[36m"
#define ANSI_RESET  "\x1b[0m"

/* =============
   ESTRUCTURAS
   ============= */
   
// El nodo de la lista enlazada. Almacena el dato real.
typedef struct Node 
{
    char *array;        
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct HashNode 
{
    char *array;            
    Node *list_node;          // Puntero al nodo en la lista doble
    struct HashNode *next;    // Siguiente en caso de colisión
} HashNode;

// El Cache ahora incluye la tabla hash
typedef struct {
    int capacity;
    int count;
    Node *head;         
    Node *tail;        
    HashNode **table;   // tabla hash 
    int table_size;     // tama;o del array de la tabla
} Cache;


/* ================================================================
     FUNCIONES PRINCIPALES (prototipos con char*)
   ================================================================ */

void show_help(void);
int create_cache(int size);
int free_cache(void);

void lru_add(char *array);
int lru_search(char *array);
void lru_get(char *array);
void lru_print(void);

#endif 
