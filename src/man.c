#include "../inc/header.h"

void mx_render_man(SDL_Renderer *renderer, t_man *man, t_level *level) {
    SDL_Rect frameRect = {
            man->currentFrame * man->position.w,
            0,
            man->position.w,
            man->position.h
    };

    SDL_Rect destRect = {
            man->position.x - level->cameraX,
            man->position.y - level->cameraY,
            man->position.w * man->scale,
            man->position.h * man->scale
    };

    SDL_RenderCopy(renderer, man->texture, &frameRect, &destRect);
}


void mx_init_man(t_man *man, int x, int y, t_level *level, int scale) {
    man->position.x = x;
    man->position.y = y;
    man->lastX = man->position.x;
    man->lastY = man->position.y;
    man->position.w = 48;
    man->position.h = 48;
    man->speed = 5;
    man->scale = scale;
    man->numberOfFrames = 2;
    man->frameDuration = 300;
    man->rightURL = "resource/resources/main_pers/walk__right-Sheet.png";
    man->leftURL = "resource/resources/main_pers/walk__left-Sheet.png";
    man->topURL = "resource/resources/main_pers/walk__top-Sheet.png";
    man->downURL = "resource/resources/main_pers/walk__down-Sheet.png";
    man->currentURL = man->rightURL;
    man->currentFrame = 0;
    man->lastUpdateTime = SDL_GetTicks();
    man->lastFootstepTime = SDL_GetTicks();
    mx_update_camera(man, level);
}
