# Compilateur à utiliser
CC = gcc

# Options de compilation
CFLAGS = -Wall -Wextra -std=c11 -IC:/SDL2/SDL2-devel-2.30.10-VC/SDL2-2.30.10/include

# Options de liaison
LDFLAGS = -LC:/SDL2/SDL2-devel-2.30.10-VC/SDL2-2.30.10/lib/x64 -lmingw32 -lSDL2main -lSDL2

# Nom de l'exécutable
TARGET = main

# Dossier de compilation
BUILD_DIR = build

# Fichiers sources
SRCS = main.c

# Fichiers objets générés à partir des sources
OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)

# Règle par défaut
all:  $(BUILD_DIR) $(BUILD_DIR)/$(TARGET)

# Créer le dossier de compilation s'il n'existe pas
$(BUILD_DIR):
	mkdir $(BUILD_DIR)

# Règle pour créer l'exécutable
$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

# Règle pour créer les fichiers objets
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Regle pour executer le programme
run: $(BUILD_DIR)/$(TARGET)
	./$(BUILD_DIR)/$(TARGET).exe

# Règle pour nettoyer les fichiers générés
clean:
	del /Q $(BUILD_DIR)\*.o $(BUILD_DIR)\$(TARGET).exe

# Règle pour compiler et exécuter le programme en une seule commande + clean
build_and_run: clean all run

# Indiquer que les cibles 'all', 'run', 'clean' et 'build_and_run' ne sont pas des fichiers
.PHONY: all run clean build_and_run

# Exemple d'utilisation de makefile
# make all
# make run
# make clean


# Importation de la librairie SDL2
# Dans CFLAGS -> add -> -IC:/SDL2/SDL2-devel-2.30.10-VC/SDL2-2.30.10/include
# LDFLAGS = -LC:/SDL2/SDL2-devel-2.30.10-VC/SDL2-2.30.10/lib/x64 -lmingw32 -lSDL2main -lSDL2
#
# Le packet SDL2-devel-2.30.10-VC.zip doit être téléchargé sur le site officiel de la SDL2 et placé dans le répertoire C:/