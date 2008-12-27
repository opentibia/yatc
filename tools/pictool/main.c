/* pictool: ANSI C converter for Tibia's PIC files
 * (c) 2007-2008 Ivan Vucica
 * Part of OpenTibia project
 *
 * Although written in ANSI C, this makes use of #pragma pack(),
 * make sure your compiler supports packed structures, or else.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */



/* Headers */
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>

#include "../../sprdata.h"
#include "picfuncs.h"

#pragma pack(1)
typedef struct {
	uint32_t signature;
	uint16_t imgcount;
} fileheader_t;
typedef struct {
	uint8_t width, height;
	uint8_t unk1, unk2, unk3;
} picheader_t;
#pragma pack()


void show_help() {
	printf(" pictool: ANSI C converter for Tibia's PIC files\n"
		"  (c) 2007-2008 Ivan Vucica\n"
		"  Part of OpenTibia project\n"
		"\n");
	printf("  pictool --help\n");
	printf("  pictool picfilename picindex bmpfilename [--tobmp|--topic]\n");
	printf("  pictool picfilename\n");
	printf("\n");
	printf("   --help		Shows this help\n");
	printf("   --tobmp		Saves the specified subimage of .PIC into a .BMP file\n");
	printf("   --topic		Saves the .BMP into the specified subimage of .PIC file\n");
	printf("\n");
	printf(" When passed only the .PIC filename, the program will output details about\n"
		" it on standard output.\n");
	printf("\n");
	printf(" This program cannot create new .PIC from scratch; a .PIC must already\n"
        " exist.\n");
	printf("\n");
	printf(" This is free software: you are free to change and redistribute it.\n"
		" There is NO WARRANTY, to the extent permitted by law. \n"
		" Review LICENSE in YATC distribution for details.\n");
}

int main (int argc, char **argv) {
	SDL_Surface *s = NULL;

	if (argc == 1){
		fprintf(stderr, "pictool: no input files\n");
		exit(1);
	}
	if(argc == 2){
		if(!strcmp(argv[1], "--help")){
			show_help();
			exit(0);
		}
		else{
			if(picdetails(argv[1])){
				fprintf(stderr, "pictool: bad format\n");
				exit(4);
			}
			exit(0);
		}
	}
	if(argc == 3){
		fprintf(stderr, "pictool: not enough arguments\n");
		exit(2);
	}

	SDL_Init(SDL_INIT_VIDEO);

	if(argc == 5 && !strcmp(argv[4], "--topic")){
	    int error;
		dbgprintf(":: Loading from bitmap %s\n", argv[3]);
		s = SDL_LoadBMP(argv[3]);
		dbgprintf(":: Success\n");

		if (error = writepic(argv[1], atoi(argv[2]), s))
		{
		    printf("Error writing pic: %d\n", error);
		    exit(error);
		}
		SDL_FreeSurface(s);
	}
	else if(argc == 4 || !strcmp(argv[4], "--tobmp")){
		if(readpic(argv[1], atoi(argv[2]), &s)){
			fprintf(stderr, "pictool: conversion to bmp failed: bad format\n");
			SDL_Quit();
			exit(4);
		}
		dbgprintf(":: Saving to %s\n", argv[3]);
		if(SDL_SaveBMP(s, argv[3])){
			fprintf(stderr, "pictool: saving to bmp failed\n");
			SDL_Quit();
			exit(6);
		}
		dbgprintf(":: Success\n");
		SDL_FreeSurface(s);
	}
	else{
		fprintf(stderr, "pictool: unrecognized argument\n");
		SDL_Quit();
		exit(5);
	}

    dbgprintf(":: SDLQuit\n");
    SDL_Quit();
    dbgprintf(":: Return\n");
	return 0;
}
