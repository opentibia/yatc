/* ANSI C converter for Tibia's PIC files
 * (c) 2007 Ivan Vucica
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


/* If your compiler does not support uint32_t and friends, then
 * change #if (0) into #if (1) and we'll try defining them.
 * Note that most modern compilers should support them; GCC
 * supports them even in ANSI mode, but then, GCC is not the
 * only compiler out there. */
#if (0)
#define uint32_t unsigned long
#define uint16_t unsigned short
#define uint8_t unsigned char
#endif

/* Below, 1 means "debugging output on", and 0 means "debugging output off". */
#if (0)
#define dbgprintf printf
#else
static void dbgprintf(const char* txt, ...) {}
#endif


/* Headers */
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>

#include "../../sprdata.h"

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


static int filesize (FILE* f) {
        int loc = ftell(f);
        int size = 0;

        fseek(f, 0, SEEK_END);
        size = ftell(f);
        fseek(f, loc, SEEK_SET);
        return size;
}

int writesprite (FILE *f, SDL_Surface *s, int offx, int offy, uint16_t *datasize) {
	return writeSprData(f, s, offx, offy, datasize);
}

int readsprite (FILE *f, uint32_t sprloc, SDL_Surface *s, int offx, int offy) {
	int oldloc = ftell(f);
	int r;

	fseek(f, sprloc, SEEK_SET);

	r = readSprData(f, s, offx, offy);

	fseek(f, oldloc, SEEK_SET);
	return r;
}

int picdetails (const char* filename) {
    FILE *f;
    int i,j,k;
    fileheader_t fh;
    picheader_t ph;
	uint32_t sprloc;

	f = fopen(filename, "rb");
	printf("information for %s\n", filename);
	if (!f)
		return -1;

    fread(&fh, sizeof(fh), 1, f);
	printf("signature %d\n", fh.signature);
	printf("imagecount %d\n", fh.imgcount);
	for(i = 0; i < fh.imgcount ; i++){
		fread(&ph, sizeof(ph), 1, f);
		 printf("img%d width %d height %d bg rgb #%02x%02x%02x\n", i, ph.width, ph.height, ph.unk1, ph.unk2, ph.unk3);
		for(j = 0; j<ph.height; j++){
			for(k = 0; k < ph.width; k++){
				fread(&sprloc, sizeof(sprloc), 1, f);
				printf("sprite img %d x %d y %d location %d\n", i,k,j,sprloc);
			}
		}
	}
	return 0;
}

