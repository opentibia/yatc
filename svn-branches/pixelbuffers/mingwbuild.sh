#!/bin/sh

#./autogen.sh
#LDFLAGS=-L/usr/lib/gcc/arm-wince-pe/3.4.2/ CPPFLAGS=-I/usr/lib/gcc/arm-wince-pe/3.4.2/include/ ./configure --host=arm-wince-pe
LDFLAGS=" -L/usr/i586-mingw32msvc/lib" LIBS="-lgmp -lws2_32 -loleaut32 -lSDL_gfx" CPPFLAGS=" -I/usr/i586-mingw32msvc/include" ./configure --host=i586-mingw32msvc  --disable-opengl

#make clean
#make
#cd ../


