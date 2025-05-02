# Variables
CC = gcc
CFLAGS = -Wall -I./include
LDFLAGS = -L/usr/local/lib -lmosquitto
SRC = src/dps310.c src/mqtt_utils.c src/main.c
OBJ = $(SRC:src/%.c=build/%.o)  # Les fichiers objets seront placés dans build/
EXEC = build/main  # L'exécutable sera dans le dossier build
HEADER = include/dps310.h include/mqtt_utils.h
BUILD_DIR = build  # Dossier pour les fichiers objets et l'exécutable

# Créer le dossier build s'il n'existe pas
$(shell mkdir -p $(BUILD_DIR))

# Règle par défaut pour compiler l'exécutable
all: $(EXEC)

# Règle pour créer l'exécutable principal
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)  # Ajouter LDFLAGS ici

# Règle pour compiler les fichiers .c en .o dans le dossier build/
build/%.o: src/%.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

# Règle pour exécuter le programme après la compilation
run: all
	./$(EXEC)

# Règle pour nettoyer les fichiers générés dans le dossier build
clean:
	rm -rf $(BUILD_DIR)

# Règle pour compiler et exécuter un fichier spécifique dans Tests (exemple: make test-mqtt_test)
test-%: Tests/%.c $(HEADER)
	$(CC) $(CFLAGS) $< -o build/$@ $(LDFLAGS)
	./build/$@
