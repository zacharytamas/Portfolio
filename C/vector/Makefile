CFLAGS = -g -Wall
CC = gcc

objects = main.o node.o vector.o MenuLib.o

test: $(objects)
	gcc -o test $(objects)

main.o: node.h vector.h MenuLib.h
vector.o: vector.c vector.h
node.o: node.c node.h
MenuLib.o: MenuLib.c MenuLib.h

.PHONY : clean
clean: 
	rm test $(objects)