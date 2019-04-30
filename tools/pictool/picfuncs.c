/* pictool: ANSI C converter for Tibia's PIC files
 * (c) 2007-2009 Ivan Vucica
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
#include <errno.h>
#if !_MSC_VER
#include <unistd.h>
#endif

#if !BAZEL_BUILD
#include "../../sprdata.h"
#else
#include "sprdata.h"
#endif
#include "picfuncs.h"

#pragma pack(1)
typedef struct {
	uint32_t signature;
	uint16_t imgcount;
} fileheader_t;
typedef struct {
	uint8_t width, height;
	uint8_t unk1, unk2, unk3; /* FIXME (ivucica#4#) zerocoolz says this should be colorkey, according to http://otfans.net/showpost.php?p=840634&postcount=134 */
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

int dumperror_stderr(char* txt, ...)
{
    va_list vl;
	va_start(vl, txt);

    int r = vfprintf(stderr, txt, vl);

    va_end(vl);

    return r;
}

int (*pictool_dumperror)(char*,...) = dumperror_stderr;



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
				return (10);
			}
			fwrite(&ph, sizeof(ph), 1, fo);

			for(j=0; j < ph.width * ph.height; j++){
				fread(&sprloc, sizeof(sprloc), 1, fi);

				if(sproffset > 4000000){
					dumperror_stderr("pictool: infinite loop\n");
					exit(8);
				}
				if(sprloc > filesize(fi)){
					dumperror_stderr("pictool: bad spr pointer\n");
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
					dumperror_stderr("pictool: allocation problem\n");
					return (7);
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

	if(rename("__tmp__.pic", filename)){
	    if (errno == 17) {// file exists
            if(unlink(filename)) {
                if (errno != 2)
                    return 93;
            }
            if(rename("__tmp__.pic", filename)){
                return 92;
            }
	    } else {
	        return 92;
	    }
	}

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
