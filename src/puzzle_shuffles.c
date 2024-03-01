#include "../inc/puzzle.h"

int mx_shuffle(SDL_Renderer *renderer, SDL_Window *window, TTF_Font *font, t_level *level, t_man *man,
                        t_shuffle *shuffle, t_number_button *shuffle_buttons) {
    int cur_level = 3;
    cur_level = mx_puzzle_shuffle_input_handler(window, shuffle, shuffle_buttons);

    if (shuffle->is_changed) {
        int is_good = 1;
        for (int i = 0; i < 15; i++) {
            if (shuffle_buttons[i].value != i+1) {
                is_good = 0;
                break;
            }
        }
        if (is_good) {
            mx_print_win_message(renderer, font, level, man, shuffle_buttons, 16, &cur_level);
        } else {
            mx_render_number_and_buttons(renderer, font, "Shuffles", shuffle_buttons, 16);
        }
        shuffle->is_changed = 0;
    }
    if (renderer == NULL || font == NULL || shuffle_buttons == NULL || window == NULL || level == NULL || man == NULL || shuffle == NULL) {

    }

    if (cur_level == 0) {
        shuffle->is_call = 0;
    }

    return cur_level;
}
