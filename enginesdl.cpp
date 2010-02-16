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
#include "product.h"
#ifndef __APPLE__
#include <SDL/SDL_gfxPrimitives.h>
#else
#include <SDL_gfx/SDL_gfxPrimitives.h>
#endif
EngineSDL::EngineSDL()
{
	DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Starting SDL engine\n");

	m_screen = NULL;

    SDL_ShowCursor(0);
	doResize(m_width, m_height);

	// note (nfries88): On Windows, if you maximize yatc and leave it maximized when it closes, it'll restart
	// at maximized size but at a random position in the on the screen, which is unsightly and annoying.
	// This should be fixed in a better way (perhaps before saving window dimensions to yatc.cfg) later on.
	#ifdef WIN32
	MoveWindow(GetActiveWindow(), 0, 0, m_width, m_height, true);
	#endif

	//glictGlobals.drawPartialOut = false;
#if (GLICT_APIREV >= 79)
	glictGlobals.mayUseGL = false;
#else
#warning To disable any hanging GL usage in GLICT when using SDL, you need GLICT APIREV 79+. Required for Mac.
#endif

    Sprite*a,*b;

	m_sysfont->SetFontParam(new YATCFont("Tibia.pic", 2, a=createSprite("Tibia.pic", 2)));
	m_minifont->SetFontParam(new YATCFont("Tibia.pic", 5, createSprite("Tibia.pic", 5)));
	m_aafont->SetFontParam(new YATCFont("Tibia.pic", 7, b=createSprite("Tibia.pic", 7)));
	m_gamefont->SetFontParam(new YATCFont("Tibia.pic", 4, createSprite("Tibia.pic", 4)));

	a->addColor(.75,.75,.75);
	b->addColor(.75,.75,.75);


    m_ui = createSprite("Tibia.pic", 3);
	m_light = createSprite("Tibia.pic", 6);
    m_cursorBasic = m_ui->createCursor(290,12,11,19, 1, 1);
    m_cursorUse = m_ui->createCursor(310,12,19,19, 9, 9);
    SDL_ShowCursor(1);
    SDL_SetCursor(m_cursorBasic);

    #if (GLICT_APIREV >= 64)
	glictGlobals.clipperCallback = EngineSDL::clipper_func;
	#else
	#warning Please upgrade GLICT to api revision 64 or greater to fix clipping problem
	#endif
}

EngineSDL::~EngineSDL()
{
	DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Closing SDL engine\n");
	delete (YATCFont*)m_sysfont->GetFontParam();
	delete (YATCFont*)m_minifont->GetFontParam();
	delete (YATCFont*)m_aafont->GetFontParam();
	delete (YATCFont*)m_gamefont->GetFontParam();
}

void EngineSDL::drawLightmap(vertex* lightmap, int type, int width, int height, int scale)
{
	// Just draw the basic light.
	Engine::drawLightmap(lightmap, type, width, height, scale);
}

void EngineSDL::drawRectangle(float x, float y, float width, float height, oRGBA color)
{
	static const SDL_VideoInfo* vi = SDL_GetVideoInfo();
	SDL_Rect r={(int)x,(int)y,(int)width,(int)height};
	if(color.a != 255) {
	    // draw with alpha channel
        boxRGBA(m_screen, x, y, x + width, y + height, (uint8_t)color.r, (uint8_t)color.g, (uint8_t)color.b, (uint8_t)(color.a));
	} else {
	    // draw without alpha channel
	    SDL_FillRect(m_screen, &r, SDL_MapRGBA(vi->vfmt, (uint8_t)color.r, (uint8_t)color.g, (uint8_t)color.b, (uint8_t)color.a));
	}
}

void EngineSDL::drawRectangleLines(float x, float y, float width, float height, oRGBA color, float thickness /*= 1.f*/)
{
	const SDL_VideoInfo* vi = SDL_GetVideoInfo();
	int col = SDL_MapRGBA(vi->vfmt, (int)(color.r), (int)(color.g), (int)(color.b), (int)(color.a));
	{
		SDL_Rect rect = {(int)x, (int)y, (int)thickness, (int)height};
		SDL_FillRect(m_screen, &rect, col);
	}

	{
		SDL_Rect rect = {(int)(x+width-thickness), (int)y, (int)thickness, (int)height};
		SDL_FillRect(m_screen, &rect, col);
	}

	{
		SDL_Rect rect = {(int)x, (int)y, (int)width, (int)thickness};
		SDL_FillRect(m_screen, &rect, col);
	}

	{
		SDL_Rect rect = {(int)x, (int)(y+height-thickness), (int)width, (int)thickness};
		SDL_FillRect(m_screen, &rect, col);
	}
}

void EngineSDL::doResize(int& w, int& h)
{
	Engine::doResize(w, h);

	m_videoflags = SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE;
	if (options.fullscreen)
		m_videoflags |= SDL_FULLSCREEN;
#ifdef WINCE
	m_videoflags |= SDL_FULLSCREEN;
#endif

	m_screen = SDL_SetVideoMode(m_width, m_height, m_video_bpp, m_videoflags);

	if(!m_screen){
		char tmperr[255];
		sprintf(tmperr, "Could not set %dx%d video mode: %s\n", m_width, m_height, SDL_GetError()); // DONE (ivucica#3#) Should report to user via msgbox
		NativeGUIError(tmperr, PRODUCTSHORT " SDL Engine Failed");
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



void EngineSDL::clipper_func(float left, float right, float top, float bottom) {
    SDL_Rect r = {left,top, right-left, bottom-top};
    SDL_SetClipRect(((EngineSDL*)g_engine)->m_screen, &r);
}
