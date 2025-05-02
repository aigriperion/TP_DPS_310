# ==== CONFIGURATION ====

# Dossiers
SRC_DIR := src
TEST_DIR := Tests
BIN_DIR := bin

# Compilateur et flags
CC := gcc
CFLAGS := -Wall -Wextra -O2
LDFLAGS := -lmosquitto

# Projet principal (à implémenter plus tard)
MAIN_SRC := $(wildcard $(SRC_DIR)/*.c)
MAIN_BIN := $(BIN_DIR)/thermalprobe

# Fichiers de test
TEST_SRC := $(wildcard $(TEST_DIR)/*.c)
TEST_BINS := $(patsubst $(TEST_DIR)/%.c,$(BIN_DIR)/%.test,$(TEST_SRC))


# ==== RÈGLES PRINCIPALES ====

# Par défaut : compile le projet principal
all: $(MAIN_BIN)

# Projet principal (temporaire placeholder)
$(MAIN_BIN): $(MAIN_SRC)
	@echo "Pas encore de source principale, placeholder..."
	mkdir -p $(BIN_DIR)
	$(CC) -o $@ $^ $(LDFLAGS)

# ==== TESTS ====

# Règle pour compiler et exécuter les tests
test: $(TEST_BINS)
	@echo "Tous les tests ont été compilés et exécutés."
	@for test in $(TEST_BINS); do \
		echo "Exécution du test $$test..."; \
		./$$test; \
	done

# Exemple : make test-mqttsend
test-%: $(BIN_DIR)/%.test
	@echo "Execution du test '$*'..."
	./$<

# Compilation individuelle des tests
$(BIN_DIR)/%.test: $(TEST_DIR)/%.c
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# ==== NETTOYAGE ====

clean:
	rm -rf $(BIN_DIR)

.PHONY: all test clean test-%
