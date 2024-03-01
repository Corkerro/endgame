#include "../inc/puzzle.h"
#include <stdio.h>

int mx_is_valid(int **grid, int row, int col, int num) {
    // Проверяем, нет ли числа num в текущей строке и столбце
    for (int x = 0; x < GRID_SIZE; ++x) {
        if (grid[row][x] == num || grid[x][col] == num) {
            return 0;
        }
    }

    int startRow = row - row % (GRID_SIZE / (GRID_SIZE / 2));
    int startCol = col - col % (GRID_SIZE / (GRID_SIZE / 2));
    for (int i = 0; i < (GRID_SIZE / (GRID_SIZE / 2)); ++i) {
        for (int j = 0; j < (GRID_SIZE / (GRID_SIZE / 2)); ++j) {
            if (grid[i + startRow][j + startCol] == num) {
                return 0;
            }
        }
    }

    return 1;
}

// Рекурсивная функция для генерации судоку
int mx_solve_sudoku(int **grid, int row, int col) {
    // Если мы достигли последней позиции в судоку, то судоку решено
    if (row == GRID_SIZE - 1 && col == GRID_SIZE) {
        return 1;
    }

    // Если достигнут конец строки, переходим к следующей строке
    if (col == GRID_SIZE) {
        row++;
        col = 0;
    }

    // Если текущая ячейка уже заполнена, пропускаем ее
    if (grid[row][col] != 0) {
        return mx_solve_sudoku(grid, row, col + 1);
    }

    // Пытаемся поставить числа от 1 до GRID_SIZE в текущую позицию
    for (int num = 1; num <= GRID_SIZE; ++num) {
        // Если число можно поставить в текущую позицию
        if (mx_is_valid(grid, row, col, num)) {
            // Поставляем число в текущую позицию
            grid[row][col] = num;

            // Пробуем решить судоку для следующей позиции
            if (mx_solve_sudoku(grid, row, col + 1)) {
                return 1; // Если судоку решено, возвращаем 1
            }

            // Если решение не возможно, возвращаемся назад и пытаемся другое число
            grid[row][col] = 0;
        }
    }

    return 0; // Если не удалось найти допустимое число для текущей позиции
}

// Функция для генерации случайного судоку
void mx_generate_random_sudoku(int **grid) {
    // Очищаем сетку перед генерацией
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            grid[i][j] = 0;
        }
    }

    // Генерируем случайное число от 1 до GRID_SIZE для первой ячейки
    int firstNum = rand() % GRID_SIZE + 1;
    grid[0][0] = firstNum;

    // Решаем судоку для получения заполненной сетки
    mx_solve_sudoku(grid, 0, 1);
}

int mx_check_solution(int **sudoku_grid) {
    // Проверка строк
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int i = 0; i < GRID_SIZE - 1; ++i) {
            for (int j = i + 1; j < GRID_SIZE; ++j) {
                if (sudoku_grid[row][i] == sudoku_grid[row][j] || sudoku_grid[row][i] == 0) {
                    return 0; // Повторяющееся число в строке
                }
            }
        }
    }

    // Проверка столбцов
    for (int col = 0; col < GRID_SIZE; col++) {
        for (int i = 0; i < GRID_SIZE - 1; ++i) {
            for (int j = i + 1; j < GRID_SIZE; ++j) {
                if (sudoku_grid[i][col] == sudoku_grid[j][col] || sudoku_grid[j][col] == 0) {
                    return 0; // Повторяющееся число в столбце
                }
            }
        }
    }

    // Проверка квадратных областей
    for (int startRow = 0; startRow < GRID_SIZE; startRow += (GRID_SIZE / 2)) {
        for (int startCol = 0; startCol < GRID_SIZE; startCol += (GRID_SIZE / 2)) {
            int seen[GRID_SIZE] = {0};
            for (int row = 0; row < (GRID_SIZE / 2); row++) {
                for (int col = 0; col < (GRID_SIZE / 2); col++) {
                    int num = sudoku_grid[startRow + row][startCol + col];
                    if (num != 0) {
                        if (seen[num - 1]) {
                            return 0; // Повторяющееся число в квадратной области
                        }
                        seen[num - 1] = 1;
                    }
                }
            }
        }
    }

    return 1; // Судоку решено правильно
}

void mx_cant_be_changed(int **sudoku_grid, int **solution_grid) {
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (solution_grid[i][j] != 0)
                sudoku_grid[i][j] = solution_grid[i][j];
        }
    }
}

void mx_print_sudoku(int **grid) {
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            printf("%d ", grid[i][j]);
        }
        printf("\n");
    }
}
