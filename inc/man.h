#pragma once

#ifndef ENDGAME_MAN_H
#define ENDGAME_MAN_H

#include "structs.h"

void mx_init_man(t_man *man, int x, int y, t_level *level, int scale) ;
void mx_render_man(SDL_Renderer *renderer, t_man *man, t_level *level);

#endif //ENDGAME_MAN_H
