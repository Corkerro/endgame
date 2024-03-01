#include "../inc/header.h"
#include "../inc/level.h"
#include "../inc/menu.h"

int mx_man_read(t_man *man) {
    FILE *file = fopen(SAVE_FILE, "r");

    int x = 0;
    int y = 0;

    fscanf(file, "%d", &x);
    fscanf(file, "%d %d", &x, &y);
    fclose(file);

    if (x != 0 && y != 0) {
        man->position.x = x;
        man->position.y = y;
        return 1;
    }
    return 0;
}

void mx_read_puzzles(t_level *level) {
    FILE *file = fopen(SOLVED_FILE, "r"); // Открытие файла для чтения
    if (file == NULL) {
        exit(EXIT_FAILURE);
    }

    // Считываем количество дверей
    if (fscanf(file, "%d", &level->puzzle_number) != 1) {
        fprintf(stderr, "Ошибка: Не удалось прочитать количество дверей из файла\n");
        exit(EXIT_FAILURE);
    }

    if (level->puzzle_number == 0) {
        return;
    }

    // Выделяем память под массив препятствий
    level->puzzles = (SDL_Rect *)malloc(level->puzzle_number * sizeof(SDL_Rect));
    if (level->puzzles == NULL) {
        fprintf(stderr, "Ошибка: Не удалось выделить память для obstacles\n");
        exit(EXIT_FAILURE);
    }

    // Считываем информацию о препятствиях
    for (int i = 0; i < level->puzzle_number; ++i) {
        if (fscanf(file, "%d %d %d %d", &level->puzzles[i].x,
                   &level->puzzles[i].y,
                   &level->puzzles[i].w,
                   &level->puzzles[i].h) != 4) {
            exit(EXIT_FAILURE);
        }
    }

    fclose(file); // Закрываем файл
}

void mx_write_puzzles(t_level *level, t_man *man) {
    int level_new_w = 96 * 2 + 20;
    int level_new_h = (48 + man->position.h) * 2 + 30;
    int level_new_x = man->position.x - level_new_w / 2 - 10;
    int level_new_y = man->position.y - level_new_h / 2.5 - 15;
    FILE *file = fopen(SOLVED_FILE, "w"); // Открытие файла для чтения
    fprintf(file, "%d", level->puzzle_number + 1);
    for (int i = 0; i < level->puzzle_number; ++i) {
        fprintf(file, "\n%d %d %d %d", level->puzzles[i].x,
                level->puzzles[i].y,
                level->puzzles[i].w,
                level->puzzles[i].h);
    }
    fprintf(file, "\n%d %d %d %d", level_new_x,
            level_new_y,
            level_new_w,
            level_new_h);
    fclose(file);
}


int mx_level_main_call(SDL_Renderer *renderer, SDL_Window *window, int level_number, int is_paused) {
    t_level level;
    t_man man;
    level.width = 4961;
    level.height = 1291;
    level.cameraX = (level.width - WINDOW_WIDTH) / 2;
    level.cameraY = (level.height - WINDOW_HEIGHT) / 2;
    level.number = -1;
    level.mapTexture = mx_load_texture("resource/resources/levels/level-main/bg.png", renderer);
    level.doorTexture = mx_load_texture("resource/resources/door.png", renderer);
    mx_read_doors("resource/resources/levels/level-main/level.info", &level);

    // Загрузка коллизии
    if (mx_load_collision(renderer, &level, level_number))
        return EXIT;

    if (mx_man_read(&man)) {
        mx_init_man(&man, man.position.x, man.position.y, &level, 1);
    } else {
        mx_init_man(&man, 700, 780, &level, 1);
    }


    if (!mx_save(level.number, man.position.x, man.position.y))
        return EXIT;

    mx_read_puzzles(&level);


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
    Mix_FreeMusic(backgroundMusic);
    return level.number;
}
