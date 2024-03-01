#include "../inc/level.h"
#include "../inc/header.h"
#include "../inc/menu.h"
#include "../inc/puzzle.h"


int mx_load_collision(SDL_Renderer *renderer, t_level *level, int level_number) {
    // Загрузка изображения для карты коллизий
    char *level_collision_url = (char *)malloc(120 * sizeof(char));
    if (level_number == -1) {
        sprintf(level_collision_url, "resource/resources/levels/level-main/collision.png");
    } else {
        sprintf(level_collision_url, "resource/resources/levels/level-%d/collision.png", level_number);
    }


    SDL_Surface *originalCollisionSurface = IMG_Load(level_collision_url);
    free(level_collision_url);
    if (!originalCollisionSurface) {
        fprintf(stderr, "Ошибка загрузки изображения коллизий: %s\n", IMG_GetError());
        // Обработка ошибки, например, возврат из функции или еще что-то
        return EXIT_FAILURE;
    }

    // Создание SDL_Surface с размерами уровня
    level->mapCollisionSurface = SDL_CreateRGBSurface(0, level->width, level->height,
                                                      originalCollisionSurface->format->BitsPerPixel,
                                                      0, 0, 0, 0);
    if (!level->mapCollisionSurface) {
        fprintf(stderr, "Ошибка создания поверхности карты коллизий: %s\n", SDL_GetError());
        SDL_FreeSurface(originalCollisionSurface);
        // Обработка ошибки, например, возврат из функции или еще что-то
        return EXIT_FAILURE;
    }

    // Изменение размера поверхности карты коллизий с сохранением пропорций
    SDL_Rect stretchRect = {0, 0, level->width, level->height};
    SDL_BlitScaled(originalCollisionSurface, NULL, level->mapCollisionSurface, &stretchRect);

    SDL_FreeSurface(originalCollisionSurface);

    // Создание текстуры из поверхности карты коллизий
    level->collisionTexture = SDL_CreateTextureFromSurface(renderer, level->mapCollisionSurface);
    if (!level->collisionTexture) {
        fprintf(stderr, "Ошибка создания текстуры коллизий: %s\n", SDL_GetError());
        SDL_FreeSurface(level->mapCollisionSurface);
        // Обработка ошибки, например, возврат из функции или еще что-то
        return EXIT_FAILURE;
    }

    return 0;
}

int level_loop(SDL_Renderer *renderer, t_level *level, t_man *man) {
    Uint32 currentTime = SDL_GetTicks();

    level->is_paused = mx_input_handler(man, level);
    man->texture = mx_load_texture(man->currentURL, renderer);

    // Анимация персонажа
    if (currentTime >= man->frameDuration + (unsigned int)man->lastUpdateTime) {
        man->currentFrame = (man->currentFrame + 1) % man->numberOfFrames;
        man->lastUpdateTime = currentTime;
    }

    mx_update_camera(man, level);
    mx_do_render(renderer, man, level, mx_render_man);
    if (DEV_MODE) {
        const SDL_Color redColor = {255, 0, 0, 255};
        SDL_SetRenderDrawColor(renderer, redColor.r, redColor.g, redColor.b, redColor.a);

        for (int i = 0; i < level->puzzle_number; i++) {

            SDL_Rect rect = level->puzzles[i];
            rect.x -= level->cameraX;
            rect.y -= level->cameraY;
            SDL_RenderFillRect(renderer, &rect);
        }
        SDL_RenderPresent(renderer);
    }
    // 60fps
    SDL_Delay(16);
    return level->is_paused;
}

void init_puzzles(t_number_button *number_buttons,int number_of_buttons) {
    int button_size = 60;
    int gap_between_buttons = 20;
    int button_block_width = button_size * number_of_buttons + gap_between_buttons * (number_of_buttons - 1);

    for (int i = 0; i < number_of_buttons; ++i) {
        number_buttons[i] = (t_number_button){(WINDOW_WIDTH/2 - button_block_width / 2) + (button_size + gap_between_buttons) * i,
                                              WINDOW_HEIGHT / 2 - button_size / 2, button_size, button_size, 0, NULL};
    }
}

