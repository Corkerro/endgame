#include "../inc/header.h"

int mx_save(int level_number, int x, int y) {
    int _x = 0;
    int _y = 0;

    FILE *file2 = fopen(SAVE_FILE, "r");
    if (file2 != NULL) {
        fscanf(file2, "%d", &_x);
        fscanf(file2, "%d %d", &_x, &_y);
    }
    FILE *file = fopen(SAVE_FILE, "w");

    // Проверяем, удалось ли открыть файл
    if (file == NULL) {
        return 0;
    }

    fprintf(file, "%d", level_number);
    if (level_number == -1) {
        fprintf(file, "\n%d %d", x, y);
    } else {
        fprintf(file, "\n%d %d", _x, _y);
    }

    // Закрываем файл
    fclose(file);
    return 1;
}

int mx_save_read(void) {
    int level_number = MENU;
    FILE *file = fopen(SAVE_FILE, "r");
    if (file == NULL) {
        FILE *file2 = fopen(SOLVED_FILE, "w");
        fprintf(file2, "%d", 0);
        fclose(file2);
        file = fopen(SAVE_FILE, "w");
        fprintf(file, "%d\n0 0", level_number);
        fclose(file);
        return level_number;
    }
    fscanf(file, "%d", &level_number);

    fclose(file);

    return level_number;
}
