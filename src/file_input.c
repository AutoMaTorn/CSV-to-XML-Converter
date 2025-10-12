#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_input.h"

#define MAX_INPUT 1024      // Максимальная длина вводимой строки
#define MAX_FILES 100        // Максимальное количество файлов

// Функция для получения списка файлов из ввода пользователя.
// Возвращает массив строк с именами файлов и записывает их количество в count.
char **get_file_list(int *count) {
    char **files = malloc(MAX_FILES * sizeof(char *));   // Выделяем память под массив указателей на строки
    char input[MAX_INPUT];                             // Буфер для ввода пользователя
    *count = 0;                                         // Инициализируем счетчик файлов

    printf("Введите имена CSV-файлов через пробел:\n> ");
    fgets(input, sizeof(input), stdin);                // Считываем строку с именами файлов

    // Убираем символ новой строки, если он есть
    input[strcspn(input, "\n")] = '\0';

    // Разбиваем строку на отдельные имена файлов по пробелу
    char *token = strtok(input, " ");
    while (token != NULL && *count < MAX_FILES) {
        files[*count] = strdup(token);                  // Копируем имя файла в массив
        (*count)++;                                    // Увеличиваем счетчик
        token = strtok(NULL, " ");                      // Переходим к следующему имени файла
    }

    return files;                                      // Возвращаем массив имен файлов
}
