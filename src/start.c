    #include "../inc/header.h"
    #include "../inc/menu.h"

    void mx_start(SDL_Renderer *renderer, SDL_Window *window) {
        t_game_start game;

        game.old_level = mx_save_read();
        game.is_first_game = 0;
        int level_number = game.old_level == MENU ? MENU : game.old_level;

        int is_paused = 1;

        while (level_number != EXIT) {
            if (mx_save_read() == MENU) {
                game.is_first_game = 1;
                game.old_level = -1;
            }
            switch (level_number) {
                case MENU:
                    level_number = mx_menu_call(renderer, window, level_number, &game);
                    break;
                case -1:
                    game.old_level = -1;
                    level_number = mx_level_main_call(renderer, window, level_number, is_paused);
                    break;
                    break;
                case 0:
                    game.old_level = 0;
                    level_number = mx_level00_call(renderer, window, level_number, is_paused);
                    break;
                case 1:
                    game.old_level = 1;
                    level_number = mx_level01_call(renderer, window, level_number, is_paused);
                    break;
                case 2:
                    game.old_level = 2;
                    level_number = mx_level02_call(renderer, window, level_number, is_paused);
                    break;
                case 3:
                    game.old_level = 3;
                    level_number = mx_level03_call(renderer, window, level_number, is_paused);
                    break;
                case 4:
                    game.old_level = 4;
                    level_number = mx_level04_call(renderer, window, level_number, is_paused);
                    break;
                case 5:
                    game.old_level = 5;
                    level_number = mx_level05_call(renderer, window, level_number, is_paused);
                    break;
                case 6:
                    game.old_level = 6;
                    level_number = mx_level06_call(renderer, window, level_number, is_paused);
                    break;
            }
            is_paused = 0;
        }
    }
