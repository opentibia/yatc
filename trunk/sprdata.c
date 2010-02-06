/*////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
// Sprite data reader and writer
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
///////////////////////////////////////////////////////////////////// */

/* Shared SPR data reading routine.
 * This algorithm is used in PICs and in SPRs. It's in a separate file
 * so that it can be shared with other tools such as PIC and SPR
 * converters.
 * This is intentionally a .C file since some tools are ANSI C programs
 * and should not be polluted by C++ code.
 */

#include <SDL/SDL.h>
#include "sprdata.h"
static void putPixel(SDL_Surface *surface, int x, int y, uint32_t pixel);
static uint32_t getPixel(SDL_Surface *surface, int x, int y);

// FIXME SOMEHOW (nfries88): Copied this here because we can't include util.h, which contains C++ code, from a file compiled with a C compiler.
#include <SDL/SDL_endian.h>
// NOTE(nfries88): SDL_SwapLE is a no-op on little-endian machines.
//  On big engian machines it always swaps byte order, so we can use it to convert back and forth between little engian and big endian. :)
#define ECORR16(var) var = SDL_SwapLE16(var)
#define ECORR32(var) var = SDL_SwapLE32(var)

int readSprData(FILE* f, SDL_Surface *surface, int offx, int offy)
{
    uint16_t size;
	register uint16_t destination=0;
	size_t i=0, dummy=0;
	register uint16_t j=0;
	register char transparent = 1;

	dummy = fread(&size, 2, 1, f);
	ECORR16(size);
	if (size > 3444) {
		SDL_UnlockSurface(surface);
		SDL_FreeSurface(surface);
		printf("Error [readSprData] Sprite size is invalid (%d)\n", size);
		return -1;
	}
	SDL_LockSurface(surface);

	for(i = ftell(f); ftell(f) < i + size-1; ){
		uint16_t pixelchunksize;
		register uint32_t color=0;
		unsigned char rgba[3];
		dummy = fread(&pixelchunksize, 2, 1, f);
		ECORR16(pixelchunksize);
		if(pixelchunksize > 3076){
			/* captain, the warp core breach has happened! what shall we do?! */
			SDL_UnlockSurface(surface);
			SDL_FreeSurface(surface);
			printf("Error [readSprData] Pixel chunk size is invalid (%d)\n", pixelchunksize);
			return -1;
			/* number one, eject the core */
		}

		if(!transparent){
			for(j = 0; j < pixelchunksize; j++){
				dummy = fread(&rgba, 3, 1, f);
				// NOTE (nfries88): Endianness correction for big endian machines, but not even sure if this is needed here...
				#if SDL_BYTEORDER == SDL_LIL_ENDIAN
				color = SDL_MapRGB(surface->format, rgba[0], rgba[1], rgba[2]);
				#else
				color = SDL_MapRGB(surface->format, rgba[2], rgba[1], rgba[0]);
				#endif
				putPixel(surface, offx+(destination+j) % 32, offy+(destination+j) / 32, color);
			}
		}
		destination += pixelchunksize;
		transparent = !transparent;

	}

	SDL_UnlockSurface(surface);
	return 0;
}
int prob;

