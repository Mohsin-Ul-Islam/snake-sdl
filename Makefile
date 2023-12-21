CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic

all: src/main.c
	$(CC) $(CFLAGS) -o main src/*.c src/*.h $(shell pkg-config sdl2 --cflags --libs)

run: all
	./main
