CFLAGS = -g -Wall
CC = gcc

objects = main.o Maze.o

test: $(objects)
	gcc -o maze $(objects)

main.o: Maze.h
Maze.o: Maze.h Maze.c

.PHONY : clean
clean: 
	rm maze $(objects)