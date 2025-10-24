#include "funciones.h"
#include <ctype.h>

// Variable global del cache
static Cache *cache = NULL;

/* ----------------------------------------------------
   Funcion de Hash 
---------------------------------------------------- */
static unsigned long hash_string(char *str) 
{
    unsigned long hash = 5381; //semilla para itar colisiones 
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; 

    return hash; 
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
    printf(ANSI_GREEN "lru add <array>" ANSI_RESET "     -> Agrega un array (string) al cache\n");
    printf(ANSI_GREEN "lru search <array>" ANSI_RESET "  -> Busca un array y devuelve su posicion\n");
    printf(ANSI_GREEN "lru get <array>" ANSI_RESET "     -> Usa un array y la mueve al frente (MRU)\n");
    printf(ANSI_GREEN "lru print" ANSI_RESET "           -> Muestra el estado actual del cache\n");
    printf(ANSI_GREEN "lru stats" ANSI_RESET "           -> Muestra estadisticas del cache (hash)\n");
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
    
    c->table_size = size * 2; //tambien para prevenir colisiones 
    c->table = (HashNode **)calloc(c->table_size, sizeof(HashNode *));
    if (!c->table) {
        fprintf(stderr, ANSI_RED "Error: fallo calloc al crear la tabla hash.\n" ANSI_RESET);
        free(c);
        return -1;
    }
    
    cache = c;
    printf(ANSI_GREEN "Cache creado con capacidad %d (tabla hash de %d)\n" ANSI_RESET, size, c->table_size);
    return 0;
}

/* ----------------------------------------------------
   Liberar cache
---------------------------------------------------- */
int free_cache(void) {
    if (!cache)
     {
        printf(ANSI_YELLOW "[Aviso] No habia cache para liberar.\n" ANSI_RESET);
        return 1;
    }

    Node *cur = cache->head;
    while (cur) 
    {
        Node *next = cur->next;
        free(cur->array); // free a la copia de la cadena
        free(cur);
        cur = next;
    }

    for (int i = 0; i < cache->table_size; i++) 
    {
        HashNode *entry = cache->table[i];
        while (entry) {
            HashNode *next_entry = entry->next;
            free(entry->array); // Liberar la copia de la cadena
            free(entry);
            entry = next_entry;
        }
    }
    
    free(cache->table);
    free(cache);
    cache = NULL;
    
    printf(ANSI_GREEN "Cache liberado correctamente.\n" ANSI_RESET);
    return 0;
}

// --- MANEJO DE LA LISTA DOBLE ---

static void list_add_to_front(Node *node) 
{
    node->next = cache->head;
    node->prev = NULL;
    if (cache->head) cache->head->prev = node;
    cache->head = node;
    if (!cache->tail) cache->tail = node;
}

static void list_remove_node(Node *node) 
{
    if (node->prev) node->prev->next = node->next;
    else cache->head = node->next;
    
    if (node->next) node->next->prev = node->prev;
    else cache->tail = node->prev;
}

static void move_to_front(Node *node) 
{
    if (node == cache->head) return;
    list_remove_node(node);
    list_add_to_front(node);
}


// HASH

static HashNode* find_in_table(char *array) 
{
    unsigned long h = hash_string(array) % cache->table_size;
    HashNode *entry = cache->table[h];

    while (entry) {
        if (strcmp(entry->array, array) == 0) {
            return entry;
        }
        entry = entry->next;
    }
    return NULL;
}

static void table_insert(char *array, Node *list_node)
 {
    unsigned long h = hash_string(array) % cache->table_size;

    HashNode *entry = (HashNode *)malloc(sizeof(HashNode));
    entry->array = strdup(array); 
    entry->list_node = list_node; 
    
    entry->next = cache->table[h];
    cache->table[h] = entry;
}

static int table_remove(char *array)
 {
    unsigned long h = hash_string(array) % cache->table_size;
    
    HashNode *entry = cache->table[h];
    HashNode *prev = NULL;

    while (entry) {
        if (strcmp(entry->array, array) == 0) {
            if (prev) prev->next = entry->next;
            else cache->table[h] = entry->next;
            
            free(entry->array);
            free(entry);
            return 1;
        }
        prev = entry;
        entry = entry->next;
    }
    return 0;
}

/* ----------------------------------------------------
   Agregar elemento (LRU add)
---------------------------------------------------- */
void lru_add(char *array) {
    if (!cache) {
        fprintf(stderr, ANSI_RED "Error: crea primero un cache.\n" ANSI_RESET);
        return;
    }
    if (!array || strlen(array) == 0)
     {
        fprintf(stderr, ANSI_RED "Error: el array no puede estar vacio.\n" ANSI_RESET);
        return;
    }

    HashNode *entry = find_in_table(array);
    
    if (entry) 
    {
        printf(ANSI_GREEN "Elemento '%s' ya existia; movido al frente.\n" ANSI_RESET, array);
        move_to_front(entry->list_node);
        lru_print();
        return;
    }


    printf(ANSI_GREEN "Elemento '%s' agregado al cache.\n" ANSI_RESET, array);

    if (cache->count >= cache->capacity) 
    {
        Node *lru_node = cache->tail;
        printf(ANSI_YELLOW "Cache lleno: se eliminara el LRU ('%s').\n" ANSI_RESET, lru_node->array);

        table_remove(lru_node->array);
        list_remove_node(lru_node);
        
        free(lru_node->array);
        free(lru_node);
        
        cache->count--;
    }

    // Añadir el nuevo elemento
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->array = strdup(array); // Copia la clave
    
    list_add_to_front(new_node);
    table_insert(array, new_node);
    
    cache->count++;
    lru_print();
}

