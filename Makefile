# Variables
CC = gcc
CFLAGS = -Wall -Wextra -I./src/include -I./src/utils -fPIC
LDFLAGS = -lcmocka

# Directorios
SRC_DIR = src
INCLUDE_DIR = $(SRC_DIR)/include
UTILS_DIR = $(SRC_DIR)/utils
BIN_DIR = bin
TEST_DIR = test
LOG_DIR = log

# Archivos fuente y objeto
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
UTILS_FILES = $(wildcard $(UTILS_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(SRC_FILES)) $(patsubst $(UTILS_DIR)/%.c, $(BIN_DIR)/%.o, $(UTILS_FILES))
TEST_FILES = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJ_FILES = $(patsubst $(TEST_DIR)/%.c, $(BIN_DIR)/%.o, $(TEST_FILES))
TEST_EXECUTABLES = $(patsubst $(TEST_DIR)/%.c, $(BIN_DIR)/%, $(TEST_FILES))

# Archivos objeto específicos para la librería vector.so (solo los de src/)
VECTOR_OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(SRC_FILES))

# Regla para compilar archivos objeto en src/
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

# Regla para compilar archivos objeto en src/utils/
$(BIN_DIR)/%.o: $(UTILS_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

# Regla para compilar archivos de test
$(BIN_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

# Regla para compilar la librería compartida vector.so
$(BIN_DIR)/vector.so: $(VECTOR_OBJ_FILES)
	$(CC) -shared -o $@ $^

# Regla para compilar cada ejecutable de test
$(BIN_DIR)/%: $(BIN_DIR)/%.o $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Regla para compilar todos los tests
tests: $(TEST_EXECUTABLES)

# Regla para correr un test específico
test-%: $(BIN_DIR)/%
	@./$<

# Regla para correr todos los tests
test: $(TEST_EXECUTABLES)
	@for test in $(TEST_EXECUTABLES); do ./$$test; done

# Regla para correr los tests con Valgrind
valgrind-%: $(BIN_DIR)/%
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --errors-for-leak-kinds=all -s ./$<

# Regla para instalar la librería
install: $(BIN_DIR)/vector.so
	sudo mv $(BIN_DIR)/vector.so /usr/lib/vector.so

# Regla para limpiar
clean:
	rm -rf $(BIN_DIR)/* $(LOG_DIR)/*

# Phony targets
.PHONY: clean all tests test valgrind install

# Regla por defecto
all: $(BIN_DIR)/vector.so tests
