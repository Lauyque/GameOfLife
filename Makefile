# Compilateur à utiliser
CC = gcc

# Dossier avec SDL2
SDL2_DIR = C:\SDL2\SDL2-devel-2.30.10-VC\SDL2-2.30.10

# Options de compilation
CFLAGS = -Wall -Wextra -std=c11 -I$(SDL2_DIR)\include

# Options de liaison
LDFLAGS = -L$(SDL2_DIR)\lib\x64 -lmingw32 -lSDL2main -lSDL2

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

# Règle pour exécuter le programme
run: $(BUILD_DIR)/$(TARGET)
	.\$(BUILD_DIR)\$(TARGET)

# Règle pour nettoyer les fichiers générés
clean:
	del /Q $(BUILD_DIR)\*.o $(BUILD_DIR)\$(TARGET).exe $(BUILD_DIR)\SDL2.dll

# Règle pour compiler et exécuter le programme
build_and_run: clean all run

# Indiquer que les cibles 'all', 'run', 'clean', 'build_and_run' et 'copy_dll' ne sont pas des fichiers
.PHONY: all run clean build_and_run copy_dll