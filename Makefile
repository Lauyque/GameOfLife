# Compilateur à utiliser
CC = gcc

# Dossier des dépendances
DEP_DIR = dependancy
SDL2_DIR = $(DEP_DIR)\SDL2\SDL2-2.30.10
SDL2_TTF_DIR = $(DEP_DIR)\SDL2_TTF\SDL2_ttf-2.22.0
SDL2_IMAGES_DIR = $(DEP_DIR)\SDL2_IMAGES\SDL2_image-2.8.4

# Options de compilation
CFLAGS = -Wall -Wextra -std=c11 -I$(SDL2_DIR)\include -I$(SDL2_TTF_DIR)\include -I$(SDL2_IMAGES_DIR)\include

# Options de liaison
LDFLAGS = -L$(SDL2_DIR)\lib\x64 -L$(SDL2_TTF_DIR)\lib\x64 -L$(SDL2_IMAGES_DIR)\lib\x64 -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image
#LDFLAGS = -lmingw32 -lSDL2main -lSDL2

# Nom de l'exécutable
TARGET = main

# Dossier de compilation
BUILD_DIR = build

# Fichiers sources
SRCS = main.c

# Fichiers objets générés à partir des sources
OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)

# Règle par défaut
all: $(BUILD_DIR) $(BUILD_DIR)/$(TARGET) copy_dll

# Créer le dossier de compilation s'il n'existe pas
$(BUILD_DIR):
	mkdir $(BUILD_DIR)

# Règle pour créer l'exécutable
$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

# Règle pour créer les fichiers objets
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Règle pour copier SDL2.dll dans le répertoire de compilation
copy_dll:
	copy $(SDL2_DIR)\lib\x64\SDL2.dll $(BUILD_DIR)
	copy $(SDL2_TTF_DIR)\lib\x64\SDL2_ttf.dll $(BUILD_DIR)
	copy $(SDL2_IMAGES_DIR)\lib\x64\SDL2_image.dll $(BUILD_DIR)

# Règle pour exécuter le programme
run: $(BUILD_DIR)/$(TARGET)
	.\$(BUILD_DIR)\$(TARGET)

# Règle pour nettoyer les fichiers générés
clean:
	del /Q $(BUILD_DIR)\*.o $(BUILD_DIR)\$(TARGET).exe $(BUILD_DIR)\SDL2.dll

# Règle pour compiler et exécuter le programme
build_and_run: clean copy_dll all run

# Indiquer que les cibles 'all', 'run', 'clean', 'build_and_run' et 'copy_dll' ne sont pas des fichiers
.PHONY: all run clean build_and_run copy_dll