/**
 * Простые утилиты для работы со строками.
 */
#include <string.h>
#include "utils.h"

// Удаляет завершающие \r и \n из строки (на месте).
// Возвращает указатель на ту же строку.
char *trim_newline(char *str) {
    if (!str) return str;
    size_t n = strlen(str);
    while (n > 0 && (str[n - 1] == '\n' || str[n - 1] == '\r')) {
        str[n - 1] = '\0';
        n--;
    }
    return str;
}

