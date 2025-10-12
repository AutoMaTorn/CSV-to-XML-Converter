#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv_parser.h"

#define DELIMETER ',' // разделитель CSV

// разделение строк на токены

static char **split_line(const char *line, int *count) {
    char *copy = strdup(line); // создание копии строки 
    char *token;                // 
    char **fields = NULL;
    int capacity = 8;
    *count = 0;

    fields = malloc(capacity * sizeof(char*)); // динмический массив для хранение указателей на каждое поле

    token = strtok(copy, ","); // поочерёдно возвращает подстроки, разделённые запятыми
    while (token != NULL) {
        if (*count >= capacity) {
            capacity *= 2;
            fields = realloc(fields, capacity * sizeof(char *));
        }
        fields[*count] = strdup(token); // сохраняем каждую в массив fields
        (*count)++;
        token = strtok(NULL, ",");
    }
    
    free(copy);
    return fields;
}



// открывает CSV-файл, считывает все строки и возвращает заполненную структуру CSVTable
CSVTable *read_csv(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Ошибка: не удалось открыть файл %s\n", filename);
        return NULL;
    }

    // cоздаём пустую таблицу в которую позже запишем заголовки и строки
    CSVTable *table = malloc(sizeof(CSVTable));
    table->headers = NULL;
    table->rows = NULL;
    table->num_cols = 0;
    table->num_cols = 0;

    //чтение файла построчно 

    //чтение цикла

    // разделение заголовков и данных


    // ФУНКЦИЯ очистки памяти

}