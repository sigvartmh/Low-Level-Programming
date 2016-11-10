CC=gcc
LDFLAGS= `sdl2-config --cflags --libs`
CFLAGS=-c -Wall
INCLUDES=-I/Library/Frameworks/SDL2
game: main.o
		$(CC) $(LDFLAGS) main.o -o game

main.o : main.c
		$(CC) $(CFLAGS) $(LDFLAGS) main.c
