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

    printf("CacheLRU - demo (fase: add/search/get)\n");
    printf("Escribe 'lru help' para ver los comandos.\n\n");

    while (1) {
        // Prompt en amarillo
        printf(ANSI_YELLOW PROMPT ANSI_RESET);

        if (!fgets(line, sizeof(line), stdin)) {
            putchar('\n');
            break;
        }

        line[strcspn(line, "\n")] = 0; // eliminar salto de línea
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
            show_help();
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
            int r = create_cache(size);
            if (r == 0) {
                // Mensaje ya mostrado en create_cache
            }
            continue;
        }

        // Agregar elemento
        if (strncmp(cmd, "lru add", 7) == 0) {
            char *arg = skip_spaces(cmd + 7);
            if (strlen(arg) == 0) {
                fprintf(stderr, ANSI_RED "Uso: lru add <caracter>\n" ANSI_RESET);
                continue;
            }

            char ch = (char) toupper((unsigned char) arg[0]);
            if (ch < 'A' || ch > 'Z') {
                fprintf(stderr, ANSI_RED "Error: el elemento debe ser una letra A-Z.\n" ANSI_RESET);
                continue;
            }
            lru_add(ch);
            continue;
        }

        // Buscar elemento
        if (strncmp(cmd, "lru search", 10) == 0) {
            char *arg = skip_spaces(cmd + 10);
            if (strlen(arg) == 0) {
                fprintf(stderr, ANSI_RED "Uso: lru search <caracter>\n" ANSI_RESET);
                continue;
            }

            char ch = (char) toupper((unsigned char) arg[0]);
            if (ch < 'A' || ch > 'Z') {
                fprintf(stderr, ANSI_RED "Error: el elemento debe ser una letra A-Z.\n" ANSI_RESET);
                continue;
            }
            lru_search(ch);
            continue;
        }

        // Obtener elemento
        if (strncmp(cmd, "lru get", 7) == 0) {
            char *arg = skip_spaces(cmd + 7);
            if (strlen(arg) == 0) {
                fprintf(stderr, ANSI_RED "Uso: lru get <caracter>\n" ANSI_RESET);
                continue;
            }

            char ch = (char) toupper((unsigned char) arg[0]);
            if (ch < 'A' || ch > 'Z') {
                fprintf(stderr, ANSI_RED "Error: el elemento debe ser una letra A-Z.\n" ANSI_RESET);
                continue;
            }
            lru_get(ch);
            continue;
        }

        fprintf(stderr, ANSI_RED "Comando no reconocido: '%s' escribe 'lru help' para ver las opciones.\n" ANSI_RESET, cmd);
    }

    return 0;
}