int writeSprData(FILE* f, SDL_Surface *surface, int offx, int offy, uint16_t *datasize)
{
    /* adaptation of algorithm from Trooper's editor */
	int sizepos, dummy;
	unsigned long i;
	uint16_t chunksize;
	unsigned long chunksizepos;
	unsigned char transparent=1;
    #define debugprintf printf

	SDL_LockSurface(surface);

	i = 0; /* i == datasize */
	sizepos = ftell(f);
					/*if (sizepos == 936424) DO_DEBUG = 1;*/
	fseek(f, 2, SEEK_CUR);
	while (i < 1024) {


		/* transparency */
		chunksize = 0;
		while (i < 1024) {
			unsigned char rgba[4];
			uint32_t color = getPixel(surface, offx + (i%32), offy + (i/32));
			/*unsigned char ransparent;// = (color != SDL_MapRGB(surface->format, 255, 0, 255) && color != SDL_MapRGB(surface->format, 254, 0, 254));*/

			SDL_GetRGBA(color, surface->format, rgba, rgba+1, rgba+2, rgba+3);
			transparent = (*(rgba) >= 252 && *(rgba+1) <= 3 && *(rgba+2) >= 252);

			if (!transparent)
				break;

			i++;
			chunksize++;
		}
		ECORR16(chunksize);
		dummy = fwrite(&chunksize, 2, 1, f);

		if (i >= 1024) break;
		if (transparent) break;
		/* solid */
		chunksizepos = ftell(f);
		fseek(f, 2, SEEK_CUR);

		chunksize = 0;

		while (i < 1024) {
			unsigned char rgba[4];
			uint32_t color = getPixel(surface, offx + (i%32), offy + (i/32));


			SDL_GetRGBA(color, surface->format, rgba, rgba+1, rgba+2, rgba+3);
			transparent = (*(rgba) >= 252 && *(rgba+1) <= 3 && *(rgba+2) >= 252);

			if (transparent)
				break;

            // NOTE (nfries88): Endianness correction for big endian machines, but not even sure if this is needed here...
            ECORR32(*(uint32_t*)rgba);
			dummy = fwrite(rgba, 3, 1, f);

			i++;
			chunksize++;
		}

		fseek(f, chunksizepos, SEEK_SET);
		ECORR16(chunksize);
		dummy = fwrite(&chunksize, 2, 1, f);

		fseek(f, chunksize*3, SEEK_CUR);

	}

	i = ftell(f)-sizepos-2;
	if (transparent) i-=2;

	fseek(f, sizepos, SEEK_SET);
    // NOTE (nfries88): Endianness correction for big endian machines, but not sure if this is correct...
	uint16_t i16=(uint16_t)i;
	ECORR16(i16);
	dummy = fwrite(&i16, 2, 1, f);
	*datasize = i;

	SDL_UnlockSurface(surface);
	return 0;
}
int writeSprData__old(FILE* f, SDL_Surface *surface, int offx, int offy, uint16_t *datasize)
{
	uint16_t i = 0;
	uint32_t color = 0;
	int transparent = 1;
	int sizepos, chunksizepos, continuationpos;
	uint16_t size = 0, chunksize = 0;
	int done = 0, dummy = 0;

	sizepos = ftell(f);
	fseek(f, 2, SEEK_CUR);
	chunksizepos = sizepos + 2;
	while (!done) {
		if (i < 1024) {
			prob = 0;
			color = getPixel(surface, offx + i%32, offy + i/32);
			if (prob) printf("%d\n", i);
		}
		else {
			done = 1;
		}

		/* if we're drawing transparency, and we encountered a nontransparent pixel or we're done */
		if(transparent && ((color != SDL_MapRGB(surface->format, 255, 0, 255) && color != SDL_MapRGB(surface->format, 254, 0, 254)) || done)){
			transparent = 0; /* then we're no longer encountering transparency */
			dummy = fwrite(&chunksize, 2, 1, f); /* we should write down the chunk size (since we have not written any other byte in meantime, we can directly write it down) */
			chunksize = 0;
			size += 2;
			chunksizepos = ftell(f); /* next time we write down chunksize, we'll put it straight after this chunksize */
			fseek(f, 2, SEEK_CUR); /* so, we skip this placeholder*/
		}
		/* if we're drawing solidity, and we encountered a transparent pixel or we're done*/
		else if(!transparent && (color == SDL_MapRGB(surface->format, 255, 0, 255) || color == SDL_MapRGB(surface->format, 254, 0, 254) || done)){
			transparent = 1; /* that means we started drawing real stuff */

			continuationpos = ftell(f);
			fseek(f, chunksizepos, SEEK_SET);
			dummy = fwrite(&chunksize, 2, 1, f);
			fseek(f, continuationpos, SEEK_SET);
			size += 2;
			chunksize = 0;
		}

		if (!transparent && !done) { /* if we're getting solid stuff draw em */
			unsigned char rgb[3];
			SDL_GetRGB(color, surface->format, rgb, rgb+1, rgb+2);
			dummy = fwrite(rgb, 3, 1, f);
			size += 3;
		}
		chunksize += 1;
		i++;
	}

	size += 2;
	fseek(f, sizepos, SEEK_SET);
	dummy = fwrite(&size, 2, 1, f);
	*datasize = size;
	SDL_LockSurface(surface);
	return 0;
}

static void putPixel(SDL_Surface *surface, int x, int y, uint32_t pixel)
{
	int bpp = surface->format->BytesPerPixel;

	uint8_t *p = (uint8_t *)surface->pixels + y * surface->pitch + x * bpp;


    if (x >= surface->w || y >= surface->h)
        prob = 1, printf("Warning: Trying to write a pixel out of boundaries - %d, %d on a %dx%d image\n", x, y, surface->w, surface->h);

	switch(bpp){
	case 1:
		*p = pixel;
		break;

	case 2:
		*(uint16_t *)p = pixel;
		break;

	case 3:
		#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			p[0] = (pixel >> 16) & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = pixel & 0xff;
		#else
			p[0] = pixel & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = (pixel >> 16) & 0xff;
		#endif
		break;

	case 4:
		*(uint32_t *)p = pixel;
		break;
	}
}

static uint32_t getPixel(SDL_Surface *surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	uint8_t *p = (uint8_t *)surface->pixels + y * surface->pitch + x * bpp;

    if (x >= surface->w || y >= surface->h)
        prob = 1, printf("Warning: Trying to access a pixel out of boundaries - %d, %d on a %dx%d image\n", x, y, surface->w, surface->h);

	switch(bpp){
	case 1:
		return *p;

	case 2:
		return *(uint16_t *)p;

	case 3:
		#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			return p[0] << 16 | p[1] << 8 | p[2];
		#else
			return p[0] | p[1] << 8 | p[2] << 16;
		#endif

	case 4:
		return *(uint32_t *)p;

	default:
		return 0; /* shouldn't happen, but avoids warnings */
	}
}

