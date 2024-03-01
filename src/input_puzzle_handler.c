#include "../inc/header.h"
#include "../inc/puzzle.h"

int mx_puzzle_binary_input_handler(SDL_Window *window, t_number_button *number_buttons, int number_buttons_count,
                                   int number_buttons_size) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_WINDOWEVENT_CLOSE: {
                if (window) {
                    SDL_DestroyWindow(window);
                }
            } break;
            case SDL_KEYDOWN: {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        // Переключение между игровым режимом и режимом меню
                        return 0;
                }
            } break;
            case SDL_MOUSEBUTTONDOWN: {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                if (mouseX >= number_buttons[0].x && mouseX <=number_buttons[0].x + number_buttons_size
                    && mouseY >= number_buttons[0].y && mouseY <= number_buttons[0].y + number_buttons[0].height) {
                    for (int i = 0; i < number_buttons_count; ++i) {
                        if (mouseX >= number_buttons[i].x && mouseX <= number_buttons[i].x + number_buttons[i].width) {
                            number_buttons[i].value = (number_buttons[i].value + 1) % 2;
                        }
                    }
                }
            } break;
        }
    }

    return 2;
}

int mx_puzzle_shuffle_input_handler(SDL_Window *window, t_shuffle *shuffle, t_number_button *buttons) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_WINDOWEVENT_CLOSE: {
                if (window) {
                    SDL_DestroyWindow(window);
                }
            } break;
            case SDL_KEYDOWN: {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        // Переключение между игровым режимом и режимом меню
                        return 0;
                }
            } break;
            case SDL_KEYUP: {
                switch (event.key.keysym.sym) {
                    case SDLK_w:
                        mx_move_tiles(shuffle, buttons, 0);
                        break;
                    case SDLK_UP:
                        mx_move_tiles(shuffle, buttons, 0);
                        break;
                    case SDLK_s:
                        mx_move_tiles(shuffle, buttons, 1);
                        break;
                    case SDLK_DOWN:
                        mx_move_tiles(shuffle, buttons, 1);
                        break;
                    case SDLK_a:
                        mx_move_tiles(shuffle, buttons, 2);
                        break;
                    case SDLK_LEFT:
                        mx_move_tiles(shuffle, buttons, 2);
                        break;
                    case SDLK_d:
                        mx_move_tiles(shuffle, buttons, 3);
                        break;
                    case SDLK_RIGHT:
                        mx_move_tiles(shuffle, buttons, 3);
                        break;
                }
            } break;
        }
    }

    return 3;
}

int mx_puzzle_sudoku_input_handler(SDL_Window *window, t_sudoku *sudoku) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_WINDOWEVENT_CLOSE: {
                if (window) {
                    SDL_DestroyWindow(window);
                }
            } break;
            case SDL_KEYDOWN: {
                int selectedRow = sudoku->selected_row;
                int selectedCol = sudoku->selected_col;
                if (event.key.keysym.sym == SDLK_BACKSPACE && selectedRow != -1 && selectedCol != -1) {
                    sudoku->sudoku_grid[selectedRow][selectedCol] = 0; // Удаление числа из клетки
                    sudoku->is_changed = 1;
                } else if (event.key.keysym.sym >= SDLK_1 && event.key.keysym.sym <= SDLK_9 && selectedRow != -1 && selectedCol != -1) {
                    int num = event.key.keysym.sym - SDLK_0;
                    sudoku->sudoku_grid[selectedRow][selectedCol] = num; // Вставка числа в клетку
                    sudoku->is_changed = 1;
                    // Не изменять выделение клетки после ввода цифры
                } else if ((event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w) && selectedRow > 0) {
                    selectedRow--;
                    sudoku->is_changed = 1;
                } else if ((event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s) && selectedRow < GRID_SIZE - 1) {
                    selectedRow++;
                    sudoku->is_changed = 1;
                } else if ((event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a) && selectedCol > 0) {
                    selectedCol--;
                    sudoku->is_changed = 1;
                } else if ((event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d) && selectedCol < GRID_SIZE - 1) {
                    selectedCol++;
                    sudoku->is_changed = 1;
                }
                sudoku->selected_row = selectedRow;
                sudoku->selected_col = selectedCol;
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        // Переключение между игровым режимом и режимом меню
                        return 0;
                }
            } break;
            case SDL_MOUSEBUTTONDOWN: {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                sudoku->selected_row = (mouseY - WINDOW_HEIGHT / 4 + 30) / CELL_SIZE;
                sudoku->selected_col = (mouseX - WINDOW_WIDTH / 3 + 10) / CELL_SIZE;
                if (sudoku->selected_row >= GRID_SIZE ) {
                    sudoku->selected_row = 3;
                }
                if (sudoku->selected_row < 0 ) {
                    sudoku->selected_row = 0;
                }
                if (sudoku->selected_col >= GRID_SIZE ) {
                    sudoku->selected_col = 3;
                }
                if (sudoku->selected_col < 0 ) {
                    sudoku->selected_col = 0;
                }
                sudoku->is_changed = 1;
                break;
            }
        }
    }

    return 4;
}
