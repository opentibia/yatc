//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
// SDL engine
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

/// \file enginesdl.cpp
/// Contains code for SDL rendering engine.
/// \sa EngineSDL

#include <GLICT/globals.h>
#include "enginesdl.h"
#include "font.h"
#include "options.h"
#include "util.h"
EngineSDL::EngineSDL()
{
	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Starting SDL engine\n");

	m_screen = NULL;
	doResize(m_width, m_height);

	//glictGlobals.drawPartialOut = false;

    Sprite*a,*b;
	// FIXME (ivucica#2#) we should save the Font* somewhere so we can delete; it... perhaps glictFont() should have a GetFontParam


	m_sysfont->SetFontParam(new Font("Tibia.pic", 2, a=createSprite("Tibia.pic", 2)));
	m_minifont->SetFontParam(new Font("Tibia.pic", 5, createSprite("Tibia.pic", 5)));
	m_aafont->SetFontParam(new Font("Tibia.pic", 7, b=createSprite("Tibia.pic", 7)));
	m_gamefont->SetFontParam(new Font("Tibia.pic", 4, createSprite("Tibia.pic", 4)));

	a->addColor(.75,.75,.75);
	b->addColor(.75,.75,.75);
}

EngineSDL::~EngineSDL()
{
	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Closing SDL engine\n");
	delete (Font*)m_sysfont->GetFontParam();
	delete (Font*)m_minifont->GetFontParam();
	delete (Font*)m_aafont->GetFontParam();
	delete (Font*)m_gamefont->GetFontParam();
}

void EngineSDL::drawRectangle(float x, float y, float width, float height, oRGBA color)
{
	static const SDL_VideoInfo* vi = SDL_GetVideoInfo();
	SDL_Rect r={(int)x,(int)y,(int)width,(int)height};
	SDL_FillRect(m_screen, &r, SDL_MapRGBA(vi->vfmt, (uint8_t)color.r, (uint8_t)color.g, (uint8_t)color.b, (uint8_t)color.a));
}

void EngineSDL::drawRectangleLines(float x, float y, float width, float height, oRGBA color)
{
	const SDL_VideoInfo* vi = SDL_GetVideoInfo();
	int col = SDL_MapRGBA(vi->vfmt, (int)(color.r), (int)(color.g), (int)(color.b), (int)(color.a));
	{
		SDL_Rect rect = {(int)x, (int)y, 1, (int)height};
		SDL_FillRect(m_screen, &rect, col);
	}

	{
		SDL_Rect rect = {(int)(x+width-1.f), (int)y, 1, (int)height};
		SDL_FillRect(m_screen, &rect, col);
	}

	{
		SDL_Rect rect = {(int)x, (int)y, (int)width, 1};
		SDL_FillRect(m_screen, &rect, col);
	}

	{
		SDL_Rect rect = {(int)x, (int)(y+height-1.f), (int)width, 1};
		SDL_FillRect(m_screen, &rect, col);
	}
}

void EngineSDL::doResize(int w, int h)
{
	Engine::doResize(w, h);

	m_videoflags = SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE;
	if (options.fullscreen)
		m_videoflags |= SDL_FULLSCREEN;

	m_screen = SDL_SetVideoMode(m_width, m_height, m_video_bpp, m_videoflags);

	if(!m_screen){
		char tmperr[255];
		sprintf(tmperr, "Could not set %dx%d video mode: %s\n", m_width, m_height, SDL_GetError()); // DONE (ivucica#3#) Should report to user via msgbox
		NativeGUIError(tmperr, "YATC SDL Engine Failed");
		exit(1); // this is perfectly valid, since it's really not believable that any other engine would be supported in case SDL fails to init
	}

	if(m_screen->format->BitsPerPixel == 8){
		// 256 colors? go grayscale, go!
		SDL_Color colors[256];
		for (int i = 0 ; i < 256; i++) {
			colors[i].r = i; colors[i].g = i; colors[i].b = i;
		}

		SDL_SetColors(m_screen, colors, 0, sizeof(colors) / sizeof(SDL_Color));
	}

}
