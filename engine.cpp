//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
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

/// \file engine.cpp
/// This file will contain generic code for all engines.
/// Currently it contains only the code to make callbacks
/// work (in effect, translations from C++ style calls to
/// a class into C-style functions).
///
/// Functions are designed to be valid GLICT callbacks.
#include <GLICT/globals.h>
#include <GLICT/types.h>
#include "defines.h"
#include "engine.h"
Engine* g_engine;
int ptrx, ptry;

// first we'll define all the C-style callbacks we'll use elsewhere
void Engine::draw_rectangle(float left, float right, float top, float bottom, glictColor &col)
{
	g_engine->drawRectangle((int)left, (int)top, (int)(right-left), (int)(bottom-top), oRGBA(col.r * 255, col.g * 255, col.b * 255, col.a * 255));
}

void Engine::font_render(const char* txt, const void* font, float x, float y)
{
	Sprite *img = (Sprite*)font;

	float cx = x*10, cy = y*10;
	float sizesofar = 0.f;
	float linessofar = 0.f;
	for(const char* t = txt; *t; ++t){
		switch(*t) {
			default:
				Engine::font_drawchar(*t, img, (int)cx, (int)cy);
				cx += 7;
				sizesofar += 7;
				break;
			case '\n':
			case '\r':
				cx -= sizesofar;
				cy += 16;
				linessofar += 1.;
				sizesofar = 0;
				if (*t == '\n' && *(t + 1) == '\r' ||
					*t == '\r' && *(t + 1)=='\n' )
					t++;
				break;
		}
	}
}

void Engine::font_drawchar(char t, Sprite* img, int x1, int y1)
{
	t -= 32;
	int x = (int)((t % 32)*16.);
	int y = (int)((t / 32)*16.);

	float w = 10;
	float h = 10;

	img->Blit(x1, y1, x, y, w, h);
}

float Engine::font_size(const char* txt, const void* font)
{
    int size=0, len=strlen(txt);
    int maxsize=0;
    for (int i=0;i<len;i++) {
        size++;
        if (txt[i]=='\n' || txt[i]=='\r') {
            if (size>maxsize) maxsize=size;
            if (i < len && (txt[i]=='\n' && txt[i+1]=='\r' || txt[i]=='\r' && txt[i+1]=='\n')) i++;
            size=0;
        }

    }
    if (size>maxsize) maxsize=size;

	return maxsize*.7;
}

// set the callbacks up in the constructor
Engine::Engine()
{
	glictGlobals.paintrectCallback = Engine::draw_rectangle;
	glictGlobals.enableGlTranslate = false;

	doResize(640, 480);

	m_video_bpp = 8;

	m_screen = NULL;
	m_sysfont = glictCreateFont("system");
	if(!m_sysfont){
		// TODO (mips_act#3#): Report error
	}
	m_sysfont->SetRenderFunc(Engine::font_render);
	m_sysfont->SetSizeFunc(Engine::font_size);
}

Engine::~Engine()
{
	glictDeleteFont("system");
}

void Engine::doResize(int w, int h)
{
	m_width = w;
	m_height = h;
	glictGlobals.w = w;
	glictGlobals.h = h;
}
