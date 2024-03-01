#ifndef ENDGAME_MENU_H
#define ENDGAME_MENU_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"

#include "structs.h"

#include <stdio.h>
#include <stdlib.h>

void mx_render_sound_button(SDL_Renderer *renderer, TTF_Font *font, t_sound_button *button);
void mx_destroy_buttons(t_menu *menu);

int mx_menu_call(SDL_Renderer *renderer, SDL_Window *window, int level_number, t_game_start *game);
int mx_menu_input_handler(SDL_Window *window, t_menu *menu, t_game_start *game);

int mx_pause_call(SDL_Renderer *renderer, SDL_Window *window, t_level *level, t_man *man, t_menu *menu, TTF_Font *font);
int mx_pause_input_handler(SDL_Window *window, t_menu *menu, t_level *level, t_man *man);

void mx_render_button(SDL_Renderer *renderer, TTF_Font *font, t_button *button);
void mx_render_menu(SDL_Renderer *renderer, TTF_Font *font, t_menu *menu);
void mx_init_menu_buttons(t_menu *menu, int is_first_game);


#endif //ENDGAME_MENU_H
