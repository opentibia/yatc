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

class Engine
{
public:
	Engine();
	virtual ~Engine() {
		DEBUGPRINT(DEBUGPRINT_WARNING, DEBUGPRINT_LEVEL_DEBUGGING, "Engine::~Engine(): Function not overloaded");
	}
	virtual bool isSupported() {
		DEBUGPRINT(DEBUGPRINT_WARNING, DEBUGPRINT_LEVEL_DEBUGGING, "Engine::isSupported(): Function not overloaded");
		return false;
	}


	virtual void doResize(int w, int h){width = w; height = h;};
	virtual void drawRectangle(int x, int y, int width, int height, oRGBA color){
		DEBUGPRINT(DEBUGPRINT_WARNING, DEBUGPRINT_LEVEL_DEBUGGING, "Engine::drawRectangle(): Function not overloaded");
	};

	int getWindowWidth(){return width;};
	int getWindowHeight(){return height;};

	virtual void Flip() {
		SDL_Flip(screen);
	}
protected:

	int videoflags;
	int width;
	int height;
	int video_bpp;
	SDL_Surface* screen;
	glictFont* sysfont;
};

#include "enginesdl.h"
#include "enginegl.h"
#include "enginedx.h"
extern Engine* engine;
#endif
