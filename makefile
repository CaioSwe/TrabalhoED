# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Output file
TARGET = ED.exe

# Source and object files
SRC_DIR = src
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:.c=.o)

# Raylib and Windows libs
LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm

# Default build
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LIBS)

# Compile .c to .o
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	del /Q $(SRC_DIR)\*.o $(TARGET)

# Mark phony targets
.PHONY: all clean
