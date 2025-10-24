#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <stddef.h>

// Colores ANSI
#define ANSI_RED    "\x1b[31m"
#define ANSI_GREEN  "\x1b[32m"
#define ANSI_YELLOW "\x1b[33m"
#define ANSI_RESET  "\x1b[0m"
#define ANSI_CYAN    "\x1b[36m"


/* ================================================================
   ESTRUCTURAS DEL SIMULADOR LRU
   ================================================================ */
typedef struct Node {
    char value;
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct {
    int capacity;
    int count;
    Node *head;
    Node *tail;
} Cache;

/* ================================================================
     FUNCIONES PRINCIPALES
   ================================================================ */
void show_help(void);
int create_cache(int size);
int free_cache(void);
int cache_exists(void);
void lru_add(char value);
int lru_search(char value);
void lru_get(char value);
void lru_print(void);

#endif
