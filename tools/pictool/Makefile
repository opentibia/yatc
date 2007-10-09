CFLAGS=-ansi -O0 -g3 -Wall
all: __tmp__.pic pictool
clean: 
	rm main.o ../../sprdata.o __tmp__.pic
rebuild: clean all

pictool: main.o ../../sprdata.o
	gcc main.o ../../sprdata.o -o pictool  -lSDL -O0 -g3

main.c: ../../sprdata.h

