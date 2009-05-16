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
#include "product.h"
Engine* g_engine;
extern int g_frames;
int ptrx, ptry;

void resetDefaultCursor();

// first we'll define all the C-style callbacks we'll use elsewhere
void Engine::draw_rectangle(float left, float right, float top, float bottom, GLICTCOLORCONST glictColor &col)
{
	g_engine->drawRectangle((int)left, (int)top, (int)(right-left), (int)(bottom-top), oRGBA(col.r * 255, col.g * 255, col.b * 255, col.a * 255));
}

void Engine::draw_rectangle_lines(float left, float right, float top, float bottom, GLICTCOLORCONST glictColor &col)
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
				cx += f->getWidth(*t) + f->getSpacing();
				sizesofar += f->getWidth(*t) + f->getSpacing();
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
				if ((*t == '\n' && *(t + 1) == '\r') ||
					(*t == '\r' && *(t + 1)=='\n' ))
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
		size += f->getWidth(txt[i]) + f->getSpacing();
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

void Engine::font_color(const void* font, glictColor &col){
    Font* f = (Font*)font;
    f->resetColor();
    f->addColor(col.r,col.g,col.b);

}


// set the callbacks up in the constructor
Engine::Engine()
{
	glictGlobals.paintrectCallback = Engine::draw_rectangle;
	glictGlobals.paintrectlinesCallback = Engine::draw_rectangle_lines;
	glictGlobals.enableGlTranslate = false;
	glictGlobals.windowTitleColor.r = glictGlobals.windowTitleColor.g = glictGlobals.windowTitleColor.b = 0.6;

	doResize(options.w, options.h);

	m_video_bpp = options.bpp;

	m_screen = NULL;

	initFont(&m_sysfont, "system");
	initFont(&m_minifont, "minifont");
	initFont(&m_aafont, "aafont");
	initFont(&m_gamefont, "gamefont");

	m_fps = 0.;
    m_ui = NULL;
    m_cursorBasic = m_cursorUse = NULL;

    // remember default cursor
    resetDefaultCursor();

    g_frames = 0;

	m_lastfpsupdate = SDL_GetTicks();
}

Engine::~Engine()
{
	glictDeleteFont("system");
	glictDeleteFont("minifont");
	glictDeleteFont("aafont");
	glictDeleteFont("gamefont");
	delete m_ui;
}


void Engine::performFpsCalc() {
    char caption[255];
    static int interval = 1000;

    if (SDL_GetTicks() >= m_lastfpsupdate + interval) {
        m_lastfpsupdate = SDL_GetTicks();
        m_fps = g_frames * 1000. / interval;
        g_frames = 0;

        sprintf(caption, PRODUCTNAME " - fps: %g", g_engine->m_fps );
        SDL_WM_SetCaption(caption, PRODUCTNAME);

    }

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
	#if (GLICT_APIREV >= 85)
        (*fnt)->SetColorFunc(Engine::font_color);
    #else
        #warning GLICT font coloring is not available. Upgrade to GLICT APIREV 85+.
    #endif
}

void Engine::drawText(const char* text, const char* font, int x, int y, uint8_t color)
{
	Font *f = (Font*)(glictFindFont(font)->GetFontParam());
	if (!f)
        glictFontRender(text, font, x, y);
    else
    {
        float r = (color % 6) / 5.;
        float g = ((color / 6) % 6) / 5.;
        float b = (color / 36) / 5.;

        if (color!=215)
            f->addColor(r,g,b);
        else if (color == 255) // we'll just use otherwise useless 255 for drawing with 0.75, 0.75, 0.75 if needed
            f->addColor(0.75, 0.75, 0.75);
        else
            f->resetColor();
        glictFontRender(text, font, x, y);
    }
}
void Engine::drawText(const char* text, const char* font, int x, int y, oRGBA color)
{
    Font *f = (Font*)(glictFindFont(font)->GetFontParam());
	if (!f)
	{
        glictFontRender(text, font, x, y);
	}
    else
    {
        if (color.r == color.g && color.g == color.b && color.b == 1.)
        {

            f->resetColor();
        }
        else
        {
            f->addColor(color.b/255, color.g/255, color.r/255); // FIXME (ivucica#1#): WTF FIXME should be RGB not BGR!?
        }



        glictFontRender(text, font, x, y);
    }
}

void Engine::doResize(int& w, int& h)
{
	if((w < 656) ||	/* horizontal resize */
		(h < 520) /* vertical resize */)
	{
		w = 656;
		h = 520;
	}
	m_width = w;
	m_height = h;
	glictGlobals.w = w;
	glictGlobals.h = h;
	options.w = w;
	options.h = h;
}



void Engine::reloadGlobalGfx()
{
    delete m_ui;
    m_ui = createSprite("Tibia.spr", 3);
}
