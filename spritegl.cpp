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

#include <GL/gl.h>
#include <GL/glu.h>
#ifdef WIN32
	#include <GL/glext.h> // TODO (ivucica#1#) ivucica asks: do we really need this include?
#endif
#include "spritegl.h"
#include "util.h"
#include "debugprint.h"
#include "engine.h"

extern bool g_running;

SpriteGL::SpriteGL(const std::string& filename, int index) :
Sprite(filename, index)
{
	buildGLTexture();
}

SpriteGL::~SpriteGL()
{
	if(m_texture != GL_INVALID_VALUE){
		glDeleteTextures(1, &m_texture);
	}
}

void SpriteGL::addColor(float r, float g, float b)
{
    m_r = r;
    m_g = g;
    m_b = b;
}

void SpriteGL::buildGLTexture() {
    //Do not continue if the image isnt loaded
	if(!getImage())
		return;

    m_engineCreationTimestamp = 0;

    if(!g_running)
        return;

	//m_multiplierx = nextpow(this->getWidth()) / this->getWidth();
	//m_multipliery = nextpow(this->getHeight()) / this->getHeight();

	//Stretch(nextpow(this->getWidth()), nextpow(this->getHeight()), true);
	//printf("Stretch %p\n", this);

	m_multiplierx = 1.;
	m_multipliery = 1.;

	SDL_LockSurface(getImage());

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	GLint ret = gluBuild2DMipmaps(GL_TEXTURE_2D,
						GL_RGBA,
						getImage()->w, getImage()->h,
						(getPixelFormat() == GL_NONE ? (DEBUGPRINT(DEBUGPRINT_WARNING, DEBUGPRINT_LEVEL_OBLIGATORY, "[SpriteGL::SpriteGL] Invalid pixelformat\n"), GL_BGRA) : getPixelFormat()),
						GL_UNSIGNED_BYTE,
			 			getImage()->pixels);

	if(ret != 0){
		DEBUGPRINT(DEBUGPRINT_ERROR, DEBUGPRINT_LEVEL_OBLIGATORY, "Error [SpriteGL::SpriteGL] Cant build 2DMipmaps: %s\n", gluErrorString(ret));
	}

	glDisable(GL_TEXTURE_2D);
	SDL_UnlockSurface(getImage());

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

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glBindTexture(GL_TEXTURE_2D, m_texture);

	float spriteWidth = getWidth()  / m_multiplierx ;
	float spriteHeight = getHeight()  / m_multipliery;

	glAlphaFunc(GL_GEQUAL, .80);
	glEnable(GL_ALPHA_TEST);
	//if (m_index==0)
		//printf("File: %s Index: %d Multipliers: %g %g WxH: %gx%g\n", m_filename.c_str(), m_index, m_multiplierx, m_multipliery, spriteWidth, spriteHeight);
    if(m_r != 1. || m_g != 1. || m_b != 1.)
        glColor4f(m_r, m_g, m_b, 1.);

	glBegin(GL_QUADS);
		glTexCoord2f((srcx)/spriteWidth, (srcy )/spriteHeight);
		glVertex2f(destx , desty );

		glTexCoord2f((srcx)/spriteWidth, (srcy + srcheight / m_multipliery)/spriteHeight);
		glVertex2f(destx , desty + destheight);

		glTexCoord2f((srcx + srcwidth / m_multiplierx)/spriteWidth, (srcy + srcheight / m_multipliery)/spriteHeight);
		glVertex2f(destx + destwidth, desty + destheight);

		glTexCoord2f((srcx + srcwidth / m_multiplierx)/spriteWidth, (srcy) /spriteHeight);
		glVertex2f(destx + destwidth, desty);
	glEnd();

    if(m_r != 1. || m_g != 1. || m_b != 1.)
        glColor4f(1.,1.,1., 1.);

	glPopMatrix();
}


#endif
