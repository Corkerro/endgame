#pragma once

#ifndef ENDGAME_HEADER_H
#define ENDGAME_HEADER_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"

#include "structs.h"
#include "man.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WINDOW_WIDTH 1720
#define WINDOW_HEIGHT 1080
#define EXIT -999
#define MENU -10
#define PAUSE -11

#define SAVE_FILE "resource/resources/levels/save.txt"
#define SOLVED_FILE "resource/resources/levels/solved.txt"
#define DEV_MODE 0 // 1 - ON, 0 - OFF

void mx_start(SDL_Renderer *renderer, SDL_Window *window);

void mx_write_puzzles(t_level *level, t_man *man);
int mx_save(int level_number, int x, int y);
int mx_save_read(void);

SDL_Texture *mx_load_texture(const char *filename, SDL_Renderer *renderer);
void mx_render_text(SDL_Renderer *renderer, int x, int y, SDL_Surface *textSurface);
void mx_do_render(SDL_Renderer *renderer, t_man *man, t_level *level, void (*render_man)(SDL_Renderer *renderer, t_man *man, t_level *level));
void mx_update_camera(t_man *man, t_level *level);
int mx_check_color(SDL_Surface *surface, int x, int y, Uint8 red_forbid, Uint8 green_forbid, Uint8 blue_forbid);
void mx_set_music_volume(int volume);

int mx_input_handler(t_man *man, t_level *level);

int mx_level00_call(SDL_Renderer *renderer, SDL_Window *window, int level_number, int is_paused);
int mx_level01_call(SDL_Renderer *renderer, SDL_Window *window, int level_number, int is_paused);
int mx_level02_call(SDL_Renderer *renderer, SDL_Window *window, int level_number, int is_paused);
int mx_level03_call(SDL_Renderer *renderer, SDL_Window *window, int level_number, int is_paused);
int mx_level04_call(SDL_Renderer *renderer, SDL_Window *window, int level_number, int is_paused);
int mx_level05_call(SDL_Renderer *renderer, SDL_Window *window, int level_number, int is_paused);
int mx_level06_call(SDL_Renderer *renderer, SDL_Window *window, int level_number, int is_paused);
int mx_level_main_call(SDL_Renderer *renderer, SDL_Window *window, int level_number, int is_paused);


#endif //ENDGAME_HEADER_H
