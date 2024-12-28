CC = gcc
CFLAGS = -Wall -Wextra -g
BIN_DIR = bin
OBJ_DIR = obj
SRC = main.cpp
OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRC))

all:
	mkdir -p obj
	g++ -Wall -Wextra -g main.cpp -o obj/main -lm

$(BIN_DIR)/main: $(OBJS)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: %.cpp
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

.PHONY: all clean
