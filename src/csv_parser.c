#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv_parser.h"

#define DELIMETER ',' // разделитель CSV

// Простое разделение строки на токены по запятой (без поддержки кавычек).
// Возвращает динамически выделенный массив полей; количество полей в *count.
// Вызывающий обязан освободить каждую строку и массив.

static char **split_line(const char *line, int *count) {
    char *copy = strdup(line);
    char *token;
    char **fields = NULL;
    int capacity = 8;
    *count = 0;

    fields = malloc(capacity * sizeof(char*));
    if (!fields || !copy) {
        free(fields);
        free(copy);
        return NULL;
    }

    token = strtok(copy, ",");
    while (token != NULL) {
        if (*count >= capacity) {
            capacity *= 2;
            fields = realloc(fields, capacity * sizeof(char *));
            if (!fields) {
                free(copy);
                return NULL;
            }
        }
        fields[*count] = strdup(token);
        (*count)++;
        token = strtok(NULL, ",");
    }
    
    free(copy);
    return fields;
}



// Открывает CSV-файл, считывает все строки и возвращает заполненную структуру CSVTable.
// Ожидается, что первая строка содержит заголовки столбцов.
CSVTable *read_csv(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Ошибка: не удалось открыть файл %s\n", filename);
        return NULL;
    }

    // Создание пустой таблицы
    CSVTable *table = malloc(sizeof(CSVTable));
    if (!table) {
        fclose(file);
        return NULL;
    }
    table->headers = NULL;
    table->rows = NULL;
    table->num_rows = 0;
    table->num_cols = 0;

    // Чтение файла построчно
    size_t len = 0;
    ssize_t read;
    char *line = NULL;

    int rows_capacity = 8;
    table->rows = malloc(rows_capacity * sizeof(char **));
    if (!table->rows) {
        free(table);
        fclose(file);
        return NULL;
    }

    int line_index = 0;
    // Основной цикл чтения
    while ((read = getline(&line, &len, file)) != -1) {
        // Удаляем \r\n в конце
        if (read > 0) {
            line[strcspn(line, "\r\n")] = '\0';
        }

        // Разделение строки на поля
        int field_count = 0;
        char **fields = split_line(line, &field_count);
        if (!fields) {
            fprintf(stderr, "Ошибка: нехватка памяти при разборе строки в %s\n", filename);
            // Корректное освобождение и прерывание
            break;
        }
        
        // Первая строка — заголовки
        if (line_index == 0) {
            if (field_count <= 0) {
                fprintf(stderr, "Ошибка: в файле %s отсутствуют заголовки.\n", filename);
                // Освобождаем текущие поля и выходим
                for (int i = 0; i < field_count; i++) free(fields[i]);
                free(fields);
                break;
            }
            table->headers = fields;
            table->num_cols = field_count;
        } else {
            // Проверяем согласованность количества колонок
            if (field_count != table->num_cols) {
                fprintf(stderr, "Ошибка: несоответствие числа колонок в файле %s (строка %d)\n", filename, line_index + 1);
                for (int i = 0; i < field_count; i++) free(fields[i]);
                free(fields);
                break;
            }
            // Расширяем буфер строк при необходимости
            if (table->num_rows >= rows_capacity) {
                rows_capacity *= 2;
                char ***new_rows = realloc(table->rows, rows_capacity * sizeof(char **));
                if (!new_rows) {
                    for (int i = 0; i < field_count; i++) free(fields[i]);
                    free(fields);
                    fprintf(stderr, "Ошибка: нехватка памяти для строк таблицы (%s)\n", filename);
                    break;
                }
                table->rows = new_rows;
        }
            // Сохраняем строку
            table->rows[table->num_rows] = fields;
            table->num_rows++;
        }
        line_index++;
    }

    free(line);
    fclose(file);

    // Если таблица некорректна (нет заголовков) — очистка и возврат NULL
    if (!table->headers || table->num_cols <= 0) {
        free_csv(table);
        return NULL;
    }
    return table;
}

 // Функция очистки памяти
void free_csv(CSVTable *table) {
    if (!table) return;
    // Заголовки
    if (table->headers) {
        for (int i = 0; i < table->num_cols; i++) {
            free(table->headers[i]);
        }
        free(table->headers);
    }
    // Строки
    if (table->rows) {
        for (int i = 0; i < table->num_rows; i++) {
            if (table->rows[i]) {
                for (int j = 0; j < table->num_cols; j++) {
                    free(table->rows[i][j]);
                }
                free(table->rows[i]);
            }
        }
        free(table->rows);
    }
    free(table);
}