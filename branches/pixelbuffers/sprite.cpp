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

#ifndef __APPLE__
#include <SDL/SDL_rotozoom.h> // library you need is called SDL_gfx
#else
#include <SDL_gfx/SDL_rotozoom.h>
#endif

#ifdef USE_OPENGL
#ifdef __APPLE__
	#include <OpenGL/gl.h>
#else
	#ifdef _WIN32
		#define WIN32_LEAN_AND_MEAN
		#include <windows.h>
	#endif
	#include <GL/gl.h>
#endif

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
#include "gamecontent/creature.h"
#include "product.h"

#pragma pack(1)
typedef struct {
	uint32_t signature;
	uint16_t imgcount;
} picfileheader_t;
typedef struct {
	uint8_t width, height;
	uint8_t unk1, unk2, unk3; // FIXME (ivucica#4#) zerocoolz says this should be colorkey, according to http://otfans.net/showpost.php?p=840634&postcount=134
} picpicheader_t;
#pragma pack()

#include "engine.h"


Sprite::Sprite(int w, int h, const oRGBA& c)
{
    // create blank sprite
    #ifdef USE_OPENGL
	m_pixelformat = GL_NONE;
	#endif
	m_image = NULL;
	m_stretchimage = NULL;
	m_coloredimage = NULL;
	m_loaded = false;
	m_smoothstretch = 0;
    m_r = 1.f;
    m_g = 1.f;
    m_b = 1.f;


    char spec[256];
    sprintf(spec, "%d %d %g %g %g %g.blank", w,h,c.r,c.g,c.b,c.a);


    m_filename = spec;
    m_index = 0;

    loadSurfaceFromFile(spec,0);

}
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

	if(!m_image)
	{
		return;
	}

	
	m_image->performCrop(x, y, w, h);
	
	m_coloredimage = createPixelBuffer(w,h,32);
	//SDL_CreateRGBSurface(SDL_HWSURFACE, m_image->w, m_image->h, 32, rmask, gmask, bmask, amask);
}



Sprite::~Sprite()
{
	if(m_image){
		delete (m_image);
	}
	if(m_stretchimage){
		delete (m_stretchimage);
	}
	if(m_coloredimage){
		delete (m_coloredimage);
	}

}


#include "util.h" // REMOVE ME

