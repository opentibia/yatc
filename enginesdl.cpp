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


// first define callbacks, and after that go on with class

static void enginesdl_font_render(const char* txt, const void* font, float x, float y) {
}
static float enginesdl_font_size(const char* txt, const void* font) {
	return 0;
}

EngineSDL::EngineSDL()
{
	printf("Starting SDL engine\n");
	videoflags = SDL_HWSURFACE|SDL_ANYFORMAT | SDL_DOUBLEBUF | SDL_RESIZABLE;
	width = 640;
	height = 480;
	video_bpp = 8;

	screen = SDL_SetVideoMode(width, height, video_bpp, videoflags);

	if (!screen){
		fprintf(stderr, "Could not set %dx%d video mode: %s\n", width, height, SDL_GetError()); // FIXME (Khaos#3#) Should report to user via msgbox
		exit(1); // this is perfectly valid, since it's really not believable that any other engine would be supported in case SDL fails to init
	}



	sysfont = glictCreateFont("system");
	sysfont->SetRenderFunc(enginesdl_font_render);
	sysfont->SetSizeFunc(enginesdl_font_size);

}

EngineSDL::~EngineSDL()
{
	printf("Closing SDL engine\n");
	glictDeleteFont ("system");
}

void EngineSDL::drawRectangle(float x, float y, float width, float height, oRGBA color)
{
	static const SDL_VideoInfo* vi = SDL_GetVideoInfo();
	SDL_Rect r={(int)x,(int)y,(int)width,(int)height};
	SDL_FillRect(screen, &r, SDL_MapRGBA(vi->vfmt, (unsigned char)color.r, (unsigned char)color.g, (unsigned char)color.b, (unsigned char)color.a));
}