/* ----------------------------------------------------
   Buscar elemento (LRU search)
---------------------------------------------------- */
int lru_search(char *array) {
    if (!cache) 
    {
        fprintf(stderr, ANSI_RED "Error: no hay cache inicializado.\n" ANSI_RESET);
        return -1;
    }
    
    Node *cur = cache->head;
    int pos = 1;
    while (cur) 
    {
        if (strcmp(cur->array, array) == 0)
        {
            printf(ANSI_GREEN "Elemento '%s' encontrado en posicion %d\n" ANSI_RESET, array, pos);
            return pos;
        }
        cur = cur->next;
        pos++;
    }
    printf(ANSI_YELLOW "Elemento '%s' no encontrado.\n" ANSI_RESET, array);
    return -1;
}

/* ----------------------------------------------------
   Obtener elemento (LRU get)
---------------------------------------------------- */
void lru_get(char *array) {
    if (!cache) {
        fprintf(stderr, ANSI_RED "Error: no hay cache inicializado.\n" ANSI_RESET);
        return;
    }

    HashNode *entry = find_in_table(array);

    if (entry) 
    {
        move_to_front(entry->list_node);
        printf(ANSI_GREEN "Elemento '%s' usado; movido al frente (MRU).\n" ANSI_RESET, array);
        lru_print();
    } else 
    {
        printf(ANSI_YELLOW "Elemento '%s' no encontrado.\n" ANSI_RESET, array);
    }
}

/* ----------------------------------------------------
   Imprimir estado del cache
---------------------------------------------------- */
void lru_print(void) {
    if (!cache || cache->count == 0) {
        printf(ANSI_YELLOW "[Cache vacio o no creado]\n" ANSI_RESET);
        return;
    }
    Node *cur = cache->head;
    printf("Cache (MRU -> LRU): ");
    while (cur) 
    {
        printf("[%s] ", cur->array);
        cur = cur->next;
    }
    printf("\n");
}


/* ----------------------------------------------------
   Mostrar estadisticas del cache
---------------------------------------------------- */
void lru_stats(void) {
    if (!cache) {
        printf(ANSI_YELLOW "[No hay cache creado]\n" ANSI_RESET);
        return;
    }
    
    printf(ANSI_CYAN "=== ESTADiSTICAS DEL CACHE (v2.0 Hash) ===\n" ANSI_RESET);
    printf("Capacidad total: %d\n", cache->capacity);
    printf("Elementos actuales: %d\n", cache->count);
    printf("Espacio disponible: %d\n", cache->capacity - cache->count);
    
    // Calcular porcentaje de ocupacion
    float ocupacion = (cache->count * 100.0) / cache->capacity;
    printf("Tasa de ocupacion: %.1f%%\n", ocupacion);
    
    // Estadisticas de la tabla hash
    printf("Tamaño tabla hash: %d\n", cache->table_size);
    
    // Calcular factor de carga y colisiones
    int buckets_ocupados = 0;
    int total_colisiones = 0;
    int max_colisiones = 0;
    
    for (int i = 0; i < cache->table_size; i++) {
        HashNode *entry = cache->table[i];
        int colisiones_en_bucket = 0;
        
        if (entry) {
            buckets_ocupados++;
            entry = entry->next; // Saltar el primer elemento
            
            while (entry) {
                colisiones_en_bucket++;
                total_colisiones++;
                entry = entry->next;
            }
            
            if (colisiones_en_bucket > max_colisiones) {
                max_colisiones = colisiones_en_bucket;
            }
        }
    }
    
    printf("Buckets ocupados: %d/%d (%.1f%%)\n", 
           buckets_ocupados, cache->table_size, 
           (buckets_ocupados * 100.0) / cache->table_size);
    printf("Total de colisiones: %d\n", total_colisiones);
    printf("Maximo colisiones por bucket: %d\n", max_colisiones);
    
    // Mostrar MRU y LRU actuales
    if (cache->head) {
        printf("Elemento mas reciente (MRU): '%s'\n", cache->head->array);
    } else {
        printf("Elemento mas reciente (MRU): [vacio]\n");
    }
    
    if (cache->tail) {
        printf("Elemento menos reciente (LRU): '%s'\n", cache->tail->array);
    } else {
        printf("Elemento menos reciente (LRU): [vacio]\n");
    }
}