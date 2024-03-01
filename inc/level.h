#pragma once

#ifndef ENDGAME_LEVEL_H
#define ENDGAME_LEVEL_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#include <stdio.h>
#include <stdlib.h>

#include "structs.h"

int mx_load_collision(SDL_Renderer *renderer, t_level *level, int level_number);
void mx_level_gameplay(int number_of_level, SDL_Renderer *renderer, SDL_Window *window, t_level *level, t_man *man, int is_paused);
void mx_read_doors(const char *filename, t_level *level);

#endif //ENDGAME_LEVEL_H
