//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
// Base engine class
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
#include <sstream>
#include "util.h"
#include "defines.h"
#include "engine.h"
#include "font.h"
#include "options.h"
Engine* g_engine;
extern int g_frames;
int ptrx, ptry;

// first we'll define all the C-style callbacks we'll use elsewhere
void Engine::draw_rectangle(float left, float right, float top, float bottom, glictColor &col)
{
	g_engine->drawRectangle((int)left, (int)top, (int)(right-left), (int)(bottom-top), oRGBA(col.r * 255, col.g * 255, col.b * 255, col.a * 255));
}

void Engine::draw_rectangle_lines(float left, float right, float top, float bottom, glictColor &col)
{
	g_engine->drawRectangleLines((int)left, (int)top, (int)(right-left), (int)(bottom-top), oRGBA(col.r * 255, col.g * 255, col.b * 255, col.a * 255));
}

void Engine::font_render(const char* txt, const void* font, float fontsize, float x, float y)
{
	Font* f = (Font*)font;
	float cx = x, cy = y ;
	float sizesofar = 0.f;
	float linessofar = 0.f;
	for(const char* t = txt; *t; ++t){
		switch(*t) {
			default:
				f->Blit(*t,(int)cx,(int)cy);
				cx += f->getWidth(*t)+1;
				sizesofar += f->getWidth(*t)+1;
				break;
			case '\t':
				cx += (int)sizesofar % 15;// FIXME (ivucica#5#) use fmod
				sizesofar += (int)sizesofar % 15;
				break;
			case '\n':
			case '\r':
				cx -= sizesofar;
				cy += f->getHeight('\n');
				linessofar += 1.;
				sizesofar = 0;
				if (*t == '\n' && *(t + 1) == '\r' ||
					*t == '\r' && *(t + 1)=='\n' )
					t++;
				break;
		}
	}
}

float Engine::font_size(const char* txt, const void* font, float fontsize)
{
	Font* f = (Font*)font;
	int size = 0, len = strlen(txt);
	int maxsize = 0;
	for(int i = 0; i < len; i++) {
		size += f->getWidth(txt[i])+1;
		if(txt[i] == '\n' || txt[i] == '\r'){
			if(size > maxsize){
				maxsize = size;
			}
			if(i < (len - 1) && ((txt[i] == '\n' && txt[i + 1] == '\r') ||
								 (txt[i] == '\r' && txt[i + 1] == '\n'))){
				i++;
			}
			size = 0;
		}
	}
	if(size > maxsize){
		maxsize = size;
	}

	return maxsize;
}

// set the callbacks up in the constructor
Engine::Engine()
{
	glictGlobals.paintrectCallback = Engine::draw_rectangle;
	glictGlobals.paintrectlinesCallback = Engine::draw_rectangle_lines;
	glictGlobals.enableGlTranslate = false;

	doResize(options.w, options.h);

	m_video_bpp = options.bpp;

	m_screen = NULL;

	initFont(&m_sysfont, "system");
	initFont(&m_minifont, "minifont");
	initFont(&m_aafont, "aafont");
	initFont(&m_gamefont, "gamefont");

	m_fps = 0.;

	g_frames = 0;
	DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Setting up FPS timer\n");
	m_fpstimer = SDL_AddTimer(1000, Engine::fpsTimer, NULL);
	if (!m_fpstimer) {// FIXME (ivucica#3#) this should be an assertion; wince appears to always fail
		DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "[!] Failed to set up FPS timer!\n");
		m_fps = 5.;
	}
}

Engine::~Engine()
{
	SDL_RemoveTimer(m_fpstimer);
	glictDeleteFont("system");
	glictDeleteFont("minifont");
	glictDeleteFont("aafont");
	glictDeleteFont("gamefont");
}

Uint32 Engine::fpsTimer(Uint32 interval, void*param)
{
    char caption[255];

    g_engine->m_fps = (g_frames / (float)interval) * 1000;
    g_frames = 0;

    sprintf(caption, "YATC v0.1 - fps: %g", g_engine->m_fps );
    SDL_WM_SetCaption(caption, "YATC v0.1");
	return interval;
}


void Engine::initFont(glictFont **fnt, const char *fontname)
{
	*fnt = glictCreateFont(fontname);
	if(!*fnt){
		std::stringstream s;
		s << "Cannot load font '" << fontname << "'.";
		NativeGUIError(s.str().c_str(), "Error loading font");
	}
	(*fnt)->SetRenderFunc(Engine::font_render);
	(*fnt)->SetSizeFunc(Engine::font_size);
}

void Engine::drawText(const char* text, const char* font, int x, int y, uint8_t color)
{
	// TODO (ivucica#2#) stub -- we'll just render in white for now. later we must recolor, and paint it ourselves instead of delegating to glic
	glictFontRender(text, font, x, y);
}

void Engine::doResize(int w, int h)
{
	m_width = w;
	m_height = h;
	glictGlobals.w = w;
	glictGlobals.h = h;
	options.w = w;
	options.h = h;
}

