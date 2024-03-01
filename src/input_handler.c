#include "../inc/header.h"
#include "../inc/puzzle.h"

int check_collision(t_man *man, t_level *level) {
    // Т.к у нас у спрайта человек не всю ширину занимает, а на 10 пикселей меньше учитываем это тут с помощью + 5
    int manLeft = man->position.x + 1 * man->scale;

    // И тут с помощью -5, но т.к мы уже учли +5, вычитаем два раза, т.е - 5 * 2
    int manRight = manLeft - 1 * man->scale * 2 + man->position.w * man->scale;

    int manTop = man->position.y;
    int manBottom = manTop + man->position.h * man->scale;

    // Проверяем коллизии с препятствиями
    for (int i = 0; i < level->obstacles_number; i++) {
        if (!level->obstacles[i].is_open) {
            int obstacleLeft = level->obstacles[i].coords.x;
            int obstacleRight = level->obstacles[i].coords.x + level->obstacles[i].coords.w;
            int obstacleTop = level->obstacles[i].coords.y;
            int obstacleBottom = level->obstacles[i].coords.y + level->obstacles[i].coords.h;

            // Проверяем коллизии по осям X и Y
            if (manRight >= obstacleLeft && manLeft <= obstacleRight && manBottom >= obstacleTop && manTop <= obstacleBottom) {
                // Обнаружена коллизия
                return !level->obstacles[i].is_open;
            }
        }
    }

    // Проверка столкновения по левой границе
    for (int y = manTop; y < manBottom; y++) {
        if (mx_check_color(level->mapCollisionSurface, manLeft, y, 0, 0, 1)) {
            return 1;
        }
    }

    // Проверка столкновения по верхней границе
    for (int x = manLeft; x < manRight; x++) {
        if (mx_check_color(level->mapCollisionSurface, x, manTop, 0, 0, 1)) {
            return 1;
        }

        if (mx_check_color(level->mapCollisionSurface, x, manTop, 255, 140, 0)) {
            if (level->puzzles == NULL)
                return 2;
            int no_match = 0;
            for (int i = 0; i < level->puzzle_number; ++i) {
                // Проверка, что человек не пересекается с пазлом
                if ((man->position.x + man->position.w < level->puzzles[i].x ||
                     man->position.x > level->puzzles[i].x + level->puzzles[i].w ||
                     man->position.y + man->position.h < level->puzzles[i].y ||
                     man->position.y > level->puzzles[i].y + level->puzzles[i].h)) {
                    no_match++;
                }
                if (no_match == level->puzzle_number)
                    return 2; // Столкновение с пазлом
            }
        }

        if (mx_check_color(level->mapCollisionSurface, x, manTop - man->speed, 255, 140, 0)) {
            if (level->puzzles == NULL)
                return 3;

            int no_match = 0;
            for (int i = 0; i < level->puzzle_number; ++i) {
                if ((man->position.x + man->position.w < level->puzzles[i].x ||
                     man->position.x > level->puzzles[i].x + level->puzzles[i].w ||
                     man->position.y + man->position.h < level->puzzles[i].y ||
                     man->position.y > level->puzzles[i].y + level->puzzles[i].h)) {
                    no_match++;
                }
                if (no_match == level->puzzle_number)
                    return 3;
            }
        }
    }

    // Проверка столкновения по правой границе
    for (int y = manTop; y < manBottom; y++) {
        if (mx_check_color(level->mapCollisionSurface, manRight, y, 0, 0, 1)) {
            return 1;
        }
    }

    // Проверка столкновения по нижней границе
    for (int x = manLeft; x < manRight; x++) {
        if (mx_check_color(level->mapCollisionSurface, x, manBottom, 0, 0, 1)) {
            return 1;
        }
        if (mx_check_color(level->mapCollisionSurface, x, manBottom, 255, 140, 0)) {
            if (level->puzzles == NULL)
                return 2;
            int no_match = 0;
            for (int i = 0; i < level->puzzle_number; ++i) {
                // Проверка, что человек не пересекается с пазлом
                if ((man->position.x + man->position.w < level->puzzles[i].x ||
                     man->position.x > level->puzzles[i].x + level->puzzles[i].w ||
                     man->position.y + man->position.h < level->puzzles[i].y ||
                     man->position.y > level->puzzles[i].y + level->puzzles[i].h)) {
                    no_match++;
                }
                if (no_match == level->puzzle_number)
                    return 2; // Столкновение с пазлом
            }
        }

        if (mx_check_color(level->mapCollisionSurface, x, manBottom + man->speed, 255, 140, 0)) {
            if (level->puzzles == NULL)
                return 3;

            int no_match = 0;
            for (int i = 0; i < level->puzzle_number; ++i) {
                if ((man->position.x + man->position.w < level->puzzles[i].x ||
                     man->position.x > level->puzzles[i].x + level->puzzles[i].w ||
                     man->position.y + man->position.h < level->puzzles[i].y ||
                     man->position.y > level->puzzles[i].y + level->puzzles[i].h)) {
                    no_match++;
                }
                if (no_match == level->puzzle_number)
                    return 3;
            }
        }
    }

    // Если не произошло столкновения ни с одной из границ, возвращаем 0
    return 0;
}

