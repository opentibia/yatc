CFLAGS=-ansi -O0 -g3 -Wall
all: __tmp__.pic pictool
clean: 
	rm main.o ../../sprdata.o __tmp__.pic
rebuild: clean all
__tmp__.pic: pictool 
	echo "=========CONVERTING TO BMP============"
	./pictool Tibia.pic 2 img.bmp --tobmp
#	echo "=========CONVERTING BACK TO PIC========="
#	./pictool Tibia.pic 2 img.bmp --topic
#	echo "=======TRYING TO READ THAT PIC========="
#	./pictool __tmp__.pic 0 img2.bmp --tobmp

pictool: main.o ../../sprdata.o
	gcc main.o ../../sprdata.o -o pictool  -lSDL -O0 -g3

main.c: ../../sprdata.h

