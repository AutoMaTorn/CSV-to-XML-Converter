#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_input.h"
#include "csv_parser.h"
#include "xml_writer.h"

int main() {
    int file_count = 0;
    char **file_list = get_file_list(&file_count);
    if (!file_list || file_count <= 0) {
        fprintf(stderr, "Ошибка: не указаны входные CSV-файлы.\n");
        return 1;
    }

    CSVTable **tables = malloc(sizeof(CSVTable *) * file_count);
    if (!tables) {
        fprintf(stderr, "Ошибка: нехватка памяти.\n");
        for (int i = 0; i < file_count; i++) {
            free(file_list[i]);
        }
        free(file_list);
        return 1;
    }

    int tables_count = 0;
    for (int i = 0; i < file_count; i++) {
        const char *filename = file_list[i];
        printf("Чтение: %s\n", filename);
        CSVTable *t = read_csv(filename);
        if (t) {
            tables[tables_count++] = t;
        } else {
            fprintf(stderr, "Предупреждение: файл пропущен: %s\n", filename);
        }
    }

    char output_name[1024];
    printf("Введите имя выходного XML-файла:\n> ");
    if (!fgets(output_name, sizeof(output_name), stdin)) {
        fprintf(stderr, "Ошибка чтения имени выходного файла.\n");
        // Освобождение ресурсов перед выходом
        for (int i = 0; i < tables_count; i++) {
            free_csv(tables[i]);
        }
        free(tables);
        for (int i = 0; i < file_count; i++) {
            free(file_list[i]);
        }
        free(file_list);
        return 1;
    }
    output_name[strcspn(output_name, "\r\n")] = '\0';

    if (tables_count > 0) {
        write_xml(output_name, tables, tables_count);
        printf("Готово: %s\n", output_name);
    } else {
        fprintf(stderr, "Нет корректно разобранных таблиц для записи XML.\n");
    }

    // Освобождаем всю выделенную память (шаг 5)
    for (int i = 0; i < tables_count; i++) {
        free_csv(tables[i]);
    }
    free(tables);
    for (int i = 0; i < file_count; i++) {
        free(file_list[i]);
    }
    free(file_list);

    return 0;
}

// 1. Получаем список CSV-файлов от пользователя
// 2. Читаем каждый CSV и сохраняем в массив структур CSVTable
// 3. Запрашиваем имя выходного XML-файла
// 4. Преобразуем все таблицы в единый XML
// 5. Освобождаем память
