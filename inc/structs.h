#pragma once

#ifndef ENDGAME_STRUCTS_H
#define ENDGAME_STRUCTS_H
#include "SDL2/SDL.h"
#define SHUFFLE_SIZE 4

typedef struct
{
    SDL_Rect position;
    int lastX, lastY, speed, scale;
    short life;
    int numberOfFrames, frameDuration;
    char *currentURL;
    char *rightURL;
    char *leftURL;
    char *topURL;
    char *downURL;
    SDL_Texture *texture;
    int currentFrame;
    Uint32 lastFootstepTime;
    Uint32 lastUpdateTime;
    int direction;
} t_man;
typedef struct
{
    int **grid;
    int empty_row;
    int empty_col;
    int is_call;
    int is_changed;
} t_shuffle;
typedef struct
{
    int **sudoku_grid;
    int **solution_grid;
    int selected_row;
    int selected_col;
    int is_call;
    int is_changed;
} t_sudoku;
typedef struct
{
    SDL_Rect coords;
    int is_open;
} t_obstacle;
typedef struct
{
    int width, height;
    int cameraX, cameraY;
    int number, is_paused;
    SDL_Texture *mapTexture;
    SDL_Texture *collisionTexture;
    SDL_Texture *doorTexture;
    SDL_Surface *mapCollisionSurface;
    int puzzle_number;
    int obstacles_number;
    SDL_Rect *puzzles;
    t_obstacle *obstacles;
    Uint64 press_e_time;
    int is_press_e_inited;
} t_level;

typedef struct
{
    int x, y, width, height;
    char *text;
    int isClicked;
    int isHovered;
    SDL_Texture *texture;
} t_button;

typedef struct
{
    int x, y, width, height;
    char *text;
    int isOff;
    SDL_Texture *texture;
} t_sound_button;

typedef struct
{
    int x, y, width, height, is_first_game;
    t_button continueButton;
    t_button startButton;
    t_sound_button bgButton;
    t_button exitButton;
} t_menu;

typedef struct
{
    int is_first_game, old_level;
} t_game_start;

typedef struct
{
    int x, y, width, height;
    int value;
    SDL_Texture *texture;
} t_number_button;
typedef struct
{
    t_number_button *number_buttons;
    int number_of_buttons, button_size, gap_between_buttons, button_block_width;
    int guess_number;
} t_guess_the_bit;

#endif //ENDGAME_STRUCTS_H
