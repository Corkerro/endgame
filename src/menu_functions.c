#include "../inc/menu.h"
#include "../inc/header.h"

void mx_render_text(SDL_Renderer *renderer, int x, int y, SDL_Surface *textSurface) {

    // Render text surface
    if (textSurface == NULL) {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }

    // Create texture from surface
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (textTexture == NULL) {
        printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        return;
    }

    // Set rendering position and dimensions
    SDL_Rect destRect = {x, y, textSurface->w, textSurface->h};

    // Render the text texture
    SDL_RenderCopy(renderer, textTexture, NULL, &destRect);

    // Free resources
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

void mx_render_button(SDL_Renderer *renderer, TTF_Font *font, t_button *button) {
    SDL_Color textColor = {0, 0, 255, 255};  // Белый цвет для текста
    SDL_Color bgColor;

    if (button->isHovered) {
        bgColor.r = 0;
        bgColor.g = 0;
        bgColor.b = 255;
        bgColor.a = 255;
        SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND)); // Установка курсора в вид "рука"
    } else {
        bgColor.r = 0;
        bgColor.g = 0;
        bgColor.b = 0;
        bgColor.a = 255;
        SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW)); // Возвращение курсора в вид стрелки
    }

    // Отрисовка прямоугольника (фона кнопки)
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_Rect buttonRect = {button->x, button->y, button->width, button->height};
    SDL_RenderFillRect(renderer, &buttonRect);

    // Отрисовка текстуры кнопки (если она загружена)
    if (button->texture != NULL) {
        SDL_RenderCopy(renderer, button->texture, NULL, &buttonRect);
    }
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, button->text, textColor);
    mx_render_text(renderer, button->x + (button->width - textSurface->w) / 2, button->y + (button->height - textSurface->h - 15) / 2, textSurface);
}

void mx_render_sound_button(SDL_Renderer *renderer, TTF_Font *font, t_sound_button *button) {
    SDL_Color textColor = {0, 0, 255, 255};  // Белый цвет для текста
    SDL_Color bgColor;

    // Отрисовка прямоугольника (фона кнопки)
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_Rect buttonRect = {button->x, button->y, button->width, button->height};
    SDL_RenderFillRect(renderer, &buttonRect);

    // Отрисовка текстуры кнопки (если она загружена)

    if (button->texture != NULL) {
        SDL_RenderCopy(renderer, button->texture, NULL, &buttonRect);
    }
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, button->text, textColor);
    mx_render_text(renderer, button->x + 40, button->y + (button->height - textSurface->h - 15) / 2, textSurface);
}

void mx_render_menu(SDL_Renderer *renderer, TTF_Font *font, t_menu *menu) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    menu->continueButton.texture = mx_load_texture("resource/resources/menu/button.png", renderer);
    menu->startButton.texture = mx_load_texture("resource/resources/menu/button.png", renderer);
    menu->exitButton.texture = mx_load_texture("resource/resources/menu/button.png", renderer);
    if (!menu->bgButton.isOff)
        menu->bgButton.texture = mx_load_texture("resource/resources/menu/sound-on.png", renderer);
    else menu->bgButton.texture = mx_load_texture("resource/resources/menu/sound-off.png", renderer);

    // Отрисовка кнопки "Continue"
    if (!menu->is_first_game)
        mx_render_button(renderer, font, &menu->continueButton);

    // Отрисовка кнопки "Start"
    mx_render_button(renderer, font, &menu->startButton);

    // Отрисовка кнопки "Выключатель музыки"
    mx_render_sound_button(renderer, font, &menu->bgButton);

    // Отрисовка кнопки "Exit"
    mx_render_button(renderer, font, &menu->exitButton);

    SDL_RenderPresent(renderer);
}

void mx_init_menu_buttons(t_menu *menu, int is_first_game) {
    t_button continueButton = {WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 4 - 220 + 200, 300, 100, "Continue", 0, 0, NULL};
    char *start_button_txt = is_first_game ? "Start game" : "New game";
    t_button startButton = {WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 4 - 80 + 200, 300, 100, start_button_txt, 0, 0, NULL};
    t_button exitButton = {WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 4 + 200 + 200, 300, 100, "Exit", 0, 0, NULL};
    t_sound_button bgButton = {WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 4 + 60 + 200, 300, 100, "Turn off music", 0, NULL};
    menu->continueButton = continueButton;
    menu->startButton = startButton;
    menu->bgButton = bgButton;
    menu->exitButton = exitButton;
    menu->is_first_game = is_first_game;
}

void mx_destroy_buttons(t_menu *menu) {
    // Освобождение памяти для текстуры кнопки (если она загружена с использованием SDL2_image)
    SDL_DestroyTexture(menu->continueButton.texture);
    SDL_DestroyTexture(menu->startButton.texture);
    SDL_DestroyTexture(menu->exitButton.texture);
    SDL_DestroyTexture(menu->bgButton.texture);
}
