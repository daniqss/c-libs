CC = gcc
CFLAGS = -Wall -Wextra -I./src/utils -fPIC
LDFLAGS = -lcmocka

# Directorios
SRC_DIR = src
BIN_DIR = bin
TEST_DIR = test
LOG_DIR = log

# Archivos fuente y objeto
SRC_FILES = $(wildcard $(SRC_DIR)/**/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(SRC_FILES))
TEST_FILES = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJ_FILES = $(patsubst $(TEST_DIR)/%.c, $(BIN_DIR)/%.o, $(TEST_FILES))

# Regla para compilar archivos objeto
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

# Regla para compilar archivos de test
$(BIN_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

# Regla para compilar la librería compartida vector.so
$(BIN_DIR)/vector.so: $(OBJ_FILES)
	$(CC) -shared -o $@ $^

# Regla para compilar el ejecutable de test
$(BIN_DIR)/test: $(OBJ_FILES) $(TEST_OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Regla para correr los tests
test: $(BIN_DIR)/test
	$(BIN_DIR)/test

# Regla para correr los tests con Valgrind
valgrind: $(BIN_DIR)/test
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --errors-for-leak-kinds=all -s ./$(BIN_DIR)/test

# Regla para limpiar
clean:
	rm -rf $(BIN_DIR)/* $(LOG_DIR)/*

# Phony targets
.PHONY: clean all test valgrind

# Regla por defecto
all: $(BIN_DIR)/vector.so test
