//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
//
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
//////////////////////////////////////////////////////////////////////

#include <GL/gl.h>
#include "sprite.h"
#ifdef WIN32
#include <GL/glext.h>
#endif
#include "sprdata.h"

#pragma pack(1)
typedef struct {
	uint32_t signature;
	uint16_t imgcount;
} picfileheader_t;
typedef struct {
	uint8_t width, height;
	uint8_t unk1, unk2, unk3;
} picpicheader_t;



Sprite::Sprite(const std::string& filename, int index)
{
	m_pixelformat = GL_NONE;
	m_image = NULL;
	m_loaded = false;

	size_t extbegins = filename.rfind(".") + 1;
	std::string extension;
	if(extbegins != std::string::npos){
		extension = filename.substr(extbegins, filename.length() - extbegins);
	}

	if(extension == "bmp"){
		m_image = SDL_LoadBMP(filename.c_str());
		if(!m_image){
			printf("Error [Sprite::loadSurfaceFromFile] SDL_LoadBMP file: %s\n", filename.c_str());
			return;
		}
		m_pixelformat = GL_BGR;
		m_loaded = true;
	}
	else if(extension == "spr"){
		uint32_t signature; // TODO (ivucica#3#) signature should be perhaps read during logon?
		uint16_t sprcount;
		uint32_t where;

		FILE *f = fopen(filename.c_str(), "r");
		if(!f){
			printf("Error [Sprite::loadSurfaceFromFile] Sprite file %s not found\n", filename.c_str());
			return;
		}
		fread(&signature, sizeof(signature), 1, f);
		fread(&sprcount, sizeof(sprcount), 1, f);
		if(index >= sprcount){// i can't do this, captain, there's not enough power
			printf("Error [Sprite::loadSurfaceFromFile] Loading spr index %d while we have %d sprites in file.\n", index, sprcount);
			return; // she won't hold it much longer
		}

		// read the pointer to the real SPR data
		fseek(f, index*4, SEEK_CUR);
		fread(&where, sizeof(where), 1, f);

		// create surface where we'll store data, and fill it with transparency
		m_image = SDL_CreateRGBSurface(SDL_SWSURFACE, 32, 32, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000); // FIXME (ivucica#5#) Potentially unportable to architectures with different endianess, take a look at SDL docs and make all Liliputtans happy
		if(!m_image){
			printf("Error [Sprite::loadSurfaceFromFile] Cant create SDL Surface.\n");
			return;
		}
		// dont make static since if we change the rendering engine at runtime,
		//  this may change too
		Uint32 magneta = SDL_MapRGB(SDL_GetVideoInfo()->vfmt, 255, 0, 255);
		SDL_FillRect(m_image, NULL, magneta);

		// read the data
		fseek(f, where, SEEK_SET);
		fgetc(f); fgetc(f); fgetc(f); // TODO (ivucica#4#) maybe we should figure out what do these do?
		if (readSprData(f, m_image, 0, 0)) {
			// error happened
			SDL_FreeSurface(m_image);
			m_image = NULL;
			return;
		}

		fclose(f);
		SDL_UpdateRect(m_image, 0, 0, 32, 32);

		m_pixelformat = GL_RGBA;
		m_loaded = true;
	}
	else if(extension == "pic"){
		FILE *f;
		SDL_Surface *s;
		int i,j,k;
		picfileheader_t fh;
		picpicheader_t ph;
		uint32_t sprloc;
		uint32_t magneta;


		f = fopen(filename.c_str(), "rb");
		if(!f){
			printf("Error [Sprite::loadSurfaceFromFile] Sprite file %s not found\n", filename.c_str());
			return;
		}

		fread(&fh, sizeof(fh), 1, f);

		for(i = 0; i < fh.imgcount && i <= index ; i++) {
			fread(&ph, sizeof(ph), 1, f);

			if(i == index){
				s = SDL_CreateRGBSurface(SDL_SWSURFACE, ph.width*32, ph.height*32, 32, 0xFF, 0xFF00, 0xFF0000, 0xFF000000);

				magneta = SDL_MapRGB(SDL_GetVideoInfo()->vfmt, 255, 0, 255);
				SDL_FillRect(s, NULL, magneta);

				for(j = 0; j < ph.height; j++){
					for(k = 0; k < ph.width; k++){
						fread(&sprloc, sizeof(sprloc), 1, f);

						int oldloc = ftell(f);
						int r;
						fseek(f, sprloc, SEEK_SET);
						r = readSprData(f, s, k*32, j*32);
						fseek(f, oldloc, SEEK_SET);

						if(r){
							SDL_FreeSurface(s);
							fclose(f);
							return;
						}
					}
				}
			}
			else{
				fseek(f, sizeof(sprloc)*ph.height*ph.width, SEEK_CUR);
			}
		}

		fclose(f);

		m_image = s;

		m_loaded = true;
	}
	else{
		// m_image is already marked as NULL, so we're over
		return;
	}

	SDL_SetColorKey(m_image, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(SDL_GetVideoInfo()->vfmt, 0xFF, 0, 0xFF)); // magenta is transparent
}

Sprite::~Sprite()
{
	if(m_image){
		SDL_FreeSurface(m_image);
	}
}

void Sprite::putPixel(int x, int y, uint32_t pixel)
{
	int bpp = m_image->format->BytesPerPixel;

	uint8_t *p = (uint8_t *)m_image->pixels + y * m_image->pitch + x * bpp;
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

uint32_t Sprite::getPixel(int x, int y)
{
	int bpp = m_image->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	uint8_t *p = (uint8_t *)m_image->pixels + y * m_image->pitch + x * bpp;

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