int mx_input_handler(t_man *man, t_level *level) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN: {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        return 1;
                        break;
                    case SDLK_r:
                        level->number = !level->number ? -1 : 0;
                        break;
                }
            } break;
            case SDL_MOUSEBUTTONDOWN: {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

            } break;
            case SDL_QUIT:
                level->number = EXIT;
        }
    }

    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W]) {
        man->position.y -= man->speed;
        man->currentURL = man->topURL;
        int collision_result = check_collision(man, level);
        if (collision_result != 0 && collision_result != 3) {
            man->position.y = man->lastY;
            man->position.x += man->direction;
            collision_result = check_collision(man, level);
            if (collision_result != 0 && collision_result != 3) {
                man->direction *= -1;
                man->position.x = man->lastX;
                man->position.x -= 1;
                collision_result = check_collision(man, level);
                if (collision_result != 0 && collision_result != 3) {
                    man->position.x = man->lastX;
                }
            }
        } else {
            man->direction = -1;
        }
    }
    if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S]) {
        man->position.y += man->speed;
        man->currentURL = man->downURL;
        int collision_result = check_collision(man, level);
        if (collision_result != 0 && collision_result != 3) {
            man->position.y = man->lastY;
            man->position.x += man->direction;
            collision_result = check_collision(man, level);
            if (collision_result != 0 && collision_result != 3) {
                man->direction *= -1;
                man->position.x = man->lastX;
                man->position.x -= 1;
                collision_result = check_collision(man, level);
                if (collision_result != 0 && collision_result != 3) {
                    man->position.x = man->lastX;
                }
            }
        } else {
            man->direction = 1;
        }
    }
    if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A]) {
        man->position.x -= man->speed;
        man->currentURL = man->leftURL;
        int collision_result = check_collision(man, level);
        if (collision_result != 0 && collision_result != 3) {
            man->position.x = man->lastX;
            man->position.y += man->direction;
            collision_result = check_collision(man, level);
            if (collision_result != 0 && collision_result != 3) {
                man->direction *= -1;
                man->position.y = man->lastY;
                man->position.y -= 1;
                collision_result = check_collision(man, level);
                if (collision_result != 0 && collision_result != 3) {
                    man->position.y = man->lastY;
                }
            }
        } else {
            man->direction = -1;
        }
    }
    if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D]) {
        man->position.x += man->speed;
        man->currentURL = man->rightURL;
        int collision_result = check_collision(man, level);
        if (collision_result != 0 && collision_result != 3) {
            man->position.x = man->lastX;
            man->position.y += man->direction;
            collision_result = check_collision(man, level);
            if (collision_result != 0 && collision_result != 3) {
                man->direction *= -1;
                man->position.y = man->lastY;
                man->position.y -= 1;
                collision_result = check_collision(man, level);
                if (collision_result != 0 && collision_result != 3) {
                    man->position.y = man->lastY;
                }
            }
        } else {
            man->direction = 1;
        }
    }

    if (level->number != -1) {
        if (state[SDL_SCANCODE_E]) {
            // Создание новой структуры t_man в динамической памяти
            t_man *new_man = (t_man *)malloc(sizeof(t_man));
            if (new_man == NULL) {
                // Обработка ошибки, если не удалось выделить память
                printf("Memory allocation failed\n");
                return EXIT;
            } else {
                *new_man = *man;
                new_man->position.y = man->position.y - man->speed;
                new_man->position.h = man->position.h + man->speed;
                int collision_result = check_collision(new_man, level);
                // Проверка на столкновение с препятствием
                if (collision_result == 3) {
                    man->position.x = man->lastX;
                    man->position.y = man->lastY;
                    return rand() % PUZZLE_NUMBER + 2;
                }

                // Освобождение памяти после использования новой структуры
                free(new_man);
            }
        }

        if (check_collision(man, level) == 3) {
            level->press_e_time = SDL_GetTicks();
            level->is_press_e_inited = 1;
            if (state[SDL_SCANCODE_E]) {
                return rand() % PUZZLE_NUMBER + 2;
            }
        }
    } else {
        if (state[SDL_SCANCODE_E]) {
            // Создание новой структуры t_man в динамической памяти
            t_man *new_man = (t_man *)malloc(sizeof(t_man));
            if (new_man == NULL) {
                // Обработка ошибки, если не удалось выделить память
                printf("Memory allocation failed\n");
                return EXIT;
            } else {
                *new_man = *man;
                new_man->position.y = man->position.y - man->speed;
                new_man->position.h = man->position.h + man->speed;
                int collision_result = check_collision(new_man, level);
                // Проверка на столкновение с препятствием
                if (collision_result == 3) {
                    man->position.x = man->lastX;
                    man->position.y = man->lastY;
                    mx_write_puzzles(level, man);
                    mx_save(level->number, man->lastX, man->lastY);
                    level->number = level->puzzle_number;
                }

                // Освобождение памяти после использования новой структуры
                free(new_man);
            }
        }

        if (check_collision(man, level) == 3) {
            level->press_e_time = SDL_GetTicks();
            level->is_press_e_inited = 1;
            if (state[SDL_SCANCODE_E]) {
                mx_write_puzzles(level, man);
                mx_save(level->number, man->lastX, man->lastY);
                level->number = level->puzzle_number;
            }
        }
    }

    if (man->position.x < level->cameraX)
        man->position.x = level->cameraX;
    if (man->position.x > level->cameraX + WINDOW_WIDTH - man->position.w * man->scale)
        man->position.x = level->cameraX + WINDOW_WIDTH - man->position.w * man->scale;

    if (man->position.y < level->cameraY)
        man->position.y = level->cameraY;
    if (man->position.y > level->cameraY + WINDOW_HEIGHT - man->position.h * man->scale)
        man->position.y = level->cameraY + WINDOW_HEIGHT - man->position.h * man->scale;

    man->lastX = man->position.x;
    man->lastY = man->position.y;


    return 0;
}

