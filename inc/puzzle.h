#pragma once

#ifndef ENDGAME_PUZZLE_H
#define ENDGAME_PUZZLE_H

#include <SDL2/SDL.h>
#include "SDL2/SDL_ttf.h"
#include "structs.h"

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
#define GRID_SIZE 4
#define CELL_SIZE (SCREEN_WIDTH / GRID_SIZE)
#define PUZZLE_CODE 2
#define PUZZLE_NUMBER 3

// puzzle_render.c
void mx_render_number_button(SDL_Renderer *renderer, TTF_Font *font, t_number_button *button);
void mx_render_number_and_buttons(SDL_Renderer *renderer, TTF_Font *font, const char *text,
                                  t_number_button *number_buttons, int number_buttons_count);
void mx_print_win_message(SDL_Renderer *renderer, TTF_Font *font, t_level *level, t_man *man, t_number_button *number_buttons, int number_of_buttons, int *cur_level);
void mx_draw_grid_sudoku(SDL_Renderer *renderer, TTF_Font *font, t_sudoku *sudoku);

// puzzle_binary_password.c
int mx_binary_password(SDL_Renderer *renderer, SDL_Window *window, TTF_Font *font, t_level *level, t_man *man,
                       t_number_button *number_buttons, int number_of_buttons, int original_password, int *new_gess_number);

// puzzle_shuffles.c
int mx_shuffle(SDL_Renderer *renderer, SDL_Window *window, TTF_Font *font, t_level *level, t_man *man,
               t_shuffle *shuffle,t_number_button *shuffle_buttons);
void mx_init_grid(t_shuffle *shuffle, t_number_button *shuffle_buttons) ;
void mx_move_empty(t_shuffle *shuffle, int new_row, int new_col);
void mx_shuffle_tiles(t_shuffle *shuffle, t_number_button *shuffle_buttons, int moves);
void mx_move_tiles(t_shuffle *shuffle, t_number_button *shuffle_buttons, int direction);
void mx_init_shuffle_buttons(t_shuffle *shuffle, t_number_button *shuffle_buttons);


// puzzle_sudoku.c
int mx_sudoku(SDL_Renderer *renderer, SDL_Window *window, TTF_Font *font, t_level *level, t_man *man,
              t_sudoku *sudoku);
void mx_init_sudoku(t_sudoku *sudoku);

// puzzle_sudoku_functions.c
void mx_print_sudoku(int **grid);
int mx_is_valid(int **grid, int row, int col, int num);
int mx_solve_sudoku(int **grid, int row, int col);
void mx_generate_random_sudoku(int **grid);
int mx_check_solution(int **sudoku_grid);
void mx_cant_be_changed(int **sudoku_grid, int **solution_grid);

// input_handler.c
int mx_puzzle_binary_input_handler(SDL_Window *window, t_number_button *number_buttons, int number_buttons_count, int number_buttons_size);
int mx_puzzle_shuffle_input_handler(SDL_Window *window, t_shuffle *shuffle, t_number_button *buttons);
int mx_puzzle_sudoku_input_handler(SDL_Window *window, t_sudoku *sudoku);

#endif //ENDGAME_PUZZLE_H
