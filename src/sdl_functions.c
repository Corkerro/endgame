#include "../inc/header.h"

SDL_Texture *mx_load_texture(const char *filename, SDL_Renderer *renderer) {
    SDL_Surface *surface = IMG_Load(filename);
    if (surface == NULL)     {
        printf("Не удалось загрузить изображение %s! Ошибка SDL_image: %s\n", filename, IMG_GetError());
        return NULL;
    }

    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0, 0));

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL) {
        printf("Не удалось создать текстуру из изображения! Ошибка SDL: %s\n", SDL_GetError());
    }

    SDL_FreeSurface(surface);

    return texture;
}

// Рендеринг каждого кадра
void mx_do_render(SDL_Renderer *renderer, t_man *man, t_level *level, void (*render_man)(SDL_Renderer *renderer, t_man *man, t_level *level)) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    int mapX = -level->cameraX;
    int mapY = -level->cameraY;

    SDL_Rect mapRect = {mapX, mapY, level->width, level->height};
    SDL_RenderCopy(renderer, level->mapTexture, NULL, &mapRect);


    // Рисуем препятствия
    for (int i = 0; i < level->obstacles_number; ++i) {
        if (!level->obstacles[i].is_open) {
            SDL_Rect obstacleRect = {
                    level->obstacles[i].coords.x - level->cameraX,
                    level->obstacles[i].coords.y - level->cameraY,
                    level->obstacles[i].coords.w,
                    level->obstacles[i].coords.h
            };
            SDL_RenderCopy(renderer, level->doorTexture, NULL, &obstacleRect);
        }
    }

    render_man(renderer, man, level);
    if (level->is_press_e_inited && SDL_GetTicks() - level->press_e_time <= 100) {
        SDL_Rect press_e = {
                (WINDOW_WIDTH - 592 / 2) / 2,
                (WINDOW_HEIGHT - 200),
                592 / 2,
                180 / 2
        };
        SDL_Texture *pressTexture = mx_load_texture("resource/resources/press_button.png", renderer);
        SDL_RenderCopy(renderer, pressTexture, NULL, &press_e);
        SDL_DestroyTexture(pressTexture);
    }

    if (DEV_MODE) {
        TTF_Font *font = TTF_OpenFont("resource/resources/fonts/EnterCommand.ttf", 24);

        char *manString = (char *)malloc(50 * sizeof(char));
        char *manString2 = (char *)malloc(50 * sizeof(char));

        char *puzzle_num = (char *)malloc(50 * sizeof(char));

        sprintf(manString, "man_x: %d", man->position.x);
        sprintf(manString2, "man_y: %d",man->position.y);
        sprintf(puzzle_num, "puzzle_number: %d",level->puzzle_number);

        SDL_Color textColor = {255, 255, 255, 255};
        SDL_Surface *textSurface = TTF_RenderText_Solid(font, manString, textColor);
        SDL_Surface *textSurface2 = TTF_RenderText_Solid(font, manString2, textColor);
        SDL_Surface *textSurface3 = TTF_RenderText_Solid(font, puzzle_num, textColor);

        for (int i = 0; i < level->puzzle_number; i++) {
            char *puzzle_num_info = (char *)malloc(50 * sizeof(char));
            sprintf(puzzle_num_info, "puzzle_num_info: x: %d    y: %d    w: %d    h: %d", level->puzzles[i].x, level->puzzles[i].y, level->puzzles[i].w, level->puzzles[i].h);

            SDL_Surface *puzzle_num_info_Surface = TTF_RenderText_Solid(font, puzzle_num_info, textColor);
            mx_render_text(renderer, 10, 120 + 20 * i, puzzle_num_info_Surface);
            free(puzzle_num_info);
        }


        mx_render_text(renderer, 10, 10, textSurface);
        mx_render_text(renderer, 10, 30, textSurface2);
        mx_render_text(renderer, 10, 50, textSurface3);

        free(manString);
        free(manString2);
        free(puzzle_num);

    }

    SDL_RenderPresent(renderer);
}

// Перемещает камеру за игроком
void mx_update_camera(t_man *man, t_level *level) {
    level->cameraX = man->position.x - (WINDOW_WIDTH / 2);
    level->cameraY = man->position.y - (WINDOW_HEIGHT / 2);

    if (level->cameraX < 0)
        level->cameraX = 0;
    if (level->cameraY < 0)
        level->cameraY = 0;
    if (level->cameraX > level->width - WINDOW_WIDTH)
        level->cameraX = level->width - WINDOW_WIDTH;
    if (level->cameraY > level->height - WINDOW_HEIGHT)
        level->cameraY = level->height - WINDOW_HEIGHT;
}

Uint32 get_pixel(SDL_Surface *loadingSurface, int x, int y) {
    Uint32 *pixels = (Uint32*)loadingSurface->pixels;
    return pixels[(y * loadingSurface->pitch / 4) + x];
}

int mx_check_color(SDL_Surface *surface, int x, int y, Uint8 red_forbid, Uint8 green_forbid, Uint8 blue_forbid) {
    SDL_PixelFormat *fmt;
    Uint32 temp;
    Uint32 pixel = get_pixel(surface, x, y);
    Uint8 red, green, blue;

    fmt = surface->format;
    SDL_LockSurface(surface);
    SDL_UnlockSurface(surface);

    /* Get Red component */
    temp = pixel & fmt->Rmask;
    temp = temp >> fmt->Rshift;
    temp = temp << fmt->Rloss;
    red = (Uint8)temp;

    /* Get Green component */
    temp = pixel & fmt->Gmask;
    temp = temp >> fmt->Gshift;
    temp = temp << fmt->Gloss;
    green = (Uint8)temp;

    /* Get Blue component */
    temp = pixel & fmt->Bmask;
    temp = temp >> fmt->Bshift;
    temp = temp << fmt->Bloss;
    blue = (Uint8)temp;

    if (red == red_forbid && green == green_forbid && blue == blue_forbid) {
        return 1;
    } else {
        return 0;
    }
}

void mx_set_music_volume(int volume) {
    Mix_VolumeMusic(volume);
}
