# Nom de l'exécutable
TARGET = thermalprobe

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
	./$(BIN_DIR)/$(TARGET) temp 5

# Règle pour nettoyer les fichiers générés
clean:
	rm -rf $(BIN_DIR)

# Vérification des dépendances
check-deps:
	@command -v gcc >/dev/null 2>&1 || { echo "gcc is not installed. Please install it."; exit 1; }
	@ldconfig -p | grep -q paho-mqtt3c || { echo "Paho MQTT C library is not installed. Please install it."; exit 1; }