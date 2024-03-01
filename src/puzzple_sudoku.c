#include "../inc/puzzle.h"
#include "../inc/header.h"

void mx_init_sudoku(t_sudoku *sudoku) {
    sudoku->is_call = 1;
    sudoku->is_changed = 1;
    sudoku->sudoku_grid = (int **)malloc(GRID_SIZE * sizeof(int *));
    for (int i = 0; i < GRID_SIZE; i++) {
        sudoku->sudoku_grid[i] = (int *)malloc(GRID_SIZE * sizeof(int));
        for (int j = 0; j < GRID_SIZE; j++) {
            sudoku->sudoku_grid[i][j] = 0;
        }
    }
    sudoku->solution_grid = (int **)malloc(GRID_SIZE * sizeof(int *));
    for (int i = 0; i < GRID_SIZE; i++) {
        sudoku->solution_grid[i] = (int *)malloc(GRID_SIZE * sizeof(int));
        for (int j = 0; j < GRID_SIZE; j++) {
            sudoku->solution_grid[i][j] = 0;
        }
    }
    sudoku->selected_col = 0;
    sudoku->selected_row = 0;
    mx_generate_random_sudoku(sudoku->solution_grid);
    int used_numbers[GRID_SIZE] = {0};
    for (int i = 0; i < GRID_SIZE; i++) {
        int is_ok = 0;
        while (!is_ok) {
            int number_of_grid = rand() % GRID_SIZE;
            int is_found = 0;
            for (int j = 0; j < GRID_SIZE; j++) {
                if (sudoku->solution_grid[i][number_of_grid] == used_numbers[j]) {
                    is_found = 1;
                    break;
                }
            }
            if (!is_found) {
                used_numbers[i] = sudoku->solution_grid[i][number_of_grid];
                sudoku->sudoku_grid[i][number_of_grid] = sudoku->solution_grid[i][number_of_grid];
                is_ok = 1;
            }
        }
    }
    if (DEV_MODE) {
        mx_print_sudoku(sudoku->solution_grid);
    }
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            sudoku->solution_grid[i][j] = sudoku->sudoku_grid[i][j];
        }
    }
}

int mx_sudoku(SDL_Renderer *renderer, SDL_Window *window, TTF_Font *font, t_level *level, t_man *man,
                        t_sudoku *sudoku) {
    int cur_level = 4;
    cur_level = mx_puzzle_sudoku_input_handler(window, sudoku);
    mx_cant_be_changed(sudoku->sudoku_grid, sudoku->solution_grid);

    if (sudoku->is_changed) {
        mx_draw_grid_sudoku(renderer, font, sudoku);
        sudoku->is_changed = 0;
        if (mx_check_solution(sudoku->sudoku_grid)) {
            cur_level = 0;
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Congratulations!", "You've solved Sudoku!", window);
            if (level->puzzles == NULL) {
                level->puzzles = (SDL_Rect *)malloc(sizeof(SDL_Rect));
            } else {
                SDL_Rect *new_binary_puzzle = (SDL_Rect *)realloc(level->puzzles, (level->puzzle_number + 1) * sizeof(SDL_Rect));

                if (new_binary_puzzle == NULL) {
                    fprintf(stderr, "Failed to reallocate memory for puzzles\n");
                    exit(EXIT_FAILURE);
                }

                level->puzzles = new_binary_puzzle;
            }
            level->puzzles[level->puzzle_number].w = 96 * 2 + 20;
            level->puzzles[level->puzzle_number].h = (48 + man->position.h) * 2 + 30;
            level->puzzles[level->puzzle_number].x = man->position.x - level->puzzles[level->puzzle_number].w / 2 - 10;
            level->puzzles[level->puzzle_number].y = man->position.y - level->puzzles[level->puzzle_number].h / 2.5 - 15;


            // Увеличение счетчика
            level->puzzle_number++;
        }
    }


    if (cur_level != 4) {
        sudoku->is_call = 0;
    }

    return cur_level;
}
