# Compilador y banderas
CC = gcc
CFLAGS = -Wall -Wextra -I./src/utils
LDFLAGS = -lcmocka

# Directorios
SRC_DIR = src
BIN_DIR = bin
TEST_DIR = test
LOG_DIR = log

# Archivos fuente y objeto
SRC_FILES = $(SRC_DIR)/vector.c $(SRC_DIR)/utils/memory.c
OBJ_FILES = $(BIN_DIR)/vector.o $(BIN_DIR)/utils/memory.o
TEST_FILES = $(TEST_DIR)/vector_test.c
TEST_OBJ_FILES = $(patsubst $(TEST_DIR)/%.c, $(BIN_DIR)/%.o, $(TEST_FILES))

# Regla para compilar la librería vector.o
$(BIN_DIR)/vector.o: $(SRC_DIR)/vector.c $(SRC_DIR)/vector.h $(SRC_DIR)/utils/memory.h
	@mkdir -p $(BIN_DIR)/utils
	$(CC) $(CFLAGS) -c -o $@ $<

# Regla para compilar memory.o
$(BIN_DIR)/utils/memory.o: $(SRC_DIR)/utils/memory.c $(SRC_DIR)/utils/memory.h
	@mkdir -p $(BIN_DIR)/utils
	$(CC) $(CFLAGS) -c -o $@ $<

# Regla para compilar los tests
$(BIN_DIR)/vector_test.o: $(TEST_DIR)/vector_test.c $(SRC_DIR)/vector.h $(SRC_DIR)/utils/memory.h
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Regla para compilar el ejecutable de test
$(BIN_DIR)/test: $(OBJ_FILES) $(TEST_OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

run: $(BIN_DIR)/test
	$(BIN_DIR)/test

valgrind: $(BIN_DIR)/test
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --errors-for-leak-kinds=all -s ./$(BIN_DIR)/test

# Regla para limpiar
clean:
	rm -rf $(BIN_DIR)/* $(LOG_DIR)/*

# Phony targets
.PHONY: clean all

# Regla por defecto
all: $(BIN_DIR)/test