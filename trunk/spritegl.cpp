//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
// Sprite for OpenGL engine
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

#ifdef USE_OPENGL

#include <SDL/SDL_opengl.h>

#include "spritegl.h"
#include "util.h"
#include "debugprint.h"
#include "engine.h"

extern bool g_running;

SpriteGL::SpriteGL(const std::string& filename, int index) :
Sprite(filename, index)
{
    m_texture = GL_INVALID_VALUE;
	buildGLTexture();
}

SpriteGL::~SpriteGL()
{
    destroyGLTexture();
}

void SpriteGL::addColor(float r, float g, float b)
{
    m_r = b;
    m_g = g;
    m_b = r;
}

void SpriteGL::buildGLTexture() {
    //Do not continue if the image isnt loaded
	if(!getImage())
		return;

    destroyGLTexture();

    m_engineCreationTimestamp = 0;

    if(!g_running)
        return;

	//m_multiplierx = nextpow(this->getWidth()) / this->getWidth();
	//m_multipliery = nextpow(this->getHeight()) / this->getHeight();

	//Stretch(nextpow(this->getWidth()), nextpow(this->getHeight()), true);
	//printf("Stretch %p\n", this);
    printf(".");
	m_multiplierx = 1.;
	m_multipliery = 1.;


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


	SDL_Surface *sfc = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, nextpow(getImage()->w), nextpow(getImage()->h), 32, rmask, gmask, bmask, amask);

	SDL_Rect s = {0,0,getImage()->w,getImage()->h};
	SDL_Rect d = {0,0,getImage()->w,getImage()->h};

	SDL_BlitSurface(getImage(), &s, sfc, &d);

	SDL_SetAlpha(sfc, SDL_SRCALPHA, 255);
	SDL_LockSurface(sfc);

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	GLint ret = gluBuild2DMipmaps(GL_TEXTURE_2D,
						GL_RGBA,
						sfc->w, sfc->h,
						(getPixelFormat() == GL_NONE ? (DEBUGPRINT(DEBUGPRINT_WARNING, DEBUGPRINT_LEVEL_OBLIGATORY, "[SpriteGL::SpriteGL] Invalid pixelformat\n"), GL_BGRA) : getPixelFormat()),
						GL_UNSIGNED_BYTE,
			 			sfc->pixels);

	if(ret != 0){
		DEBUGPRINT(DEBUGPRINT_ERROR, DEBUGPRINT_LEVEL_OBLIGATORY, "Error [SpriteGL::SpriteGL] Cant build 2DMipmaps: %s\n", gluErrorString(ret));
	}

	glDisable(GL_TEXTURE_2D);
	SDL_UnlockSurface(sfc);
	SDL_FreeSurface(sfc);

    m_engineCreationTimestamp = g_engine->m_creationTimestamp;
}

void SpriteGL::Blit(float destx, float desty, float srcx, float srcy, float srcwidth, float srcheight)
{
	Blit(destx, desty, srcx, srcy, srcwidth, srcheight, srcwidth, srcheight);
}

void SpriteGL::Blit(float destx, float desty, float srcx, float srcy, float srcwidth, float srcheight, float destwidth, float destheight)
{
	if(!getImage())
		return;

    if (m_engineCreationTimestamp != g_engine->m_creationTimestamp) { // we need to recreate the texture since the context was invalidated in the meantime
        buildGLTexture();
    }
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, m_texture);

	double spriteWidth = nextpow(getWidth())  / m_multiplierx ;
	double spriteHeight = nextpow(getHeight())  / m_multipliery;

	//glAlphaFunc(GL_GEQUAL, .80);
	//glEnable(GL_ALPHA_TEST);
    if(m_r != 1. || m_g != 1. || m_b != 1.)
        glColor4f(m_r, m_g, m_b, 1.);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBegin(GL_QUADS);
		glTexCoord2d((srcx)/spriteWidth, (srcy)/spriteHeight);
		glVertex2d(destx, desty);

		glTexCoord2d((srcx)/spriteWidth, (srcy + (srcheight) / m_multipliery )/spriteHeight);
		glVertex2d(destx, desty + destheight);

		glTexCoord2d((srcx + (srcwidth) / m_multiplierx)/spriteWidth, (srcy + (srcheight) / m_multipliery)/spriteHeight);
		glVertex2d(destx + destwidth, desty + destheight);

		glTexCoord2d((srcx + (srcwidth) / m_multiplierx)/spriteWidth, (srcy) /spriteHeight);
		glVertex2d(destx + destwidth, desty);
	glEnd();

    if(m_r != 1. || m_g != 1. || m_b != 1.)
        glColor4f(1.,1.,1., 1.);

	glPopMatrix();
}
void SpriteGL::rebuildSelf()
{
    destroyGLTexture();
    buildGLTexture();
}
void SpriteGL::destroyGLTexture()
{
    if(m_texture != GL_INVALID_VALUE){
		glDeleteTextures(1, &m_texture);
		m_texture = GL_INVALID_VALUE;
	}
}
#endif

