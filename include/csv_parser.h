#ifndef CSV_PARSER_H
#define CSV_PARSER_H

typedef struct {
    char **headers;      // Заголовки (имена столбцов)
    char ***rows;        // Строки данных (массив строковых массивов)
    int num_rows;
    int num_cols;
} CSVTable;

CSVTable *read_csv(const char *filename); // Чтение CSV файла
void free_csv(CSVTable *table);          // Освобождение памяти, занятой CSVTable

#endif
