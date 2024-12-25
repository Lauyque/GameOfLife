# Compilateur à utiliser
CC = gcc

# Options de compilation
CFLAGS = -Wall -Wextra -std=c11

# Nom de l'exécutable
TARGET = main

# Fichiers sources
SRCS = main.c

# Fichiers objets générés à partir des sources
OBJS = $(SRCS:.c=.o)

# Règle par défaut
all: $(TARGET)

# Règle pour créer l'exécutable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Règle pour créer les fichiers objets
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regle pour executer le programme
run: $(TARGET)
	./$(TARGET)

# Règle pour nettoyer les fichiers générés
clean:
	rm -f $(OBJS) $(TARGET)

# Règle pour compiler et exécuter le programme
build_and_run: all run

# Indiquer que les cibles 'all' et 'clean' ne sont pas des fichiers
.PHONY: all run clean

# Exemple d'utilisation de makefile
# make all
# make run
# make clean
