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

/// \file enginesdl.cpp
/// Contains code for SDL rendering engine.
/// \sa EngineSDL

#include "enginesdl.h"

EngineSDL::EngineSDL()
{
	printf("Starting SDL engine\n");
	m_videoflags = SDL_HWSURFACE | SDL_ANYFORMAT | SDL_DOUBLEBUF | SDL_RESIZABLE;

	m_screen = SDL_SetVideoMode(m_width, m_height, m_video_bpp, m_videoflags);

	if(!m_screen){
		fprintf(stderr, "Could not set %dx%d video mode: %s\n", m_width, m_height, SDL_GetError()); // FIXME (ivucica#3#) Should report to user via msgbox
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


	m_sysfont->SetFontParam(createSprite("font.bmp"));
}

EngineSDL::~EngineSDL()
{
	SDL_FreeSurface(m_screen);
	delete (SpriteSDL*)m_sysfont->GetFontParam();
	printf("Closing SDL engine\n");
}

void EngineSDL::drawRectangle(float x, float y, float width, float height, oRGBA color)
{
	static const SDL_VideoInfo* vi = SDL_GetVideoInfo();
	SDL_Rect r={(int)x,(int)y,(int)width,(int)height};
	SDL_FillRect(m_screen, &r, SDL_MapRGBA(vi->vfmt, (uint8_t)color.r, (uint8_t)color.g, (uint8_t)color.b, (uint8_t)color.a));
}
