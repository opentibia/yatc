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

/// \file enginegl.cpp
///
/// Contains the code for OpenGL rendering engine.
///
/// \sa EngineGL

#include <GL/gl.h>
#include "enginegl.h"


static void engine_font_drawchar(char t, SpriteGL* img, int x1, int y1) {
	t -= 32;
    int x = (int)(t % 32)*16.;
    int y = (int)(t / 32)*16.;

    int w = 10;
    int h = 10;



	img->Blit(x1, y1, x, y, w, h);

}

static void engine_font_render(const char* txt, const void* font, float x, float y) {
	SpriteGL *img = (SpriteGL*)font;

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



EngineGL::EngineGL()
{
	printf("Starting OpenGL engine\n");
	videoflags = SDL_OPENGL | SDL_RESIZABLE;
	width = 640;
	height = 480;
	video_bpp = 8;

	screen = SDL_SetVideoMode(width, height, video_bpp, videoflags);

	if(!screen){
		fprintf(stderr, "Could not set %dx%d video mode: %s\n", width, height, SDL_GetError());
		exit(1);
	}



	sysfont = glictCreateFont("system");
	sysfont->SetRenderFunc(engine_font_render);
	sysfont->SetSizeFunc(engine_font_size);
	sysfont->SetFontParam(createSprite("font.bmp"));

	initEngine();
	doResize(width, height);

}

EngineGL::~EngineGL()
{
	printf("Closing OpenGL engine\n");
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
	width = w;
	height = h;

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
