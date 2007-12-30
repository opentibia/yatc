//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
// OpenGL engine
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

/// \file enginegl.cpp
///
/// Contains the code for OpenGL rendering engine.
///
/// \sa EngineGL

#ifdef USE_OPENGL

#include <GL/gl.h>
#include <GLICT/globals.h>
#include "options.h"
#include "enginegl.h"
#include "font.h"
EngineGL::EngineGL()
{
	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Starting OpenGL engine\n");
	m_screen = NULL;

	doResize(m_width, m_height);

	if(!m_screen)
		// looks like GL is not supported -- dont attempt anything else
		return;

	glictGlobals.drawPartialOut = true;
	glictGlobals.clippingMode = GLICT_SCISSORTEST;

	// FIXME (ivucica#2#) we should save the Font* somewhere so we can delete; it... perhaps glictFont() should have a GetFontParam
	m_sysfont->SetFontParam(new Font("Tibia.pic", 2, createSprite("Tibia.pic", 2)));
	m_minifont->SetFontParam(new Font("Tibia.pic", 5, createSprite("Tibia.pic", 5)));
	m_aafont->SetFontParam(new Font("Tibia.pic", 7, createSprite("Tibia.pic", 7)));
	m_gamefont->SetFontParam(new Font("Tibia.pic", 4, createSprite("Tibia.pic", 4)));

	initEngine();

}

EngineGL::~EngineGL()
{
	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Closing OpenGL engine\n");
	delete (Font*)m_sysfont->GetFontParam();
}

void EngineGL::initEngine()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void EngineGL::doResize(int w, int h)
{
	Engine::doResize(w, h);

	if (m_screen) SDL_FreeSurface(m_screen);
	m_videoflags = SDL_OPENGL | SDL_RESIZABLE;

	m_creationTimestamp = time(NULL);

	if (options.fullscreen)
		m_videoflags |= SDL_FULLSCREEN;

	m_screen = SDL_SetVideoMode(m_width, m_height, m_video_bpp, m_videoflags);

	if(!m_screen){
		fprintf(stderr, "Could not set %dx%d video mode: %s\n", m_width, m_height, SDL_GetError());
		return;
	}

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, w, h, 0.0f, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void EngineGL::drawRectangle(float x, float y, float width, float height, oRGBA color)
{
	glDisable(GL_TEXTURE_2D);

	glColor4f(color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f);

	glRectf(x, y, x+width, y+height);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void EngineGL::drawRectangleLines(float x, float y, float width, float height, oRGBA color)
{
	//TODO
}

bool EngineGL::isSupported()
{
	if (!m_screen)
		return false;
	else
		return true;
	/*uint32_t vf = SDL_OPENGL | SDL_RESIZABLE;

	SDL_Surface *s = SDL_SetVideoMode(m_width, m_height, m_video_bpp, m_videoflags);
	if (s) {
		SDL_FreeSurface(s);
		return true;
	} else
		return false;*/
}
#endif
