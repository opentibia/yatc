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

#ifndef __ENGINEGL_H
#define __ENGINEGL_H

#include "engine.h"

#ifdef USE_OPENGL

#include <GLICT/fonts.h>
#include "spritegl.h"
class EngineGL : public Engine
{
	public:
		EngineGL();
		~EngineGL();

		bool isSupported();

		void Flip(){SDL_GL_SwapBuffers();};
		void doResize(int h, int w);
		void drawRectangle(float x, float y, float width, float height, oRGBA color);
		void drawRectangleLines(float x, float y, float width, float height, oRGBA color);

		virtual Sprite* createSprite(const std::string& filename, int index = 0)
		{
			return new SpriteGL(filename, index);
		}
		virtual Sprite* createSprite(int w, int h, const oRGBA& c){
			return new SpriteGL(w, h, c);
		}

        virtual const char* getName() const {return "OpenGL";}
        bool hasGL() const { return true; }
	protected:

		void initEngine();
};

#else // use_opengl

class EngineGL : public Engine
{
}

#endif // use_opengl

#endif