void Sprite::loadSurfaceFromFile(const std::string& filename, int index) {
	size_t extbegins = filename.rfind(".") + 1;
	std::string extension;
	if(extbegins != std::string::npos){
		extension = filename.substr(extbegins, filename.length() - extbegins);
	}


#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    uint32_t rmask = 0xff000000;
    uint32_t gmask = 0x00ff0000;
    uint32_t bmask = 0x0000ff00;
    uint32_t amask = 0x000000ff;
#else
    uint32_t rmask = 0x000000ff;
    uint32_t gmask = 0x0000ff00;
    uint32_t bmask = 0x00ff0000;
    uint32_t amask = 0xff000000;
#endif


	// print loading status (useful for slow machines such as wince; on the other hand, flipping too often causes slowness, too)
	#ifdef WINCE
		std::stringstream loadingtext;
		loadingtext << "Loading " << filename << "[" << index << "]...";
		if (g_engine && index) {
			g_engine->drawRectangle(0,0,240,10,oRGBA(0,0,0,255));
			g_engine->drawText(loadingtext.str().c_str(), "minifont", 0, 0, 0xFF);
			g_engine->Flip();
		}
	#endif

	if(extension == "bmp"){
		m_image = createPixelBuffer(SDL_LoadBMP(filename.c_str()));
		if(!m_image){
			DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_ERROR, "[Sprite::loadSurfaceFromFile] SDL_LoadBMP failed on file: %s\n", filename.c_str());
			return;
		}
		#ifdef USE_OPENGL
		m_pixelformat = GL_BGR;
		#endif
		m_loaded = true;
	}
	else if(extension == "spr"){
		uint32_t signature;
		uint16_t sprcount;
		uint32_t where;

		FILE *f = yatc_fopen(filename.c_str(), "rb");
		if(!f){
			DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY,DEBUGPRINT_ERROR,"[Sprite::loadSurfaceFromFile] Sprite file %s not found\n", filename.c_str());
			return;
		}


		if (index < 1) {
			if (index) DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY,DEBUGPRINT_ERROR, "[Sprite::loadSurfaceFromFile] Invalid index %d\n", index);
			fclose(f);
			goto loadFail;
		}

		yatc_fread(&signature, sizeof(signature), 1, f);
		yatc_fread(&sprcount, sizeof(sprcount), 1, f);
		if(index >= sprcount){// i can't do this, captain, there's not enough power
			DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_ERROR, "[Sprite::loadSurfaceFromFile] Loading spr index %d while we have %d sprites in file.\n", index, sprcount);
			goto loadFail; // she won't hold it much longer
		}

		// read the pointer to the real SPR data
		fseek(f, (index-1)*4, SEEK_CUR);
		yatc_fread(&where, sizeof(where), 1, f);

		// create surface where we'll store data, and fill it with transparency
		m_image = createPixelBuffer(32,32,32); //SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 32, 32, 32, rmask, gmask, bmask, amask);
		if(!m_image){
			DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY,DEBUGPRINT_ERROR, "[Sprite::loadSurfaceFromFile] Cant create SDL Surface.\n");
			goto loadFail;
		}
		SDL_LockSurface(m_image);

		// dont make static since if we change the rendering engine at runtime,
		//  this may change too
		Uint32 magenta = SDL_MapRGBA(m_image->format, 255, 0, 255, 255);
		SDL_FillRect(m_image, NULL, magenta);

		// read the data
        if (where) {
            fseek(f, where, SEEK_SET);

            fgetc(f); fgetc(f); fgetc(f); // FIXME (ivucica#4#) zerocoolz says this should be colorkey, according to http://otfans.net/showpost.php?p=840634&postcount=134
            if (readSprData(f, m_image, 0, 0)) {
                // error happened
                DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_ERROR, "[Sprite::loadSurfaceFromFile] Problem in readSprData()\n");
                SDL_FreeSurface(m_image);
                m_image = NULL;
                fclose(f);
                goto loadFail;
            }
        }
		fclose(f);
		SDL_UnlockSurface(m_image);
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


		f = yatc_fopen(filename.c_str(), "rb");
		if(!f){
			DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_ERROR, "[Sprite::loadSurfaceFromFile] Picture file %s not found\n", filename.c_str());
			goto loadFail;
		}

		yatc_fread(&fh, sizeof(fh), 1, f);

		for(int i = 0; i < fh.imgcount && i <= index ; i++) {
			yatc_fread(&ph, sizeof(ph), 1, f);

			if(i == index){
				s = SDL_CreateRGBSurface(SDL_SWSURFACE, ph.width*32, ph.height*32, 32, rmask, gmask, bmask, amask);
				SDL_LockSurface(s);
				if (!s) {
					DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_ERROR, "Failed to create surface of size %dx%d\n", ph.width*32, ph.height*32);
					fclose(f);
					goto loadFail;
				}
				magenta = SDL_MapRGB(s->format, 255, 0, 255);
				SDL_FillRect(s, NULL, magenta);

				for(int j = 0; j < ph.height; j++){
					for(int k = 0; k < ph.width; k++){
						yatc_fread(&sprloc, sizeof(sprloc), 1, f);

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
				SDL_UnlockSurface(s);
				break;
			}
			else{
				fseek(f, sizeof(sprloc)*ph.height*ph.width, SEEK_CUR);
			}
		}

		fclose(f);
		m_image = s;
		#ifdef USE_OPENGL
		m_pixelformat = GL_RGBA;
		#endif
		m_loaded = true;
	}
	else if(extension=="blank"){
	    double r,g,b,a;
	    int w,h;
	    sscanf(m_filename.c_str(), "%d %d %lg %lg %lg %lg", &w,&h,&r,&g,&b,&a);

        SDL_Surface *s = SDL_CreateRGBSurface(SDL_SWSURFACE, 256, 256, 32, rmask, gmask, bmask, amask);
        Uint32 color = SDL_MapRGB(s->format, (int)r, (int)g, (int)b);
		SDL_FillRect(s, NULL, color);

		#ifdef USE_OPENGL
		m_pixelformat = GL_RGBA;
		#endif
        m_image = s;
		m_loaded = true;

	}
	else{
		// m_image is already marked as NULL, so we're over
		return;
	}

	m_filename = filename;
	m_index = index;

	m_coloredimage = SDL_CreateRGBSurface(SDL_SWSURFACE, m_image->w, m_image->h, 32, rmask, gmask, bmask, amask);

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
    // DONT uncomment these lines, it IS possible to get null as parameter and it should be ignored
	if(!templatespr){
		printf("!templatespr\n");
		return;
	}
	if(!templatespr->getBasicImage()){
		return;
	}
	if(!m_image){
		printf("!m_image\n");
		return;
	}
	if(dynamic_cast<EngineGL*>(g_engine))
		// gl engine is crashing on SDL_UnlockSurface.
		return;
	templatespr->lockSurface();
	if(SDL_MUSTLOCK(m_image)) SDL_LockSurface(m_image);
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
				continue; // if nothing changed, skip the change of pixel
			}

			putPixel(j, i, SDL_MapRGB(m_image->format, ro, go, bo), m_image);
		}
		printf("\n");
	}
	if(SDL_MUSTLOCK(m_image)) SDL_UnlockSurface(m_image);
	templatespr->unlockSurface();

    rebuildSelf();
}

