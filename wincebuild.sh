#!/bin/sh

#./autogen.sh
#LDFLAGS=-L/usr/lib/gcc/arm-wince-pe/3.4.2/ CPPFLAGS=-I/usr/lib/gcc/arm-wince-pe/3.4.2/include/ ./configure --host=arm-wince-pe
PATH=/opt/mingw32ce/bin:$PATH LDFLAGS=" -L/opt/mingw32ce/arm-wince-mingw32ce/lib -L/opt/mingw32ce/lib" LIBS="-lgmp -lws2 -lwinsock -loleaut32 -lSDL_gfx" CPPFLAGS="-DWINCE -I/opt/mingw32ce/arm-wince-mingw32ce/include" ./configure --host=arm-wince-mingw32ce

#make clean
#make
#cd ../


