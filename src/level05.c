#include "../inc/header.h"
#include "../inc/level.h"
#include "../inc/menu.h"

int mx_level05_call(SDL_Renderer *renderer, SDL_Window *window, int level_number, int is_paused) {
    t_level level;
    level.width = 4096;
    level.height = 4096;
    level.cameraX = (level.width - WINDOW_WIDTH) / 2;
    level.cameraY = (level.height - WINDOW_HEIGHT) / 2;
    if (DEV_MODE)
        level.mapTexture = mx_load_texture("resource/resources/levels/level-5/dev-mod.png", renderer);
    else level.mapTexture = mx_load_texture("resource/resources/levels/level-5/bg.png", renderer);
    level.doorTexture = mx_load_texture("resource/resources/door.png", renderer);
    level.number = 5;
    level.puzzle_number = 0;
    mx_read_doors("resource/resources/levels/level-5/level.info", &level);
    // Загрузка коллизии
    if (mx_load_collision(renderer, &level, level_number))
        return EXIT;

    t_man man;
    mx_init_man(&man, 1600, 1550, &level, 1);
    if (!mx_save(level.number, man.position.x, man.position.y))
        return EXIT;

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    Mix_Music *backgroundMusic = Mix_LoadMUS("resource/resources/sound/background.mp3");
    Mix_VolumeMusic(20);

    // Loading background music
    Mix_PlayMusic(backgroundMusic, -1);

    mx_level_gameplay(level_number, renderer, window, &level, &man, is_paused);

    SDL_DestroyTexture(level.mapTexture);
    SDL_DestroyTexture(level.doorTexture);
    SDL_DestroyTexture(level.collisionTexture);
    SDL_FreeSurface(level.mapCollisionSurface);
    free(level.puzzles);
    Mix_FreeMusic(backgroundMusic);
    return level.number;
}
