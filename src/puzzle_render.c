#include "../inc/puzzle.h"
#include "../inc/header.h"

void mx_print_win_message(SDL_Renderer *renderer, TTF_Font *font, t_level *level, t_man *man, t_number_button *number_buttons, int number_of_buttons, int *cur_level) {
    *cur_level = 0;

    // Вывести заголовок и кнопки еще раз
    mx_render_number_and_buttons(renderer, font, "Congratulations", number_buttons, number_of_buttons);

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

    // Задержка на 2 секунды
    SDL_Delay(2000);
}

void mx_render_number_button(SDL_Renderer *renderer, TTF_Font *font, t_number_button *button) {
    SDL_Color textColor = {255, 255, 255, 255};  // Белый цвет для текста
    SDL_Color bgColor;
    bgColor.r = 0;
    bgColor.g = 0;
    bgColor.b = 0;
    bgColor.a = 255;

    // Отрисовка прямоугольника (фона кнопки)
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_Rect buttonRect = {button->x, button->y, button->width, button->height};
    SDL_RenderFillRect(renderer, &buttonRect);

    // Отрисовка текстуры кнопки (если она загружена)
    button->texture = mx_load_texture("resource/resources/puzzles/shuffle.png", renderer);
    if (button->texture != NULL) {
        SDL_RenderCopy(renderer, button->texture, NULL, &buttonRect);
    }
    SDL_DestroyTexture(button->texture);
    char *button_value = (char *)malloc(2 * sizeof(int));
    sprintf(button_value, "%d", button->value);
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, button_value, textColor);
    free(button_value);
    mx_render_text(renderer, button->x + (button->width - textSurface->w) / 2, button->y + (button->height - textSurface->h) / 2, textSurface);
}

void mx_render_number_and_buttons(SDL_Renderer *renderer, TTF_Font *font, const char *text,
                                  t_number_button *number_buttons, int number_buttons_count) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Color textColor = {255, 255, 255, 255};

    // Создание нового шрифта с нужным размером
    TTF_Font *largeFont = TTF_OpenFont("resource/resources/fonts/EnterCommand.ttf", 128);
    if (!largeFont) {
        // Обработка ошибки, если шрифт не удалось загрузить
        printf("Ошибка при загрузке шрифта: %s\n", TTF_GetError());
        return;
    }

    // Отрисовка заголовка (числа)
    SDL_Surface *textSurface = TTF_RenderText_Solid(largeFont, text, textColor);
    mx_render_text(renderer, WINDOW_WIDTH / 2 - textSurface->w / 2, WINDOW_HEIGHT / 2 - 200, textSurface);

    // Отрисовка кнопок
    for (int i = 0; i < number_buttons_count; i++) {
        mx_render_number_button(renderer, font, &number_buttons[i]);
    }

    // Закрытие созданного шрифта
    TTF_CloseFont(largeFont);

    SDL_RenderPresent(renderer);
}


void mx_draw_grid_sudoku(SDL_Renderer *renderer, TTF_Font *font, t_sudoku *sudoku) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Определите отступы сверху и справа
    int left_offset = (WINDOW_WIDTH - SCREEN_WIDTH) / 2; // Отступ слева
    int top_offset = (WINDOW_HEIGHT- SCREEN_HEIGHT) / 2;  // Отступ сверху

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i <= GRID_SIZE; ++i) {
        SDL_Rect line = {left_offset + i * CELL_SIZE, top_offset, 1, SCREEN_HEIGHT};
        SDL_RenderFillRect(renderer, &line);
        SDL_Rect line2 = {left_offset, top_offset + i * CELL_SIZE, SCREEN_WIDTH, 1};
        SDL_RenderFillRect(renderer, &line2);
    }

    int selectedRow = sudoku->selected_row;
    int selectedCol = sudoku->selected_col;
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface *surface = NULL;
    SDL_Texture *texture = NULL;
    char text[2];
    SDL_Rect textRect;
    textRect.w = CELL_SIZE;
    textRect.h = CELL_SIZE;
    textRect.y = top_offset;
    for (int i = 0; i < GRID_SIZE; ++i) {
        textRect.x = left_offset;
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (sudoku->sudoku_grid[i][j] != 0) {
                sprintf(text, "%d", sudoku->sudoku_grid[i][j]);
                surface = TTF_RenderText_Solid(font, text, textColor);
                texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_FreeSurface(surface);
                textRect.x = left_offset + j * CELL_SIZE;
                SDL_RenderCopy(renderer, texture, NULL, &textRect);
                SDL_DestroyTexture(texture);
            }
        }
        textRect.y += CELL_SIZE;
    }

    // Рисуем контур вокруг выбранной клетки
    if (selectedRow != -1 && selectedCol != -1) {
        SDL_Rect highlightRect = {left_offset + selectedCol * CELL_SIZE, top_offset + selectedRow * CELL_SIZE, CELL_SIZE, CELL_SIZE};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &highlightRect);
    }
    sudoku->selected_row = selectedRow;
    sudoku->selected_col = selectedCol;

    SDL_RenderPresent(renderer);
}
