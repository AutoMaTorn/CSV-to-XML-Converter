#include <stdio.h>
#include <stdlib.h>
#include "file_input.h"

int main() {
    int file_count;
    char **file_list = get_file_list(&file_count);

    printf("\nВы ввели %d файл(ов):\n", file_count);
    for (int i = 0; i < file_count; i++) {
        printf("  %s\n", file_list[i]);
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
