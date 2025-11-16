#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xml_writer.h"
#include "csv_parser.h"

// Экранирует спецсимволы XML в новой, динамически выделенной строке.
// Вызывающий обязан освободить результат через free().
static char *escape_xml(const char *input) {
    if (!input) {
        char *empty = malloc(1);
        if (empty) empty[0] = '\0';
        return empty;
    }

    size_t length = strlen(input);
    // Предварительная оценка: худший случай ~ 6x (например, '&' -> "&amp;")
    size_t capacity = length * 6 + 1;
    char *buffer = malloc(capacity);
    if (!buffer) {
        return NULL;
    }

    size_t write_index = 0;
    for (size_t i = 0; i < length; i++) {
        const char ch = input[i];
        const char *replacement = NULL;
        size_t replacement_len = 0;

        switch (ch) {
            case '&': replacement = "&amp;";  replacement_len = 5; break;
            case '<': replacement = "&lt;";   replacement_len = 4; break;
            case '>': replacement = "&gt;";   replacement_len = 4; break;
            case '\"': replacement = "&quot;"; replacement_len = 6; break;
            case '\'': replacement = "&apos;"; replacement_len = 6; break;
            default: break;
        }

        if (replacement) {
            if (write_index + replacement_len + 1 > capacity) {
                capacity = (capacity * 2) + replacement_len + 1;
                char *new_buffer = realloc(buffer, capacity);
                if (!new_buffer) {
                    free(buffer);
                    return NULL;
                }
                buffer = new_buffer;
            }
            memcpy(buffer + write_index, replacement, replacement_len);
            write_index += replacement_len;
        } else {
            if (write_index + 2 > capacity) {
                capacity = (capacity * 2) + 2;
                char *new_buffer = realloc(buffer, capacity);
                if (!new_buffer) {
                    free(buffer);
                    return NULL;
                }
                buffer = new_buffer;
            }
            buffer[write_index++] = ch;
        }
    }
    buffer[write_index] = '\0';
    return buffer;
}

void write_xml(const char *output_filename, CSVTable **tables, int num_tables) {
    if (!output_filename || !tables || num_tables <= 0) {
        fprintf(stderr, "Ошибка: некорректные аргументы для write_xml\n");
        return;
    }

    FILE *out = fopen(output_filename, "w");
    if (!out) {
        fprintf(stderr, "Ошибка: не удалось открыть файл для записи: %s\n", output_filename);
        return;
    }

    fprintf(out, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(out, "<dataset>\n");

    for (int t = 0; t < num_tables; t++) {
        CSVTable *table = tables[t];
        if (!table) {
            // Пропускаем пустые таблицы
            continue;
        }

        // Имя таблицы неизвестно (в текущем интерфейсе оно не передаётся),
        // используем индекс как атрибут для стабильности.
        fprintf(out, "  <table index=\"%d\">\n", t);

        // Проверка валидности структуры
        if (!table->headers || table->num_cols <= 0) {
            fprintf(stderr, "Предупреждение: у таблицы %d нет заголовков или num_cols <= 0. Пропуск.\n", t);
            fprintf(out, "  </table>\n");
            continue;
        }

        for (int r = 0; r < table->num_rows; r++) {
            fprintf(out, "    <row>\n");
            char **row = table->rows ? table->rows[r] : NULL;

            for (int c = 0; c < table->num_cols; c++) {
                const char *header_raw = table->headers[c];
                const char *value_raw = (row && row[c]) ? row[c] : "";

                char *header = escape_xml(header_raw ? header_raw : "");
                char *value = escape_xml(value_raw);
                if (!header || !value) {
                    free(header);
                    free(value);
                    fclose(out);
                    fprintf(stderr, "Ошибка: нехватка памяти при экранировании XML.\n");
                    return;
                }

                fprintf(out, "      <col name=\"%s\">%s</col>\n", header, value);

                free(header);
                free(value);
            }
            fprintf(out, "    </row>\n");
        }

        fprintf(out, "  </table>\n");
    }

    fprintf(out, "</dataset>\n");
    fclose(out);
}


