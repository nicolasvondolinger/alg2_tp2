CC = gcc
CFLAGS = -Wall -Wextra -g
BIN_DIR = bin
OBJ_DIR = obj
SRC = main.cpp
OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC))

all: $(BIN_DIR)/main

# Alvo para o servidor
$(BIN_DIR)/main: $(OBJ_DIR)/main.o
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(OBJ_DIR)/main.o

# Compilação dos objetos
$(OBJ_DIR)/%.o: %.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

.PHONY: all clean