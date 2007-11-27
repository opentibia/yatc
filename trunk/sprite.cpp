//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
// Base engine-independant sprite class
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

#include <SDL/SDL_rotozoom.h> // library you need is called SDL_gfx

#ifdef USE_OPENGL
	#include <GL/gl.h>
#endif

#ifndef GL_BGR
#warning Odd OpenGL header. Defining GL_BGR to a 0.
#define GL_BGR 0
#endif
#include "sprite.h"
#include "sprdata.h"
#include <math.h>

#pragma pack(1)
typedef struct {
	uint32_t signature;
	uint16_t imgcount;
} picfileheader_t;
typedef struct {
	uint8_t width, height;
	uint8_t unk1, unk2, unk3;
} picpicheader_t;
#pragma pack()


Sprite::Sprite(const std::string& filename, int index)
{
	#ifdef USE_OPENGL
	m_pixelformat = GL_NONE;
	#endif
	m_image = NULL;
	m_stretchimage = NULL;
	m_loaded = false;
	m_smoothstretch = 0;
    m_r = 1.f;
    m_g = 1.f;
    m_b = 1.f;

	loadSurfaceFromFile(filename, index);
}


Sprite::Sprite(const std::string& filename, int index, int x, int y, int w, int h)
{
	#ifdef USE_OPENGL
	m_pixelformat = GL_NONE;
	#endif
	m_image = NULL;
	m_stretchimage = NULL;
	m_loaded = false;
	m_smoothstretch = 1;
	m_r = 1.f;
    m_g = 1.f;
    m_b = 1.f;

	loadSurfaceFromFile(filename, index);

	if(!m_image){
		return;
	}

	SDL_Surface *ns = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 0xFF, 0xFF00, 0xFF0000, 0xFF000000);
	SDL_Rect src = {x,y,w,h};
	SDL_Rect dst = {0,0,w,h};
	SDL_BlitSurface(m_image, &src, ns, &dst);

	SDL_FreeSurface(m_image);
	SDL_FreeSurface(m_coloredimage);
	m_image = ns;
	m_coloredimage = SDL_CreateRGBSurface(SDL_SWSURFACE, m_image->w, m_image->h, 32, 0xFF, 0xFF00, 0xFF0000, 0xFF000000);
}



Sprite::~Sprite()
{
	if(m_image){
		SDL_FreeSurface(m_image);
	}
	if(m_stretchimage){
		SDL_FreeSurface(m_stretchimage);
	}
	if(m_coloredimage){
        SDL_FreeSurface(m_coloredimage);
	}
}



