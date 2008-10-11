CFLAGS= -O0 -g3 -Wall
all:  pictool
clean: 
	rm main.o ../../sprdata.o

rebuild: clean all

pictool: main.o ../../sprdata.o
	$(CC) main.o ../../sprdata.o -o pictool  -lSDL -O0 -g3

YATC.pic: pictool ../../yatc.bmp ../../inv.bmp
	cp ../../831/Tibia.pic YATC.pic
	./pictool YATC.pic 0 ../../yatc.bmp --topic
	./pictool YATC.pic 1 ../../inv.bmp --topic
	./pictool YATC.pic 3 ../../ui.bmp --topic
	cp YATC.pic Tibia.pic
	cp YATC.pic ../../Tibia.pic

main.c: ../../sprdata.h

