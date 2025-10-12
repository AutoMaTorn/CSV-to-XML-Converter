#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 1024
#define MAX_FILES 100

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


int main() {
    int file_count;
    char **file_list = get_file_list(&file_count);

    printf("\nВы ввели %d файл(ов):\n", file_count);
    for (int i = 0; i < file_count; i++) {
        printf("  %s\n", file_list[i]);
        free(file_list[i]); // очищаем каждую строку
    }
    free(file_list); // очищаем массив указателей

    return 0;
}

    // 1. Получаем список CSV-файлов от пользователя
    // 2. Читаем каждый CSV и сохраняем в массив структур CSVTable
    // 3. Запрашиваем имя выходного XML-файла
    // 4. Преобразуем все таблицы в единый XML
    // 5. Освобождаем память