void Sprite::loadSurfaceFromFile(const std::string& filename, int index) {
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
		#ifdef USE_OPENGL
		m_pixelformat = GL_BGR;
		#endif
		m_loaded = true;
	}
	else if(extension == "spr"){
		uint32_t signature; // TODO (ivucica#3#) signature should be perhaps read during logon?
		uint16_t sprcount;
		uint32_t where;

		FILE *f = fopen(filename.c_str(), "rb");
		if(!f){
			printf("Error [Sprite::loadSurfaceFromFile] Sprite file %s not found\n", filename.c_str());
			return;
		}

		if (index < 1) {
			printf("Error [Sprite::loadSurfaceFromFile] Invalid index %d\n", index);
			fclose(f);
			return;
		}

		fread(&signature, sizeof(signature), 1, f);
		fread(&sprcount, sizeof(sprcount), 1, f);
		if(index >= sprcount){// i can't do this, captain, there's not enough power
			printf("Error [Sprite::loadSurfaceFromFile] Loading spr index %d while we have %d sprites in file.\n", index, sprcount);
			return; // she won't hold it much longer
		}

		// read the pointer to the real SPR data
		fseek(f, (index-1)*4, SEEK_CUR);
		fread(&where, sizeof(where), 1, f);

		// create surface where we'll store data, and fill it with transparency
		m_image = SDL_CreateRGBSurface(SDL_SWSURFACE, 32, 32, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000); // FIXME (ivucica#5#) Potentially unportable to architectures with different endianess, take a look at SDL docs and make all Liliputtans happy
		if(!m_image){
			printf("Error [Sprite::loadSurfaceFromFile] Cant create SDL Surface.\n");
			return;
		}
		printf("Created rgb surface\n");
		// dont make static since if we change the rendering engine at runtime,
		//  this may change too
		Uint32 magenta = SDL_MapRGB(SDL_GetVideoInfo()->vfmt, 255, 0, 255);
		SDL_FillRect(m_image, NULL, magenta);

		// read the data
		fseek(f, where, SEEK_SET);
		fgetc(f); fgetc(f); fgetc(f); // TODO (ivucica#4#) maybe we should figure out what do these do?
		if (readSprData(f, m_image, 0, 0)) {
			// error happened
			printf("Error [Sprite::loadSurfaceFromFile] Problem in readSprData()\n");
			SDL_FreeSurface(m_image);
			m_image = NULL;
			fclose(f);
			return;
		}

		fclose(f);
		SDL_UpdateRect(m_image, 0, 0, 32, 32);

		#ifdef USE_OPENGL
		m_pixelformat = GL_RGBA;
		#endif
		m_loaded = true;
	}
	else if(extension == "pic"){
		FILE *f;
		SDL_Surface *s = NULL;
		picfileheader_t fh;
		picpicheader_t ph;
		uint32_t sprloc;
		uint32_t magenta;


		f = fopen(filename.c_str(), "rb");
		if(!f){
			printf("Error [Sprite::loadSurfaceFromFile] Picture file %s not found\n", filename.c_str());
			return;
		}

		fread(&fh, sizeof(fh), 1, f);

		for(int i = 0; i < fh.imgcount && i <= index ; i++) {
			fread(&ph, sizeof(ph), 1, f);

			if(i == index){
				printf(":: Loading pic index %d\n", i);
				s = SDL_CreateRGBSurface(SDL_SWSURFACE, ph.width*32, ph.height*32, 32, 0xFF, 0xFF00, 0xFF0000, 0xFF000000);
				if (!s) {
				    printf(":: Failed to create surface of size %dx%d\n", ph.width*32, ph.height*32);
                    fclose(f);
                    return;
				}
                printf(":: Created surface\n");
				magenta = SDL_MapRGB(SDL_GetVideoInfo()->vfmt, 255, 0, 255);
				SDL_FillRect(s, NULL, magenta);

				for(int j = 0; j < ph.height; j++){
					for(int k = 0; k < ph.width; k++){
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
        printf(":: Loading pic complete\n");
		m_image = s;
		#ifdef USE_OPENGL
		m_pixelformat = GL_RGBA;
		#endif
		m_loaded = true;
	}
	else{
		// m_image is already marked as NULL, so we're over
		return;
	}

	m_filename = filename;
	m_index = index;

    //m_coloredimage = SDL_CreateRGBSurface(SDL_SWSURFACE, m_image->w, m_image->h, 32, 0xFF, 0xFF00, 0xFF0000, 0xFF000000);

	SDL_SetColorKey(m_image, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(SDL_GetVideoInfo()->vfmt, 0xFF, 0, 0xFF)); // magenta is transparent
}


void Sprite::putPixel(int x, int y, uint32_t pixel, SDL_Surface *img)
{
	if (!img)
        img = m_image;
	int bpp = img->format->BytesPerPixel;

	uint8_t *p = (uint8_t *)img->pixels + y * img->pitch + x * bpp;

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

uint32_t Sprite::getPixel(int x, int y, SDL_Surface *img)
{
	if (!img)
        img = m_image;
	int bpp = img->format->BytesPerPixel;

	/* Here p is the address to the pixel we want to retrieve */
	uint8_t *p = (uint8_t *)img->pixels + y * img->pitch + x * bpp;

    if (x >= surface->w || y >= surface->h)
        prob = 1, printf("Warning: Trying to read a pixel out of boundaries - %d, %d on a %dx%d image\n", x, y, surface->w, surface->h);


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

void Sprite::Stretch (float w, float h, int smooth, bool force)
{
	SDL_Surface *img;
	if (m_stretchimage && !force)
		if (fabs(m_stretchimage->w - w) < 2 && fabs(m_stretchimage->h - h)<2)
			return;

    if (m_r == 1.f && m_g == 1.f && m_b == 1.f)
        img = m_image;
    else
        img = m_coloredimage;

    if (smooth == -1)
        smooth = m_smoothstretch;
    else
        m_smoothstretch = smooth;


	unStretch();

	if (w == m_image->w && h == m_image->h)
        return;

    printf("Stretching to %g %g\n", w, h);
	m_stretchimage = zoomSurface(img, w/img->w, h/img->h, smooth);

	printf("New size: %d %d\n", m_stretchimage->w, m_stretchimage->h);

}

void Sprite::addColor(float r, float g, float b)
{
    uint8_t ro, go, bo;  // old rgb
    if (r == m_r && g == m_g && b == m_b)
        return;
    for (int i=0; i < m_image->w; i++)
        for (int j=0; j < m_image->h; j++) {
            SDL_GetRGB(getPixel(i,j, getImage()), getImage()->format, &ro, &go, &bo);
            putPixel(i, j, SDL_MapRGB(getImage()->format, (uint8_t)(ro*r), (uint8_t)(go*g), (uint8_t)(bo*b)), m_coloredimage);
        }

    Stretch(getImage()->w, getImage()->h, -1, true);
}
