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


static void engine_font_drawchar(char t, SpriteSDL* img, int x1, int y1) {
	t -= 32;
    int x = (int)(t % 32)*16.;
    int y = (int)(t / 32)*16.;

    float w = 10;
    float h = 10;



	img->Blit(x1, y1, x, y, w, h);

}

static void engine_font_render(const char* txt, const void* font, float x, float y) {
	SpriteSDL *img = (SpriteSDL*)font;

	float cx=x*10,cy=y*10;

	volatile register float sizesofar = 0.;
	volatile register float linessofar = 0.;
	for (volatile register unsigned char *t = (unsigned char*)txt; *t; ++t) {
		switch (*t) {
			default:
				engine_font_drawchar(*t, img, cx, cy);
				cx += 10;
				sizesofar += 10;
				break;
			case '\n':
			case '\r':
				cx -= sizesofar;
				cy += 10;
				linessofar += 1.;
				sizesofar = 0;
				if (*t == '\n' && *(t+1)=='\r' || *t == '\r' && *(t+1)=='\n' ) t++;
		                break;

		}
	}

}
static float engine_font_size(const char* txt, const void* font) {
	return strlen(txt);
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
	sysfont->SetRenderFunc(engine_font_render);
	sysfont->SetSizeFunc(engine_font_size);
	sysfont->SetFontParam(createSprite("font.bmp"));


}

EngineSDL::~EngineSDL()
{
	printf("Closing SDL engine\n");
}

void EngineSDL::drawRectangle(float x, float y, float width, float height, oRGBA color)
{
	static const SDL_VideoInfo* vi = SDL_GetVideoInfo();
	SDL_Rect r={(int)x,(int)y,(int)width,(int)height};
	SDL_FillRect(screen, &r, SDL_MapRGBA(vi->vfmt, (unsigned char)color.r, (unsigned char)color.g, (unsigned char)color.b, (unsigned char)color.a));
}
