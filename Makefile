NAME = endgame

SRC_DIR = src

OBJ_DIR = obj

# папка с хедерами
INC_DIR = inc

SDL = -F resource/framework -I resource/framework/SDL2.framework/SDL2 -I resource/framework/SDL2_image.framework/SDL2_image \
	-I resource/framework/SDL2_mixer.framework/SDL2_mixer -I resource/framework/SDL2_ttf.framework/SDL2_ttf

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

OBJ_FILES = $(addprefix $(OBJ_DIR)/, $(notdir $(SRC_FILES:%.c=%.o)))

INC_FILES = $(wildcard $(INC_DIR)/*.h)

# компилятор для создания o-файлов
CC = clang

CFLAGS = -std=c11 $(addprefix -W, all extra error pedantic) -g \

SDL_FLAGS = -rpath resource/framework -framework SDL2 \
		-framework SDL2_image \
		-I resource/framework/SDL2_image.framework/Headers \
		-framework SDL2_mixer \
		-I resource/framework/SDL2_mixer.framework/Headers \
		-framework SDL2_ttf \
		-I resource/framework/SDL2_ttf.framework/Headers \

# архиватор для формирования библиотеки из о-файлов
AR = ar

#флаги для архиватора
AFLAGS = rcs

MKDIR = mkdir -p
RM = rm -rf

# главная цель, при ее вызове вызывается цель libmx.a
all: $(NAME)

$(NAME): $(OBJ_FILES)
	@$(CC) $(CFLAGS) $^ -o $@ -I $(INC_DIR) $(SDL_FLAGS) $(SDL)
	@printf "\r\33[2K$@\t \033[32;1mcreated\033[0m\n"

$(OBJ_FILES): | $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC_FILES)
	@$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_DIR) $(SDL)
	@printf "\r\33[2K$(NAME)\033[33;1m\t compile \033[0m$(<:$(SRC_DIR)/%.c=%)\r"

# создается папка obj
$(OBJ_DIR):
	@$(MKDIR) $@

# удаляем папку с о-файлами
clean:
	@$(RM) $(OBJ_DIR)
	@printf "$(OBJ_DIR) in $(NAME)\t \033[31;1mdeleted\033[0m\n"

# полностью удаляем результат работы мейкфайла
uninstall:
	@$(RM) $(OBJ_DIR)
	@$(RM) $(NAME)
	@@printf "$(NAME)\t \033[31;1muninstalled\033[0m\n"

reinstall: uninstall all

.PHONY: all uninstall clean reinstall