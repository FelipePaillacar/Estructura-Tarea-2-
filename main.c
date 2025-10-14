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
    printf("CacheLRU - demo (fase: create)\n");
    printf("Escribe 'lru help' para ver los comandos.\n\n");
    while (1) {
        printf(PROMPT);
        if (!fgets(line, sizeof(line), stdin)) {
            putchar('\n');
            break; 
        }
        line[strcspn(line, "\n")] = 0; //quita el salto de linea 
        char *cmd = skip_spaces(line);
        if (strlen(cmd) == 0) continue;
        if (strcmp(cmd, "lru exit") == 0 || strcmp(cmd, "exit") == 0) {
            printf("Clearing cache (if any) and exiting...\n");
            free_cache();
            break;
        }
        if (strcmp(cmd, "lru help") == 0 || strcmp(cmd, "help") == 0) {
            show_help();
            continue;
        }
        if (strncmp(cmd, "lru create", 10) == 0) {
            char *arg = cmd + 10;
            arg = skip_spaces(arg);
            if (strlen(arg) == 0) {
                fprintf(stderr, "Error: falta el argumento <size>. Uso: lru create <size>\n");
                continue;
            }
            char *endptr = NULL;
            long val = strtol(arg, &endptr, 10);
            if (endptr == arg || *endptr != '\0' || val <= 0) {
                fprintf(stderr, "Error: tamaño inválido: '%s'. Debe ser un entero positivo .\n", arg);
                continue;
            }
            int size = (int)val;
            if (create_cache(size) == 0) {       
            }
            continue;
        }
        fprintf(stderr, "Comando no reconocido: '%s' — escribe 'lru help' para ver las opciones.\n", cmd);
    }
    return 0;
}
