#include "../inc/puzzle.h"

void mx_init_shuffle_buttons(t_shuffle *shuffle, t_number_button *shuffle_buttons) {
    for (int i = 0; i < 16; i++) {
        if (i >= 0 && i <= 3) {
            shuffle_buttons[i].value =  shuffle->grid[0][i];
        }
        if (i >= 4 && i <= 7) {
            shuffle_buttons[i].value  =  shuffle->grid[1][i - 4];
        }
        if (i >= 8 && i <= 11) {
            shuffle_buttons[i].value  =  shuffle->grid[2][i - 8];
        }
        if (i >= 12 && i <= 15) {
            shuffle_buttons[i].value  =  shuffle->grid[3][i - 12];
        }
    }
}

void mx_init_grid(t_shuffle *shuffle, t_number_button *shuffle_buttons) {
    int num = 1;
    shuffle->grid = (int **)malloc(SHUFFLE_SIZE * sizeof(int *));
    for (int i = 0; i < SHUFFLE_SIZE; i++) {
        shuffle->grid[i] = (int *)malloc(SHUFFLE_SIZE * sizeof(int));
        for (int j = 0; j < SHUFFLE_SIZE; j++) {
            shuffle->grid[i][j] = num++;
        }
    }
    mx_init_shuffle_buttons(shuffle, shuffle_buttons);
    shuffle->empty_row = SHUFFLE_SIZE - 1;
    shuffle->empty_col = SHUFFLE_SIZE - 1;
    shuffle->grid[shuffle->empty_row][shuffle->empty_col] = 0;
}


void mx_move_empty(t_shuffle *shuffle, int new_row, int new_col) {
    shuffle->grid[shuffle->empty_row][shuffle->empty_col] = shuffle->grid[new_row][new_col];
    shuffle->grid[new_row][new_col] = 0;
    shuffle->empty_row = new_row;
    shuffle->empty_col = new_col;
    shuffle->is_changed = 1;
}

void mx_shuffle_tiles(t_shuffle *shuffle, t_number_button *shuffle_buttons, int moves) {
    int random_move;
    for (int i = 0; i < moves; i++) {
        // Генерируем случайное число от 0 до 3, где каждое число соответствует направлению хода
        random_move = rand() % 4;
        switch (random_move) {
            case 0:
                if (shuffle->empty_row < SHUFFLE_SIZE - 1) {
                    mx_move_empty(shuffle, shuffle->empty_row + 1, shuffle->empty_col);

                }
                break;
            case 1:
                if (shuffle->empty_row > 0) {
                    mx_move_empty(shuffle, shuffle->empty_row - 1, shuffle->empty_col);
                }
                break;
            case 2:
                if (shuffle->empty_col < SHUFFLE_SIZE - 1) {
                    mx_move_empty(shuffle, shuffle->empty_row, shuffle->empty_col + 1);
                }
                break;
            case 3:
                if (shuffle->empty_col > 0) {
                    mx_move_empty(shuffle, shuffle->empty_row, shuffle->empty_col - 1);
                }
                break;
        }
        mx_init_shuffle_buttons(shuffle, shuffle_buttons);
    }
}

void mx_move_tiles(t_shuffle *shuffle, t_number_button *shuffle_buttons, int direction) {
    switch (direction) {
        case 0:
            if (shuffle->empty_row < SHUFFLE_SIZE - 1) {
                mx_move_empty(shuffle, shuffle->empty_row + 1, shuffle->empty_col);
            }
            break;
        case 1:
            if (shuffle->empty_row > 0) {
                mx_move_empty(shuffle, shuffle->empty_row - 1, shuffle->empty_col);
            }
            break;
        case 2:
            if (shuffle->empty_col < SHUFFLE_SIZE - 1) {
                mx_move_empty(shuffle, shuffle->empty_row, shuffle->empty_col + 1);
            }
            break;
        case 3:
            if (shuffle->empty_col > 0) {
                mx_move_empty(shuffle, shuffle->empty_row, shuffle->empty_col - 1);
            }
            break;
    }
    mx_init_shuffle_buttons(shuffle, shuffle_buttons);
}