int mx_count_bits(int number) {
    // Если число отрицательное, считаем его абсолютное значение
    if (number < 0) {
        number = abs(number);
    }

    // Логарифм по основанию 2 для определения количества битов
    int bitCount = 0;
    while (number != 0) {
        bitCount++;
        number >>= 1;
    }

    return bitCount;
}

int init_guess_number(int range, int min) {
    int guess_number = rand() % (range-min) + 1 + min;
    return guess_number;
}

void init_shuffle_puzzle(t_number_button *shuffle_buttons) {
    int button_size = 60;
    int gap_between_buttons = 20;
    int number_of_buttons = 16;
    int button_block_width = button_size * number_of_buttons + gap_between_buttons * (number_of_buttons - 1);
    for (int i = 0; i < number_of_buttons; ++i) {
        if (i >= 0 && i <= 3) {
            shuffle_buttons[i] = (t_number_button){(WINDOW_WIDTH/2 - button_block_width / 2) + (button_size + gap_between_buttons) * (i + 6),
                                                   WINDOW_HEIGHT / 2 - button_size / 2 - (button_size - gap_between_buttons), button_size, button_size, 0, NULL};
        } else if (i >= 4 & i <= 7) {
            shuffle_buttons[i] = (t_number_button){(WINDOW_WIDTH/2 - button_block_width / 2) + (button_size + gap_between_buttons) * (i + 2),
                                                   WINDOW_HEIGHT / 2 - button_size / 2  + (button_size - gap_between_buttons) * 1, button_size, button_size, 0, NULL};
        }  else if (i >= 8 & i <= 11) {
            shuffle_buttons[i] = (t_number_button){(WINDOW_WIDTH/2 - button_block_width / 2) + (button_size + gap_between_buttons) * (i - 2),
                                                   WINDOW_HEIGHT / 2 - button_size / 2 + (button_size + gap_between_buttons) * 1.5, button_size, button_size, 0, NULL};
        } else if (i >= 12 & i <= 15) {
            shuffle_buttons[i] = (t_number_button){(WINDOW_WIDTH/2 - button_block_width / 2) + (button_size + gap_between_buttons) * (i - 6),
                                                   WINDOW_HEIGHT / 2 - button_size / 2 + (button_size + gap_between_buttons) * 2.5, button_size, button_size, 0, NULL};
        }

    }
}

