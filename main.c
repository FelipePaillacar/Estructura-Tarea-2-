#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "funciones.h"

#define PROMPT "lru> "
#define LINE_BUFSIZE 256

static char *skip_spaces(char *s) {
    while (*s == ' ' || *s == '\t') s++;
    return s;
}

int main(void) 
{
    char line[LINE_BUFSIZE];
    printf("CacheLRU - demo (fase: add/search/get)\n");
    printf("Escribe 'lru help' para ver los comandos.\n\n");

    while (1) {
        printf(PROMPT);
        if (!fgets(line, sizeof(line), stdin)) {
            putchar('\n');
            break; 
        }

        line[strcspn(line, "\n")] = 0;
        char *cmd = skip_spaces(line);
        if (strlen(cmd) == 0) continue;

        if (strcmp(cmd, "lru exit") == 0 || strcmp(cmd, "exit") == 0) {
            printf("Liberando cache y saliendo...\n");
            free_cache();
            break;
        }
        if (strcmp(cmd, "lru help") == 0 || strcmp(cmd, "help") == 0) {
            show_help();
            continue;
        }
        if (strncmp(cmd, "lru create", 10) == 0) {
            char *arg = skip_spaces(cmd + 10);
            if (strlen(arg) == 0) {
                fprintf(stderr, "Uso: lru create <size>\n");
                continue;
            }
            int size = atoi(arg);
            create_cache(size);
            continue;
        }
        if (strncmp(cmd, "lru add", 7) == 0) {
            char *arg = skip_spaces(cmd + 7);
            if (strlen(arg) == 0) {
                fprintf(stderr, "Uso: lru add <caracter>\n");
                continue;
            }
            lru_add(arg[0]);
            continue;
        }
        if (strncmp(cmd, "lru search", 10) == 0) {
            char *arg = skip_spaces(cmd + 10);
            if (strlen(arg) == 0) {
                fprintf(stderr, "Uso: lru search <caracter>\n");
                continue;
            }
            lru_search(arg[0]);
            continue;
        }
        if (strncmp(cmd, "lru get", 7) == 0) {
            char *arg = skip_spaces(cmd + 7);
            if (strlen(arg) == 0) {
                fprintf(stderr, "Uso: lru get <caracter>\n");
                continue;
            }
            lru_get(arg[0]);
            continue;
        }

        fprintf(stderr, "Comando no reconocido: '%s' — escribe 'lru help' para ver las opciones.\n", cmd);
    }
    return 0;
}
