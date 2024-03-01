#include "../inc/header.h"
#include "../inc/menu.h"

int mx_menu_call(SDL_Renderer *renderer, SDL_Window *window, int level_number, t_game_start *game) {
    t_menu menu;
    mx_init_menu_buttons(&menu, game->is_first_game);

    // Load font
    TTF_Font *font = TTF_OpenFont("resource/resources/fonts/EnterCommand.ttf", 24);  // Change the path to your font file

    if (font == NULL) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return 1;
    }

    while (level_number == MENU) {
        mx_render_menu(renderer, font, &menu);
        level_number = mx_menu_input_handler(window, &menu, game);

        // 30 fps
        SDL_Delay(32);
    }

    mx_destroy_buttons(&menu);
    TTF_CloseFont(font);
    return level_number;
}

int mx_pause_call(SDL_Renderer *renderer, SDL_Window *window, t_level *level, t_man *man, t_menu *menu, TTF_Font *font) {
    mx_render_menu(renderer, font, menu);

    level->is_paused = mx_pause_input_handler(window, menu, level, man);

    // 30 fps
    SDL_Delay(32);

    return level->is_paused;
}
