CC = gcc
CFLAGS = -Wall -g -Iinclude
LDFLAGS = -lcrypto
SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = include
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))
EXECUTABLE = tronics
HEADERS = $(wildcard $(INCLUDE_DIR)/*.h)

# Check if main.c exists
ifeq (,$(wildcard $(SRC_DIR)/main.c))
$(error Source file src/main.c not found. Please ensure it exists in the src directory.)
endif

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(EXECUTABLE)

.PHONY: all clean