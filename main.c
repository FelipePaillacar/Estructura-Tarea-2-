#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>   // Para toupper()
#include "funciones.h"


#define PROMPT "lru> "
#define LINE_BUFSIZE 256

// Funcion auxiliar para omitir espacios iniciales
static char *skip_spaces(char *s) {
    while (*s == ' ' || *s == '\t')
        s++;
    return s;
}

int main(void) {
    char line[LINE_BUFSIZE];
    printf("CacheLRU - v2.0 (Hash Map & Strings)\n");
    printf("Escribe 'lru help' para ver los comandos.\n\n");

    while (1) {
        // Prompt en amarillo
        printf(ANSI_YELLOW PROMPT ANSI_RESET);
        if (!fgets(line, sizeof(line), stdin)) {
            putchar('\n');
            break;
        }

        line[strcspn(line, "\n")] = 0; // eliminar salto de linea
        char *cmd = skip_spaces(line);
        if (strlen(cmd) == 0)
            continue;
            
        // Salir del programa
        if (strcmp(cmd, "lru exit") == 0 || strcmp(cmd, "exit") == 0) {
            printf(ANSI_YELLOW "Liberando cache y saliendo...\n" ANSI_RESET);
            free_cache();
            break;
        }
        
        // Mostrar ayuda
        if (strcmp(cmd, "lru help") == 0 || strcmp(cmd, "help") == 0) {
            show_help(); // Asumo que show_help() se actualizo en funciones.c
            continue;
        }
        
        // Crear cache
        if (strncmp(cmd, "lru create", 10) == 0) {
            char *arg = skip_spaces(cmd + 10);
            if (strlen(arg) == 0) {
                fprintf(stderr, ANSI_RED "Uso: lru create <size>\n" ANSI_RESET);
                continue;
            }
            int size = atoi(arg);
            create_cache(size);
            continue;
        }
        
        // Agregar elemento 
        if (strncmp(cmd, "lru add", 7) == 0) {
            char *arg = skip_spaces(cmd + 7);
            if (strlen(arg) == 0) {
                fprintf(stderr, ANSI_RED "Uso: lru add <clave_texto>\n" ANSI_RESET);
                continue;
            }
            lru_add(arg);
            continue;
        }
        
        // Buscar elemento 
        if (strncmp(cmd, "lru search", 10) == 0) {
            char *arg = skip_spaces(cmd + 10);
            if (strlen(arg) == 0) {
                fprintf(stderr, ANSI_RED "Uso: lru search <clave_texto>\n" ANSI_RESET);
                continue;
            }
            lru_search(arg); // Necesitaras implementar lru_search() con hash
            continue;
        }
        
        // Obtener elemento 
        if (strncmp(cmd, "lru get", 7) == 0) {
            char *arg = skip_spaces(cmd + 7);
            if (strlen(arg) == 0) {
                fprintf(stderr, ANSI_RED "Uso: lru get <clave_texto>\n" ANSI_RESET);
                continue;
            }
            lru_get(arg);
            continue;
        }

        // Imprimir estado del cache
        if (strcmp(cmd, "lru print") == 0) {
            lru_print();
            continue;
        }

        //  MOSTRAR ESTADiSTICAS DEL CACHE
        if (strcmp(cmd, "lru stats") == 0) {
            lru_stats();
            continue;
        }

        fprintf(stderr, ANSI_RED "Comando no reconocido: '%s' escribe 'lru help' para ver las opciones.\n" ANSI_RESET, cmd);
    }
    return 0;
}
