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

#ifndef __ENGINESDL_H
#define __ENGINESDL_H

#include "engine.h"
#include "spritesdl.h"
class EngineSDL : public Engine
{
	public:
    	EngineSDL();
    	~EngineSDL();

    	bool isSupported() { return true; } ///< Since SDL is always supported, this function always returns true.
		void doResize(int h, int w);

    	void drawRectangle(float x, float y, float width, float height, oRGBA color);

		virtual Sprite* createSprite(const std::string& filename, int index = 0)
		{
			return new SpriteSDL(filename, index);
		}

		virtual void Flip(){
			SDL_Flip(m_screen);
		}

	protected:

		static void font_render(const char* txt, const void* font, float x, float y);
		static void font_drawchar(char t, SpriteSDL* img, int x1, int y1);
		static float font_size(const char* txt, const void* font);
};

#endif
