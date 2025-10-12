#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILES 100
#define MAX_FILENAME 256

char **get_file_list(int *count) {
    char **files = malloc(MAX_FILES * sizeof(char *));
    char buffer[MAX_FILENAME];
    *count = 0;

    printf("Введите имена CSV-файлов (введите 'stop' для завершения):\n");

    while (1) {
        printf("Файл %d: ", count + 1);
        if (scanf("%255s", buffer) != 1) break;

        if (strcmp(buffer, "stop") == 0) break;

        files[*count] = strdup(buffer);
        (*count)++;
    }

    return files;
}