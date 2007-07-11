#include <SDL/SDL.h>
#include "debugprint.h"
#include "database.h"
#include "options.h"
#include "engine.h"

Engine* engine;

int main () {

    DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "YATC -- YET ANOTHER TIBIA CLIENT\n");
    DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "================================\n");
    DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "version 0.1\n");
    DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Initializing database...\n");
    DBInit();
    DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Loading options...\n");
	options.Load();
	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Initializing windowing...\n");
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr,"Couldn't initialize SDL: %s\n",SDL_GetError());
        exit( 1 );
    }

    switch(options.engine) {
        case ENGINE_SDL:
            engine = new EngineSDL;
            break;
        case ENGINE_OPENGL:
            engine = new EngineGL;
            break;
        case ENGINE_DIRECTX:
            engine = new EngineDX;
            break;
    }

	return 0;
}
