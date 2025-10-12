#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 1024
#define MAX_FILES 100

// ввод списка файлов
char **get_file_list(int *count) {
    char **files = malloc(MAX_FILES * sizeof(char *));
    char input[MAX_INPUT];
    *count = 0;

    printf("Введите имена CSV-файлов (введите 'stop' для завершения):\n");

    // считываем всю строку с пробелами
    fgets(input, sizeof(input), stdin);

    // убираем символ новой строки если есть
    input[strcspn(input, "\n")] = '\0';

    // разбиваем строку по пробелам
    char *token = strtok(input, " ");
    while (token != NULL && *count < MAX_FILES) {
        files[*count] = strdup(token); // копируем строку
        (*count)++;
        token = strtok(NULL, " ");
    }

    return files;
}