int mx_menu_input_handler(SDL_Window *window, t_menu *menu, t_game_start *game) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_WINDOWEVENT_CLOSE: {
                if (window) {
                    SDL_DestroyWindow(window);
                }
            } break;
            case SDL_KEYDOWN: {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        // Переключение между игровым режимом и режимом меню
                        return game->old_level;
                }
            } break;
            case SDL_MOUSEBUTTONDOWN: {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                if (mouseX >= menu->startButton.x && mouseX <= menu->startButton.x + menu->startButton.width) {
                    if (mouseY >= menu->continueButton.y && mouseY <= menu->continueButton.y + menu->continueButton.height) {
                        return game->old_level;
                    }
                    else if (mouseY >= menu->startButton.y && mouseY <= menu->startButton.y + menu->startButton.height) {
                        return -1;
                    } else if (mouseY >= menu->exitButton.y && mouseY <= menu->exitButton.y + menu->exitButton.height) {
                        return EXIT;
                    }
                    else if (mouseY >= menu->bgButton.y && mouseY <= menu->bgButton.y + menu->bgButton.height) {
                        menu->bgButton.isOff = !menu->bgButton.isOff;
                        if (!menu->bgButton.isOff)
                            mx_set_music_volume(20);
                        else mx_set_music_volume(0);
                    }
                }
            } break;
            case SDL_QUIT:
                break;
        }
    }

    return MENU;
}

int mx_pause_input_handler(SDL_Window *window, t_menu *menu, t_level *level, t_man *man) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_WINDOWEVENT_CLOSE: {
                if (window) {
                    mx_save(level->number, man->lastX, man->lastY);
                    SDL_DestroyWindow(window);
                }
            } break;
            case SDL_KEYDOWN: {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        // Переключение между игровым режимом и режимом меню
                        return 0;
                }
            } break;
            case SDL_MOUSEBUTTONDOWN: {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                if (mouseX >= menu->startButton.x && mouseX <= menu->startButton.x + menu->startButton.width) {
                    if (mouseY >= menu->continueButton.y && mouseY <= menu->continueButton.y + menu->continueButton.height) {
                        return 0;
                    }
                    else if (mouseY >= menu->startButton.y && mouseY <= menu->startButton.y + menu->startButton.height) {
                        remove(SAVE_FILE);
                        level->number = MENU;
                        break;
                    } else if (mouseY >= menu->exitButton.y && mouseY <= menu->exitButton.y + menu->exitButton.height) {
                        mx_save(level->number, man->lastX, man->lastY);
                        level->number = EXIT;
                        break;
                    }
                    else if (mouseY >= menu->bgButton.y && mouseY <= menu->bgButton.y + menu->bgButton.height) {
                        menu->bgButton.isOff = !menu->bgButton.isOff;
                        if (!menu->bgButton.isOff)
                            mx_set_music_volume(20);
                        else mx_set_music_volume(0);
                    }
                }
            } break;
            case SDL_QUIT:
                mx_save(level->number, man->lastX, man->lastY);
                level->number = EXIT;
                break;
        }
    }

    return 1;
}
