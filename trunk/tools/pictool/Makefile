CFLAGS=-ansi -O0 -g3 -Wall
all:  pictool
clean: 
	rm main.o ../../sprdata.o

rebuild: clean all

pictool: main.o ../../sprdata.o
	gcc main.o ../../sprdata.o -o pictool  -lSDL -O0 -g3

YATC.pic: pictool ../../yatc.bmp ../../inv.bmp
	cp Tibia.pic YATC.pic
	./pictool YATC.pic 0 ../../yatc.bmp --topic
	./pictool YATC.pic 2 ../../inv.bmp --topic
	cp YATC.pic ../../Tibia.pic

main.c: ../../sprdata.h

