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
#include <GLICT/globals.h>
#include <SDL/SDL.h>
#include "debugprint.h"
#include "defines.h"
#include "sprite.h"
#include "spritesdl.h"
#include "font.h"
#include "fassert.h"
struct glictColor;

#if defined(_MSC_VER) && !defined(__PRETTY_FUNCTION__)
#define __PRETTY_FUNCTION__ "(msvc)"
#endif

class Engine
{
	public:
		virtual ~Engine();

		virtual bool isSupported() = 0;

		virtual void doResize(int w, int h);
		virtual void drawRectangle(float x, float y, float width, float height, oRGBA color) = 0;
		virtual void drawRectangleLines(float x, float y, float width, float height, oRGBA color) {}
		virtual void drawText(const char* text, const char* font, int x, int y, uint8_t color=215); // 215 == TEXTCOLOR_WHITE
		virtual float sizeText(const char* text, const char* font) { float a = glictFontSize(text, font, 10); ASSERTFRIENDLY(a<20000, "Looks like text width is enormous."); return a; }

		int getWindowWidth() const {return m_width;};
		int getWindowHeight() const {return m_height;};

		virtual Sprite* createSprite(const std::string& filename, int index = 0) = 0;

		virtual void Flip() = 0;

        void performFpsCalc();

		uint32_t m_creationTimestamp; // some engines, like GL, need this in order to know when to re-create their sprite memories

        virtual void resetClipping() {
        }
        virtual void setClipping(int top, int left, int width, int height) {
            static bool printedwarning = false;
            if (printedwarning) return;
            printedwarning = true;
            printf("warning: engine %s did not redefine %s\n", getName(), __PRETTY_FUNCTION__);
        }
        virtual const char* getName() const {return "Unknown";}
        virtual bool hasSDL() const { return true; }
        virtual bool hasGL() const { return false; }
	protected:
		Engine();
		void initFont(glictFont** fnt, const char* fontname);

		friend void SpriteSDL::Blit(float dx, float dy, float sx, float sy, float w, float h);


#if (GLICT_APIREV < 69)
#define GLICTCOLORCONST
#warning Please upgrade to GLICT APIREV 69+
#else
#define GLICTCOLORCONST const
#endif

		static void draw_rectangle(float left, float right, float top, float bottom, GLICTCOLORCONST glictColor &col);
		static void draw_rectangle_lines(float left, float right, float top, float bottom, GLICTCOLORCONST glictColor &col);
		static void font_render(const char* txt, const void* font, float fontsize, float x, float y);
		static float font_size(const char* txt, const void* font, float fontsize);

		int m_width;
		int m_height;
		int m_video_bpp;

		float m_fps;
		//SDL_TimerID m_fpstimer __attribute__((__deprecated__));
		//SDL_mutex *m_fpsmutex __attribute__((__deprecated__));

		glictFont *m_sysfont, *m_minifont, *m_aafont, *m_gamefont;
		SDL_Surface* m_screen;
		int m_videoflags;

		uint32_t m_lastfpsupdate;


		friend class winOptionsGraphics_t;
};

extern int ptrx, ptry;
#include "enginesdl.h"
#ifdef USE_OPENGL
#include "enginegl.h"
#endif
extern Engine* g_engine;

#endif
