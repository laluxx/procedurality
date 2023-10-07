# Compiler and flags
CC = gcc
CFLAGS = -Wall -std=c99 -I/usr/local/include  # Path to raylib's include if it's not in the standard location
LDFLAGS = -L/usr/local/lib -lraylib -lm  # Link with raylib and math library

# Source and output
SOURCE = main.c
OUTPUT = app

all: $(OUTPUT)

$(OUTPUT): $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) -o $(OUTPUT) $(LDFLAGS)

clean:
	rm -f $(OUTPUT)