int writepic(const char* filename, int index, SDL_Surface *s){

	FILE *fi, *fo;
	fileheader_t fh;
	picheader_t ph;
	uint32_t sprloc, sproffset;
	size_t continuationposi, continuationposo;
	uint16_t datasize;
	void *data;
	int i,j,k;
	fi = fopen(filename, "rb");
	fo = fopen("__tmp__.pic","wb+");


	if (!fi || !fo)
		return -1;
	fread(&fh, sizeof(fh), 1, fi);
	fwrite(&fh, sizeof(fh), 1, fo);

	sproffset = fh.imgcount * (sizeof(ph)+1)-2;
	for(i = 0; i < fh.imgcount; i++){
		fread(&ph, sizeof(ph), 1, fi);
		if(i == index){
			ph.width = s->w / 32;
			ph.height = s->h / 32;
		}
		sproffset += ph.width * ph.height * 4;
		fseek(fi, ph.width*ph.height*4, SEEK_CUR);
	}


	fseek(fi, sizeof(fh), SEEK_SET);
	for(i = 0; i < fh.imgcount; i++){
		fread(&ph, sizeof(ph), 1, fi);

		if(i != index){
			if(!ph.width || !ph.height){
				fprintf(stderr, "pictool: width or height are 0\n");
				exit(10);
			}
			fwrite(&ph, sizeof(ph), 1, fo);

			for(j=0; j < ph.width * ph.height; j++){
				fread(&sprloc, sizeof(sprloc), 1, fi);

				if(sproffset > 2000000){
					fprintf(stderr, "pictool: infinite loop\n");
					exit(8);
				}
				if(sprloc > filesize(fi)){
					fprintf(stderr, "pictool: bad spr pointer\n");
					exit(9);
				}
				fwrite(&sproffset, sizeof(sproffset), 1, fo);

				continuationposi = ftell(fi);
				continuationposo = ftell(fo);

				fseek(fi, sprloc, SEEK_SET);
				fseek(fo, sproffset, SEEK_SET);

				fread(&datasize, sizeof(datasize), 1, fi);
				fwrite(&datasize, sizeof(datasize), 1, fo);
				data = malloc(datasize+2);
				if(!data){
					fprintf(stderr, "Allocation problem\n");
					exit(7);
				}
				fread(data, datasize+2, 1, fi);
				fwrite(data, datasize+2, 1, fo);
				free(data);

				fseek(fo, continuationposo, SEEK_SET);
				fseek(fi, continuationposi, SEEK_SET);
				sproffset += datasize+2; // 2 == space for datasize
			}
			fflush(fo);
		}
		else{
			fseek(fi, ph.width*ph.height*4, SEEK_CUR);
			ph.width = s->w / 32; ph.height = s->h / 32;
			fwrite(&ph, sizeof(ph), 1, fo);
			for(j = 0; j < ph.height; j++){
				for(k = 0; k < ph.width; k++){
					/*printf("Placing %d %d on %d\n", j, k, sproffset);*/
					fwrite(&sproffset, sizeof(sproffset), 1, fo);

					continuationposo = ftell(fo);
					fseek(fo, sproffset, SEEK_SET);
					writesprite(fo, s, k * 32, j*32, &datasize);
					/*printf("Its size is: %d\n", datasize);*/

					fseek(fo, continuationposo, SEEK_SET);
					sproffset += datasize+2;
				}


			}
			fflush(fo);
		}
	}

	fclose(fo);
	fclose(fi);

	rename("__tmp__.pic", filename);

	return 0;
}

int readpic (const char* filename, int index, SDL_Surface **sr) {
	/* index >= -1; -1 means that we should print out details */
	SDL_Surface *s=NULL;
	FILE *f;
	int i,j,k;
	fileheader_t fh;
	picheader_t ph;
	uint32_t sprloc;
	uint32_t magenta;

	f = fopen(filename, "rb");
	if (!f)
		return -1;

	fread(&fh,sizeof(fh),1,f);

	
	for(i = 0; i < fh.imgcount && i <= index; i++){
		fread(&ph, sizeof(ph), 1, f);

		if(i == index){
			s = SDL_CreateRGBSurface(SDL_SWSURFACE, ph.width*32, ph.height*32, 32, 0xFF, 0xFF00, 0xFF0000, 0xFF000000);
			if(!s){
				printf("CreateRGBSurface failed: %s\n", SDL_GetError());
				return -1;
			}
			
			magenta = SDL_MapRGB(s->format, 255, 0, 255);
			SDL_FillRect(s, NULL, magenta);

			/* FIXME (ivucica#4#) Above statement is potentially unportable to architectures with
			 * different endianess. Lilliputtans would be happier if we took a look at SDL
			 * docs and corrected this. */

			for(j = 0; j < ph.height; j++){
				for(k = 0; k < ph.width; k++){
					fread(&sprloc, sizeof(sprloc), 1, f);
					dbgprintf(":: reading sprite at pos %d %d\n", j, k);
					if(readsprite(f, sprloc, s, k*32, j*32)){ /* TODO (ivucica#1#) cleanup sdl surface upon error */
						return -1;
					}
				}
			}
		}
		else{
			fseek(f, sizeof(sprloc)*ph.height*ph.width, SEEK_CUR);
		}
	}

	fclose(f);
	*sr = s;
	return 0;
}

void show_help() {
	printf(" ANSI C converter for Tibia's PIC files\n"
		"  (c) 2007 Ivan Vucica\n"
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
		dbgprintf(":: Loading from bitmap %s\n", argv[3]);
		s = SDL_LoadBMP(argv[3]);
		dbgprintf(":: Success\n");
		writepic(argv[1], atoi(argv[2]), s);
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
