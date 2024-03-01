#include "../inc/header.h"

SDL_Window *window;
SDL_Renderer *renderer;


int main(void) {
    // Инциализация библиотеки
    srand(time(NULL));
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }
    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        printf("SDL_ttf could not be initialized! SDL_ttf Error: %s\n", TTF_GetError());
        return 1;
    }
    int iniFlags = Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG );
    if (!iniFlags) {
        printf("Couldn't initialize\n");
    }

    // Создание окна
    if (SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT,  0, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }

    mx_start(renderer, window);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}
