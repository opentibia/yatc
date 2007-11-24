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

int readSprData(FILE* f, SDL_Surface *surface, int offx, int offy)
{
	uint16_t size;
	uint16_t destination=0;
	int i,j;
	int transparent = 1;

	fread(&size, 2, 1, f);
	if (size > 3500) {
        SDL_UnlockSurface(surface);
        SDL_FreeSurface(surface);
        fclose(f);
        printf("Error [readSprData] Sprite size is invalid (%d)\n", size);
        return -1;
    }
	SDL_LockSurface(surface);
	for(i = ftell(f); ftell(f) < i + size-1; ){
		uint16_t pixelchunksize;
		uint32_t color;
		unsigned char rgba[3];
		fread(&pixelchunksize, 2, 1, f);
		if(pixelchunksize > 3076){
			/* captain, the warp core breach has happened! what shall we do?! */
			SDL_UnlockSurface(surface);
			SDL_FreeSurface(surface);
			fclose(f);
			printf("Error [readSprData] Pixel chunk size is invalid (%d)\n", pixelchunksize);
			return -1;
			/* number one, eject the core */
		}

		if(!transparent){
			for(j = 0; j < pixelchunksize; j++){
				fread(&rgba, 3, 1, f);

				color = SDL_MapRGB(surface->format, rgba[0], rgba[1], rgba[2]);
				putPixel(surface, offx+(destination+j) % 32, offy+(destination+j) / 32, color);
			}
		}
		destination += pixelchunksize;
		transparent = !transparent;

	}

	SDL_UnlockSurface(surface);
	return 0;
}

int writeSprData(FILE* f, SDL_Surface *surface, int offx, int offy, uint16_t *datasize)
{
	uint16_t i = 0;
	uint32_t color = 0;
	int transparent = 1;
	int sizepos, chunksizepos, continuationpos;
	uint16_t size = 0, chunksize = 0;
	int done = 0;

	sizepos = ftell(f);
	fseek(f, 2, SEEK_CUR);
	chunksizepos = sizepos + 2;
	while (!done) {
		if (i <= 1024) {
			color = getPixel(surface, offx + i%32, offy + i/32);
		}
		else{
			done = 1;
		}

		/* if we're drawing transparency, and we encountered a nontransparent pixel or we're done */
		if(transparent && ((color != SDL_MapRGB(surface->format, 255, 0, 255) && color != SDL_MapRGB(surface->format, 254, 0, 254)) || done)){
			transparent = 0; /* then we're no longer encountering transparency */
			fwrite(&chunksize, 2, 1, f); /* we should write down the chunk size (since we have not written any other byte in meantime, we can directly write it down) */
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
			fwrite(&chunksize, 2, 1, f);
			fseek(f, continuationpos, SEEK_SET);
			size += 2;
			chunksize = 0;
		}

		if (!transparent && !done) { /* if we're getting solid stuff draw em */
			unsigned char rgb[3];
			SDL_GetRGB(color, surface->format, rgb, rgb+1, rgb+2);
			fwrite(rgb, 3, 1, f);
			size += 3;

		}
		chunksize += 1;
		i++;
	}

	fseek(f, sizepos, SEEK_SET);
	fwrite(&size, 2, 1, f);
	*datasize = size;
	SDL_LockSurface(surface);
	return 0;
}

static void putPixel(SDL_Surface *surface, int x, int y, uint32_t pixel)
{
	int bpp = surface->format->BytesPerPixel;

	uint8_t *p = (uint8_t *)surface->pixels + y * surface->pitch + x * bpp;
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

