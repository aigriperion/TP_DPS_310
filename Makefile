# Nom de l'exécutable
TARGET = dps310_reader

# Dossier pour l'exécutable
BIN_DIR = bin

# Dossiers source et include
SRC_DIR = src
INCLUDE_DIR = include

# Compilateur
CC = gcc

# Options de compilation
CFLAGS = -Wall -Wextra -O2 -I$(INCLUDE_DIR)

# Bibliothèques nécessaires
LIBS = -lpaho-mqtt3c

# Fichiers source
SRC = $(SRC_DIR)/main.c $(SRC_DIR)/dps310.c $(SRC_DIR)/mqtt_client.c

# Règle par défaut
all: $(BIN_DIR)/$(TARGET)

# Règle pour créer le dossier bin et l'exécutable
$(BIN_DIR)/$(TARGET): $(SRC)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$(TARGET) $(SRC) $(LIBS)

# Règle pour exécuter le programme
run: $(BIN_DIR)/$(TARGET)
	./$(BIN_DIR)/$(TARGET)

# Règle pour nettoyer les fichiers générés
clean:
	rm -rf $(BIN_DIR)