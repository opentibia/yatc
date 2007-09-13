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

#ifndef __ENGINE_H
#define __ENGINE_H

#include <GLICT/fonts.h>
#include <SDL/SDL.h>
#include "debugprint.h"
#include "defines.h"
#include "sprite.h"

struct glictColor;

class Engine
{
	public:
		virtual ~Engine();

		virtual bool isSupported() = 0;

		virtual void doResize(int w, int h){m_width = w; m_height = h;};
		virtual void drawRectangle(float x, float y, float width, float height, oRGBA color) = 0;

		int getWindowWidth() const {return m_width;};
		int getWindowHeight() const {return m_height;};

		virtual Sprite* createSprite(const std::string& filename, int index = 0) = 0;

		virtual void Flip() = 0;

		SDL_Surface* getScreen() { return m_screen;}

	protected:
		Engine();

		static void draw_rectangle(float left, float right, float top, float bottom, glictColor &col);
		static void font_render(const char* txt, const void* font, float x, float y);
		static void font_drawchar(char t, Sprite* img, int x1, int y1);
		static float font_size(const char* txt, const void* font);

		int m_width;
		int m_height;
		int m_video_bpp;

		glictFont* m_sysfont;
		SDL_Surface* m_screen;
		int m_videoflags;
};

extern int ptrx, ptry;
#include "enginesdl.h"
#include "enginegl.h"
extern Engine* g_engine;

#endif
