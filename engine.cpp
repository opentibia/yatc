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
#include <cmath>
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
	YATCFont* f = (YATCFont*)font;
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
	YATCFont* f = (YATCFont*)font;
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
    YATCFont* f = (YATCFont*)font;
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
	m_light = NULL;
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
	delete m_light;
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
	YATCFont *f = (YATCFont*)(glictFindFont(font)->GetFontParam());
	if (f)
    {
        float r = (color / 36) / 5.;
        float g = ((color / 6) % 6) / 5.;
        float b = (color % 6) / 5.;

        if (color!=215)
            f->addColor(r,g,b);
        else if (color == 255) // we'll just use otherwise useless 255 for drawing with 0.75, 0.75, 0.75 if needed
            f->addColor(0.75, 0.75, 0.75);
        else
            f->resetColor();
    }
    glictFontRender(text, font, x, y);
}
void Engine::drawText(const char* text, const char* font, int x, int y, oRGBA color)
{
    YATCFont *f = (YATCFont*)(glictFindFont(font)->GetFontParam());
	if (f)
	{
        if (color.r == color.g && color.g == color.b && color.b == 1.)
        {
            f->resetColor();
        }
        else
        {
            f->addColor(color.r/255, color.g/255, color.b/255);
        }
    }
    glictFontRender(text, font, x, y);
}

void Engine::drawTextGW(const char* text, const char* font, int x, int y, float scale, uint8_t color)
{
    // NOTE (nfries88): keeps all rendering in the game area.
    //x = std::min(std::max(1, x), (m_width - 176) - (int)sizeText(text, font)); // NOTE (Kilouco): Doesn't work this way anymore.
    //y = std::max(1, y);

	YATCFont *f = (YATCFont*)(glictFindFont(font)->GetFontParam());
    if (f)
    {
        float r = (color / 36) / 5.;
        float g = ((color / 6) % 6) / 5.;
        float b = (color % 6) / 5.;

        if (color!=215)
            f->addColor(r,g,b);
        else if (color == 255) // we'll just use otherwise useless 255 for drawing with 0.75, 0.75, 0.75 if needed
            f->addColor(0.75, 0.75, 0.75);
        else
            f->resetColor();
    }

    std::string temp_text = text;

    std::string new_line_text, old_line_text;
    new_line_text = old_line_text = temp_text;
    int linecount = 1;
    size_t iter_pos;

    // NOTE (Kilouco): Here we centralize all the message and handle positions so it will never go offscreen.
    while (1) {
        iter_pos = old_line_text.find_first_of("\n");

        if(iter_pos == std::string::npos || iter_pos <= 0) {
            int text_size = sizeText(old_line_text.c_str(), font);
            volatile float centralizationoffset =  text_size / 2;
            if (x + centralizationoffset > (480 * scale) + 2)
                x = (480 * scale - 2) - text_size;
            else if (x < 2)
                x = 2 + centralizationoffset;
            else
                x = x - centralizationoffset;

            if (y < 2)
                y = 2;

            glictFontRender(old_line_text.c_str(), font, x, y + (12 * (linecount - 1)));
            break;
        }
        else {
            new_line_text = old_line_text.substr(iter_pos+1);
            old_line_text.resize(iter_pos);
            int text_size = sizeText(old_line_text.c_str(), font);
            volatile float centralizationoffset =  text_size / 2;
            if (x + centralizationoffset > (480 * scale) + 2)
                x = (480 * scale - 2) - text_size;
            else if (x < 2)
                x = 2 + centralizationoffset;
            else
                x = x - centralizationoffset;

            if (y < 2)
                y = 2;

            glictFontRender(old_line_text.c_str(), font, x, y + (12 * (linecount - 1)));
            old_line_text = new_line_text;
            linecount++;
        }
    }
}

void Engine::drawTextGW(const char* text, const char* font, int x, int y, float scale, oRGBA color)
{
    // NOTE (nfries88): keeps all rendering in the game area.
    //x = std::min(std::max(1, x), (m_width - 176) - (int)sizeText(text, font));
    //y = std::max(1, y);

    YATCFont *f = (YATCFont*)(glictFindFont(font)->GetFontParam());
	if (f)
    {
        if (color.r == color.g && color.g == color.b && color.b == 1.)
        {
            f->resetColor();
        }
        else
        {
            f->addColor(color.r/255, color.g/255, color.b/255);
        }
    }

    std::string temp_text = text;
    std::string new_line_text, old_line_text;
    new_line_text = old_line_text = temp_text;
    int linecount = 1;
    size_t iter_pos;

    while (1) {
        iter_pos = old_line_text.find_first_of("\n");

        if(iter_pos == std::string::npos || iter_pos <= 0) {

            int text_size = sizeText(old_line_text.c_str(), font);
            volatile float centralizationoffset =  text_size / 2;

            if (x + centralizationoffset > (480 * scale) + 2)
                x = (480 * scale - 2) - text_size;
            else if (x < 2)
                x = 2 + centralizationoffset;
            else
                x = x - centralizationoffset;

            if (y < 2)
                y = 2;

            glictFontRender(old_line_text.c_str(), font, x, y + (12 * (linecount - 1)));

            break;
        }
        else {
            new_line_text = old_line_text.substr(iter_pos+1);
            old_line_text.resize(iter_pos);

            int text_size = sizeText(old_line_text.c_str(), font);
            volatile float centralizationoffset =  text_size / 2;

            if (x + centralizationoffset > (480 * scale) + 2)
                x = (480 * scale - 2) - text_size;
            else if (x < 2)
                x = 2 + centralizationoffset;
            else
                x = x - centralizationoffset;

            if (y < 2)
                y = 2;

            glictFontRender(old_line_text.c_str(), font, x, y + (12 * (linecount - 1)));

            old_line_text = new_line_text;
            linecount++;
        }
    }
}

void Engine::doResize(int& w, int& h)
{
	#ifndef WINCE
    // NOTE (nfries88): Make sure the window is sized as per the options
    w = std::max(w, 656);
	h = std::max(h, 352+options.consoleh);
	#endif
	m_width = w;
	m_height = h;
	glictGlobals.w = w;
	glictGlobals.h = h;
	options.w = w;
	options.h = h;
}

void Engine::drawLightmap(vertex* lightmap, int type, int width, int height, int scale)
{
	int index = 0;
	int scaledSize = (int)std::floor((float)(32 * scale));

	for (int i = 0; i < width; ++i){
        for (int j = 0; j < height; ++j){
            index = (j * width) + i;
            if(type == 1 && m_light != NULL){
                int n = std::floor(8 - (8 * (lightmap[index].alpha / 255.0f)));
                if (n >= 0 && n < 8)
                    m_light->Blit(lightmap[index].x, lightmap[index].y, 32 * n, 0, 32, 32, scaledSize, scaledSize);
            }
            else if(type != 0){ // always default to the better light unless "no light" is specified (this value might be 3 while in SDL engine).
                drawRectangle(lightmap[index].x, lightmap[index].y, scaledSize, scaledSize, oRGBA(lightmap[index].r, lightmap[index].g,
                    lightmap[index].b, (lightmap[index].alpha)));
            }
        }
    }
}

void Engine::reloadGlobalGfx()
{
    delete m_ui;
    m_ui = createSprite("Tibia.pic", 3);
}
