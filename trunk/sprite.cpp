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

#if defined WIN32 || defined WINDOWS
#include <GL/glext.h>
#endif

#ifndef GL_BGR
#warning Odd OpenGL header. Defining GL_BGR to a 0.
#define GL_BGR 0
#endif

#endif
#include <sstream>
#include "debugprint.h"

#include "sprite.h"
#include "sprdata.h"
#include <math.h>
#include "wince.h"
#include "gamecontent/creature.h"

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

#include "engine.h"


Sprite::Sprite(const std::string& filename, int index)
{
	#ifdef USE_OPENGL
	m_pixelformat = GL_NONE;
	#endif
	m_image = NULL;
	m_stretchimage = NULL;
	m_coloredimage = NULL;
	m_loaded = false;
	m_smoothstretch = 0;
    m_filename = filename;
    m_index = index;
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
	m_coloredimage = NULL;
	m_loaded = false;
	m_smoothstretch = 1;
	m_r = 1.f;
    m_g = 1.f;
    m_b = 1.f;

	loadSurfaceFromFile(filename, index);

	if(!m_image){
		return;
	}

	SDL_Surface* ns = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 0xFF, 0xFF00, 0xFF0000, 0xFF000000);
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


#include "util.h" // REMOVE ME

void Sprite::loadSurfaceFromFile(const std::string& filename, int index) {
	size_t extbegins = filename.rfind(".") + 1;
	std::string extension;
	if(extbegins != std::string::npos){
		extension = filename.substr(extbegins, filename.length() - extbegins);
	}


	// print loading status (useful for slow machines such as wince; on the other hand, flipping too often causes slowness, too)
	#ifdef WINCE
		std::stringstream loadingtext;
		loadingtext << "Loading " << filename << "[" << index << "]...";
		if (g_engine && index) {
			g_engine->drawRectangle(0,0,240,10,oRGBA(0,0,0,1));
			g_engine->drawText(loadingtext.str().c_str(), "minifont", 0, 0, 0xFF);
			g_engine->Flip();
		}
	#endif


	if(extension == "bmp"){
		m_image = SDL_LoadBMP(filename.c_str());
		if(!m_image){
			DEBUGPRINT(DEBUGPRINT_ERROR, DEBUGPRINT_LEVEL_OBLIGATORY, "[Sprite::loadSurfaceFromFile] SDL_LoadBMP file: %s\n", filename.c_str());
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
			DEBUGPRINT(DEBUGPRINT_ERROR, DEBUGPRINT_LEVEL_OBLIGATORY,"[Sprite::loadSurfaceFromFile] Sprite file %s not found\n", filename.c_str());
			return;
		}

		if (index < 1) {
			DEBUGPRINT(DEBUGPRINT_ERROR, DEBUGPRINT_LEVEL_OBLIGATORY,"[Sprite::loadSurfaceFromFile] Invalid index %d\n", index);
			fclose(f);
			goto loadFail;
		}

		fread(&signature, sizeof(signature), 1, f);
		fread(&sprcount, sizeof(sprcount), 1, f);
		if(index >= sprcount){// i can't do this, captain, there's not enough power
			DEBUGPRINT(DEBUGPRINT_ERROR, DEBUGPRINT_LEVEL_OBLIGATORY, "[Sprite::loadSurfaceFromFile] Loading spr index %d while we have %d sprites in file.\n", index, sprcount);
			goto loadFail; // she won't hold it much longer
		}

		// read the pointer to the real SPR data
		fseek(f, (index-1)*4, SEEK_CUR);
		fread(&where, sizeof(where), 1, f);

		// create surface where we'll store data, and fill it with transparency
		m_image = SDL_CreateRGBSurface(SDL_SWSURFACE, 32, 32, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000); // FIXME (ivucica#5#) Potentially unportable to architectures with different endianess, take a look at SDL docs and make all Liliputtans happy
		if(!m_image){
			DEBUGPRINT(DEBUGPRINT_ERROR, DEBUGPRINT_LEVEL_OBLIGATORY,"[Sprite::loadSurfaceFromFile] Cant create SDL Surface.\n");
			goto loadFail;
		}

		// dont make static since if we change the rendering engine at runtime,
		//  this may change too
		Uint32 magenta = SDL_MapRGB(m_image->format, 255, 0, 255);
		SDL_FillRect(m_image, NULL, magenta);

		// read the data
        if (where) {
            fseek(f, where, SEEK_SET);

            fgetc(f); fgetc(f); fgetc(f); // TODO (ivucica#4#) maybe we should figure out what do these do?
            if (readSprData(f, m_image, 0, 0)) {
                // error happened
                DEBUGPRINT(DEBUGPRINT_ERROR, DEBUGPRINT_LEVEL_OBLIGATORY, "[Sprite::loadSurfaceFromFile] Problem in readSprData()\n");
                SDL_FreeSurface(m_image);
                m_image = NULL;
                fclose(f);
                goto loadFail;
            }
        }
		fclose(f);
		SDL_UpdateRect(m_image, 0, 0, 32, 32);

		#ifdef USE_OPENGL
		m_pixelformat = GL_BGRA;
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
			DEBUGPRINT(DEBUGPRINT_ERROR, DEBUGPRINT_LEVEL_OBLIGATORY, "[Sprite::loadSurfaceFromFile] Picture file %s not found\n", filename.c_str());
			goto loadFail;
		}

		fread(&fh, sizeof(fh), 1, f);

		for(int i = 0; i < fh.imgcount && i <= index ; i++) {
			fread(&ph, sizeof(ph), 1, f);

			if(i == index){
				s = SDL_CreateRGBSurface(SDL_SWSURFACE, ph.width*32, ph.height*32, 32, 0xFF, 0xFF00, 0xFF0000, 0xFF000000);
				if (!s) {
					DEBUGPRINT(DEBUGPRINT_ERROR, DEBUGPRINT_LEVEL_OBLIGATORY, "Failed to create surface of size %dx%d\n", ph.width*32, ph.height*32);
					fclose(f);
					goto loadFail;
				}
				magenta = SDL_MapRGB(s->format, 255, 0, 255);
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
							goto loadFail;
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
		#ifdef USE_OPENGL
		m_pixelformat = GL_BGRA;
		#endif
		m_loaded = true;
	}
	else{
		// m_image is already marked as NULL, so we're over
		return;
	}

	m_filename = filename;
	m_index = index;

	m_coloredimage = SDL_CreateRGBSurface(SDL_SWSURFACE, m_image->w, m_image->h, 32, 0xFF, 0xFF00, 0xFF0000, 0xFF000000);

	SDL_SetColorKey(m_image, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(m_image->format, 0xFF, 0, 0xFF)); // magenta is transparent


	{
		SDL_Surface *ns=SDL_DisplayFormatAlpha(m_image);
		if (ns) {
			SDL_FreeSurface(m_image);
			m_image=ns;
		}
	}



	return;


loadFail:
	return;
}
void Sprite::templatedColorizePixel(uint8_t color, uint8_t &r, uint8_t &g, uint8_t &b)
{
	uint8_t ro = (Creatures::OutfitLookupTable[color] & 0xFF0000) >> 16; // rgb outfit
	uint8_t go = (Creatures::OutfitLookupTable[color] & 0xFF00) >> 8;
	uint8_t bo = (Creatures::OutfitLookupTable[color] & 0xFF);
	r = (uint8_t)(r * (ro / 255.f));
    g = (uint8_t)(g * (go / 255.f));
    b = (uint8_t)(b * (bo / 255.f));
}
void Sprite::templatedColorize(Sprite* templatespr, uint8_t head, uint8_t body, uint8_t legs, uint8_t feet)
{
	if(!templatespr){
		printf("!templatespr\n");
		return;
	}
	if(!templatespr->getBasicImage()){
		printf("!templatespr->getBasicImage()\n");
		return;
	}
	if(!m_image){
		printf("!m_image\n");
		return;
	}
	for(int i=0; i < m_image->h; i++){
		for(int j=0; j < m_image->w; j++){
			uint32_t pixel = getPixel(j,i,m_image);
			uint32_t templatepixel = getPixel(j,i,templatespr->getBasicImage());
			uint8_t rt, gt, bt; // rgb template
			uint8_t ro, go, bo; // rgb original

			SDL_GetRGB(templatepixel, templatespr->getBasicImage()->format, &rt, &gt, &bt);
			SDL_GetRGB(pixel, m_image->format, &ro, &go, &bo);

			if(rt && gt && !bt){ // yellow == head
				templatedColorizePixel(head, ro, go, bo);
			}
			else if (rt && !gt && !bt){ // red == body
				templatedColorizePixel(body, ro, go, bo);
			}
			else if (!rt && gt && !bt){ // green == legs
				templatedColorizePixel(legs, ro, go, bo);
			}
			else if (!rt && !gt && bt){ // blue == feet
				templatedColorizePixel(feet, ro, go, bo);
			}
			else{
				continue; // to if nothing changed, skip the change of pixel
			}

			putPixel(j, i, SDL_MapRGB(getBasicImage()->format, ro, go, bo), m_image);
		}
	}

}

void Sprite::putPixel(int x, int y, uint32_t pixel, SDL_Surface *img)
{
	if (!img)
        img = m_image;
	int bpp = img->format->BytesPerPixel;

	uint8_t *p = (uint8_t *)img->pixels + y * img->pitch + x * bpp;

	if (x >= img->w || y >= img->h)
    		DEBUGPRINT(DEBUGPRINT_WARNING, DEBUGPRINT_LEVEL_OBLIGATORY, "Trying to write a pixel out of boundaries - %d, %d on a %dx%d image\n", x, y, img->w, img->h);

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

	if (x >= img->w || y >= img->h)
		DEBUGPRINT(DEBUGPRINT_WARNING, DEBUGPRINT_LEVEL_OBLIGATORY, "Trying to read a pixel out of boundaries - %d, %d on a %dx%d image\n", x, y, img->w, img->h);


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
	SDL_Surface* img;
	if(m_stretchimage && !force){
		if(fabs(m_stretchimage->w - w) < 2.f && fabs(m_stretchimage->h - h) < 2.f){
			return;
		}
	}

    if(m_r == 1.f && m_g == 1.f && m_b == 1.f){
        img = m_image;
    }
    else{
        img = m_coloredimage;
    }

    if(smooth == -1){
        smooth = m_smoothstretch;
    }
    else{
        m_smoothstretch = smooth;
    }


	unStretch();

	if (w == m_image->w && h == m_image->h)
        return;

	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Stretching to %g %g\n", w, h);
	m_stretchimage = zoomSurface(img, w/img->w, h/img->h, smooth);

	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "New size: %d %d\n", m_stretchimage->w, m_stretchimage->h);

}

void Sprite::addColor(float r, float g, float b)
{
    uint8_t ro, go, bo, ao;  // old rgba
    uint32_t pv; // pixel value
    if(r == m_r && g == m_g && b == m_b){
		return;
    }
    for(int i=0; i < m_image->w; i++){
        for (int j=0; j < m_image->h; j++){
            SDL_GetRGBA(pv=getPixel(i,j, getImage()), getImage()->format, &ro, &go, &bo, &ao);
			if(ao){
                putPixel(i, j, SDL_MapRGBA(getImage()->format, (uint8_t)(ro*r), (uint8_t)(go*g), (uint8_t)(bo*b), ao), m_coloredimage);
			}
        }
    }
    m_r = r; m_g = g; m_b = b;
    Stretch(getImage()->w, getImage()->h, -1, true);
}

#include "util.h" // REMOVE ME

void Sprite::setAsIcon()
{
	#ifndef WINCE
	return; // sdl for wince doesnt uspport runtime icons anyway
	#endif
	if(!m_image)
		NativeGUIError("Issue","Yes");

	SDL_WM_SetIcon(m_image,NULL);
}
