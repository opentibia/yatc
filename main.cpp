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

/// \file main.cpp
/// This file contains the main(int,char*) function.

#include <SDL/SDL.h>
#include "debugprint.h"
#include "database.h"
#include "options.h"
#include "engine.h"
#include "objects.h"

Engine* engine;


/// \brief Main program function
///
/// This function does very little on its own. It manages some output to
/// player's console, directs subsystems to initialize themselves and makes
/// choice of rendering engine. Then it runs the main game loop, processing
/// events and sending them further into the game.

int main(int argc, char *argv[])
{
	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "YATC -- YET ANOTHER TIBIA CLIENT\n");
	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "================================\n");
	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "version 0.1\n");
	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Initializing database...\n");
	DBInit();
	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Loading options...\n");
	options.Load();
	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Loading data file...\n");

	if(!Objects::getInstance()->loadDat("tibia.dat")){ //TODO, inform the user with a messagebox
		DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "[FAILED]");
		exit(1);
	}

	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Initializing windowing...\n");

	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr,"Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	atexit(SDL_Quit);

	switch(options.engine) {
		case ENGINE_OPENGL:
				engine = new EngineGL;
			break;
		case ENGINE_DIRECTX:
				engine = new EngineDX;
			break;
		case ENGINE_SDL:
		default:
				engine = new EngineSDL;
			break;
	}

	SDL_WM_SetCaption ("YATC v0.1", NULL);

	bool running = true;
	SDL_Event event;

	while(running){
		while(SDL_PollEvent(&event)){
			switch (event.type){
				case SDL_KEYDOWN:
						//
					break;
				case SDL_VIDEORESIZE:
						engine->doResize(event.resize.w, event.resize.h);
					break;
				case SDL_QUIT:
						running = false;
					break;
				default:
					break;
			}
		}

		engine->Flip();
	}

	Objects::getInstance()->unloadDat();

	return 0;
}
