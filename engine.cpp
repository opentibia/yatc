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
Engine* engine;


// first we'll define all the callbacks
static void engine_draw_rectangle(float left, float right, float top, float bottom, glictColor &col) {
	engine->drawRectangle((int)left, (int)top, (int)(right-left), (int)(bottom-top), oRGBA(col.r * 255, col.g * 255, col.b * 255));
}

// set the callbacks up
Engine::Engine() {
	glictGlobals.paintrectCallback = engine_draw_rectangle;
	glictGlobals.enableGlTranslate = false;
	sysfont = 0;
}
