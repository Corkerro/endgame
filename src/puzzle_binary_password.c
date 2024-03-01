#include "../inc/puzzle.h"
#include "../inc/header.h"
#include <string.h>
#include <stdlib.h>

char *decimal_to_binary(int number, int numberOfBits) {
    char *binary_number = (char *)malloc((numberOfBits + 1) * sizeof(char));

    if (binary_number == NULL) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        exit(EXIT_FAILURE);
    }

    // Заполняем массив символов представлением числа в двоичной системе
    for (int i = numberOfBits - 1; i >= 0; i--) {
        binary_number[numberOfBits - 1 - i] = '0' + ((number >> i) & 1);
    }

    binary_number[numberOfBits] = '\0'; // Нуль-терминатор

    return binary_number;
}

int mx_binary_password(SDL_Renderer *renderer, SDL_Window *window, TTF_Font *font, t_level *level, t_man *man,
                       t_number_button *number_buttons, int number_of_buttons, int original_password, int *new_gess_number) {
    int offset = 0;
    int button_size = 60;
    int gap_between_buttons = 20;
    int button_block_width = button_size * number_of_buttons + gap_between_buttons * (number_of_buttons - 1);
    int cur_level = 2;

    char *original_text = (char *)malloc(2 * sizeof(int));
    sprintf(original_text, "%d", original_password);
    mx_render_number_and_buttons(renderer, font, original_text, number_buttons, number_of_buttons);
    free(original_text);
    cur_level = mx_puzzle_binary_input_handler(window, number_buttons, number_of_buttons, button_block_width);

    char *button_text = malloc((number_of_buttons + 1) * sizeof(char));
    offset = 0;
    for (int i = 0; i < number_of_buttons; i++) {
        offset += sprintf(button_text + offset, "%d", number_buttons[i].value);
    }

    // Если пароль совпал
    char *decimal_to_binary_char = decimal_to_binary(original_password, number_of_buttons);
    if (strcmp(button_text, decimal_to_binary_char) == 0) {
        free(decimal_to_binary_char);
        mx_print_win_message(renderer, font, level, man, number_buttons, number_of_buttons, &cur_level);
    }

    // 30fps
    SDL_Delay(32);

    // Освобождение памяти и уничтожение текстур
    free(button_text);
    for (int i = 0; i < number_of_buttons; i++) {
        SDL_DestroyTexture(number_buttons[i].texture);
    }

    if (cur_level == 0) {
        (*new_gess_number)++;
    }

    return cur_level;
}
