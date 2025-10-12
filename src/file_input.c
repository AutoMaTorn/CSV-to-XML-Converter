#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_input.h"

#define MAX_INPUT 1024
#define MAX_FILES 100

char **get_file_list(int *count) {
    char **files = malloc(MAX_FILES * sizeof(char *));
    char input[MAX_INPUT];
    *count = 0;

    printf("Введите имена CSV-файлов через пробел:\n> ");
    fgets(input, sizeof(input), stdin);

    // Убираем \n
    input[strcspn(input, "\n")] = '\0';

    char *token = strtok(input, " ");
    while (token != NULL && *count < MAX_FILES) {
        files[*count] = strdup(token);
        (*count)++;
        token = strtok(NULL, " ");
    }

    return files;
}