void Sprite::putPixel(int x, int y, uint32_t pixel, SDL_Surface *img)
{
	if (!img)
		img = m_image;

	if (!img->pixels) {
        //DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_WARNING, "Trying to write a pixel into a NIL array - %d, %d on a %dx%d image\n", x, y, img->w, img->h);
        return;
    }
	
	
	
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

    if (!img->pixels) {
        //DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_WARNING, "Trying to read a pixel from a NIL array - %d, %d on a %dx%d image\n", x, y, img->w, img->h);
        return 0;
    }
	/* Here p is the address to the pixel we want to retrieve */
	uint8_t *p = (uint8_t *)img->pixels + y * img->pitch + x * bpp;

	if (x >= img->w || y >= img->h)
		DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_WARNING, "Trying to read a pixel out of boundaries - %d, %d on a %dx%d image\n", x, y, img->w, img->h);


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
    if (m_stretchimage &&  w == m_stretchimage->w && h == m_stretchimage->h)
        return;

//    DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Stretching to %g %g\n", w, h);

	m_stretchimage = zoomSurface(img, w/img->w, h/img->h, smooth);

//	DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "New size: %d %d\n", m_stretchimage->w, m_stretchimage->h);

}

void Sprite::addColor(float r, float g, float b)
{
	register uint8_t ro, go, bo, ao;  // old rgba

	if (r > 1) r = 1;
	if (g > 1) g = 1;
	if (b > 1) b = 1;

	if(r == m_r && g == m_g && b == m_b){
		return;
	}
	SDL_LockSurface(m_image);
	SDL_LockSurface(m_coloredimage);

	for(register int i = 0; i < m_image->w; i++){
		for(register int j =0; j < m_image->h; j++){
		    if (!getBasicImage()) {
		        printf("I don't have an image!\n");
		    }
		    if (!getBasicImage()->pixels) {
		        printf("I don't have image's pixels!\n");
		    }
			SDL_GetRGBA(getPixel(i,j, getBasicImage()), getBasicImage()->format, &ro, &go, &bo, &ao);

			if(ao){
				putPixel(i, j, SDL_MapRGBA(m_coloredimage->format, (uint8_t)(ro*r), (uint8_t)(go*g), (uint8_t)(bo*b), ao), m_coloredimage);
			}
		}
	}

	SDL_UnlockSurface(m_image);
	SDL_UnlockSurface(m_coloredimage);
	m_r = r; m_g = g; m_b = b;
	Stretch(getImage()->w, getImage()->h, -1, true);
}

void Sprite::setAsIcon()
{
	#ifdef WINCE
	return; // sdl for wince doesnt uspport runtime icons anyway
	#endif
	if(!m_image) {
	    std::stringstream s;
	    s << "Did not find source image for setting the icon. Probably " << PRODUCTSHORT << " did not find '" << m_filename << "'.";
		NativeGUIError(s.str().c_str(), "Error");
	}

	SDL_WM_SetIcon(m_image, NULL);
}


SDL_Cursor* Sprite::createCursor(int topx, int topy, int w, int h, int hot_x, int hot_y)
{
  int i, row, col;
  Uint8 data[4*32];
  Uint8 mask[4*32];
  //int hot_x, hot_y;

  i = -1;
  for ( row=0; row<32; ++row ) {
    for ( col=0; col<32; ++col ) {
      if ( col % 8 ) {
        data[i] <<= 1;
        mask[i] <<= 1;
      } else {
        ++i;
        data[i] = mask[i] = 0;
      }

      if (col < w && row < h) {
        uint32_t pv;
        uint8_t r,g,b,a;

        SDL_GetRGBA(pv=getPixel(topx+col, topy+row, m_image), m_image->format, &r, &g, &b, &a);

        if(a == 0){
            // transparent
        } else if (r == 0 && g == 0 && b == 0) { // black
            data[i] |= 0x01;
            mask[i] |= 0x01;
        } else { // white
            mask[i] |= 0x01;
        }
      } else { // if out of boundaries, then leave

      }

    }
  }
  //sscanf(image[4+row], "%d,%d", &hot_x, &hot_y);
  //hot_x = 16;
  //hot_y = 16;

  return SDL_CreateCursor(data, mask, 32, 32, hot_x, hot_y);
}


SDL_Surface* Sprite::lockSurface()
{
	//printf("LOCK %p\n", m_image);
    if(SDL_MUSTLOCK(m_image)) 
		if(SDL_LockSurface(m_image)==-1)
			return NULL;
    return m_image;
}
void Sprite::unlockSurface()
{
	//printf("UNLOCK %p\n", m_image);
    if(SDL_MUSTLOCK(m_image)) SDL_UnlockSurface(m_image);
    Stretch(getWidth(), getHeight(),m_smoothstretch,1); // when unlocking
    rebuildSelf();
}