void mx_level_gameplay(int number_of_level, SDL_Renderer *renderer, SDL_Window *window, t_level *level, t_man *man, int is_paused) {
    if (level->number != -1) {
        level->puzzle_number = 0;
    }

    if (level->puzzle_number == 0) {
        level->puzzles = NULL;
    }
    int is_pause = is_paused;
    int guess_number = init_guess_number(65536, 1048);
    int new_gess_number = 0;
    t_menu pause;
    mx_init_menu_buttons(&pause, 0);

    // Load font
    TTF_Font *font = TTF_OpenFont("resource/resources/fonts/EnterCommand.ttf", 24);

    if (font == NULL) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        level->number = EXIT;
    }

    int number_of_buttons = mx_count_bits(guess_number);

    // Динамическое выделение памяти для массива t_number_button
    t_number_button *number_buttons = (t_number_button *)malloc(number_of_buttons * sizeof(t_number_button));
    t_number_button *shuffle_buttons = (t_number_button *)malloc(16 * sizeof(t_number_button));

    // Проверка успешного выделения памяти
    if (number_buttons == NULL || shuffle_buttons == NULL) {
        printf("Failed to allocate memory for number_buttons\n");
        exit(EXIT_FAILURE);
    }

    init_puzzles(number_buttons, number_of_buttons);

    init_shuffle_puzzle(shuffle_buttons);
    t_shuffle shuffle; // Создание экземпляра структуры t_shuffle

    mx_init_grid(&shuffle, shuffle_buttons); // Передача указателя на структуру shuffle
    shuffle_buttons[15].value = 0;

    shuffle.empty_col = 3;
    shuffle.empty_row = 3;
    shuffle.is_call = 0;
    shuffle.is_changed = 0;

    t_sudoku sudoku;
    sudoku.is_call = 0;
    sudoku.is_changed = 0;


    while (level->number == number_of_level) {
        for (int i = 0; i < level->puzzle_number; ++i) {
            if (level->obstacles_number >= level->puzzle_number && !level->obstacles[i].is_open) {
                level->obstacles[i].is_open = 1;
            }
        }
        if (level->number != -1) {
            if (level->number == 5) {
                if (level->puzzle_number == level->obstacles_number) {
                    level->number = 6;
                    break;
                }
            } else {
                if (level->puzzle_number == level->obstacles_number) {
                    level->number = -1;
                    break;
                }
            }
        }

        switch (is_pause) {
            case 0:
                is_pause = level_loop(renderer, level, man);
                break;
            case 1:
                is_pause = mx_pause_call(renderer, window, level, man, &pause, font);
                break;
            case 2:
                if (new_gess_number) {
                    guess_number = init_guess_number(65536, 1048);

                    // Пересчитать число битов перед удалением и созданием массива number_buttons
                    number_of_buttons = mx_count_bits(guess_number);

                    // Освободить память от старого массива
                    free(number_buttons);

                    // Выделить новую память для массива number_buttons
                    number_buttons = (t_number_button *)malloc(number_of_buttons * sizeof(t_number_button));

                    init_puzzles(number_buttons, number_of_buttons);

                    new_gess_number = 0;
                }
                is_pause = mx_binary_password(renderer, window, font, level, man, number_buttons, number_of_buttons, guess_number, &new_gess_number);
                break;
            case 3:
                if (!shuffle.is_call) {
                    shuffle.is_call = 1;
                    mx_shuffle_tiles(&shuffle, shuffle_buttons,1000);
                }
                is_pause = mx_shuffle(renderer, window, font, level, man, &shuffle, shuffle_buttons);
                break;
            case 4:
                if (!sudoku.is_call) {
                    mx_init_sudoku(&sudoku);
                }
                is_pause = mx_sudoku(renderer, window, font, level, man, &sudoku);
                break;
        }
    }

    // Освобождение выделенной памяти
    free(number_buttons);
    mx_destroy_buttons(&pause);
}

void mx_read_doors(const char *filename, t_level *level) {
    FILE *file = fopen(filename, "r"); // Открытие файла для чтения
    if (file == NULL) {
        exit(EXIT_FAILURE);
    }

    // Считываем количество дверей
    if (fscanf(file, "%d", &level->obstacles_number) != 1) {
        fprintf(stderr, "Ошибка: Не удалось прочитать количество дверей из файла\n");
        exit(EXIT_FAILURE);
    }

    // Выделяем память под массив препятствий
    level->obstacles = (t_obstacle *)malloc(level->obstacles_number * sizeof(t_obstacle));
    if (level->obstacles == NULL) {
        fprintf(stderr, "Ошибка: Не удалось выделить память для obstacles\n");
        exit(EXIT_FAILURE);
    }

    // Считываем информацию о препятствиях
    for (int i = 0; i < level->obstacles_number; ++i) {
        if (fscanf(file, "%d %d %d %d", &level->obstacles[i].coords.x,
                   &level->obstacles[i].coords.y,
                   &level->obstacles[i].coords.w,
                   &level->obstacles[i].coords.h) != 4) {
            exit(EXIT_FAILURE);
        }
        // Устанавливаем значение по умолчанию для is_open
        level->obstacles[i].is_open = 0;
    }

    fclose(file); // Закрываем файл
}
