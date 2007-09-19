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
#include <GLICT/globals.h>
#include <sstream>
#include "debugprint.h"
#include "options.h"
#include "engine.h"
#include "objects.h"
#include "gamemode.h"
#include "gm_mainmenu.h"
#include "gm_debug.h"

#include "net/rsa.h"
#include "net/connection.h"
#include "net/protocollogin.h"
#include "net/protocolgame.h"

const char RSAKey_otserv[] = "109120132967399429278860960508995541528237502902798129123468757937266291492576446330739696001110603907230888610072655818825358503429057592827629436413108566029093628212635953836686562675849720620786279431090218017681061521755056710823876476444260558147179707119674283982419152118103759076030616683978566631413";
const char RSAKey_cip[]    = "124710459426827943004376449897985582167801707960697037164044904862948569380850421396904597686953877022394604239428185498284169068581802277612081027966724336319448537811441719076484340922854929273517308661370727105382899118999403808045846444647284499123164879035103627004668521005328367415259939915284902061793";

bool running = true;
uint32_t keymods = 0;

Connection* g_connection = NULL;


void onKeyDown(const SDL_Event& event)
{
	switch(event.key.keysym.sym){
	case SDLK_ESCAPE:
		running = false;
		break;
	case SDLK_LSHIFT:
	case SDLK_RSHIFT:
		keymods = keymods | KMOD_SHIFT;
		break;
	// TODO (Khaos#1#) Add pageup, pagedown, home, end below
	case SDLK_LEFT:
	case SDLK_RIGHT:
	case SDLK_UP:
	case SDLK_DOWN:
		// TODO (Khaos#1#) Pass special keys to a different function
		break;
	default:
		// glict expects what glut usually serves: completely prepared keys,
		// with shift already parsed and all that
		// we'll try to do the parsing here or elsewhere
		int key = event.key.keysym.sym;

		if(event.key.keysym.mod & KMOD_SHIFT){
			if(key >= 'a' && key <='z'){
				key = key - 32;
			}
			else if(key >= '0' && key <='9'){
				key = key - 16;
			}
		}
		// TODO (mips_act#1#) Use SDLK_ constants instead of numeric values
		if(key < 32 && key != 8 && key != 27 && key != 13 && key != 10) // most keys won't be passed here
			return;

		g_game->keyPress(key);
	}
}

/// \brief Main program function
///
/// This function does very little on its own. It manages some output to
/// player's console, directs subsystems to initialize themselves and makes
/// choice of rendering engine. Then it runs the main game loop, processing
/// events and sending them further into the game.
int main(int argc, char *argv[])
{
#ifdef WIN32
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD( 2, 2 );

	if(WSAStartup(wVersionRequested, &wsaData) != 0){
		printf("Winsock startup failed!!");
		return -1;
	}

	if((LOBYTE(wsaData.wVersion) != 2) || (HIBYTE(wsaData.wVersion) != 2)){
		WSACleanup( );
		printf("No Winsock 2.2 found!");
		return -1;
	}
#endif

	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "YATC -- YET ANOTHER TIBIA CLIENT\n");
	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "================================\n");
	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "version 0.1\n");
	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Loading options...\n");
	options.Load();
	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Loading data file...\n");

	if(!Objects::getInstance()->loadDat("tibia.dat")){ //TODO (Smygflik#3#), inform the user with a messagebox
		DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "[FAILED]");
		exit(1);
	}


	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Initializing windowing...\n");

	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr,"Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	try {

		switch(options.engine) {
			case ENGINE_OPENGL:
				g_engine = new EngineGL;
				break;
			/*
			case ENGINE_DIRECTX:
				g_engine = new EngineDX;
				break;
			*/
			default:
				DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_WARNING, "Unknown engine was selected. Falling back to SDL.");
				options.engine = ENGINE_SDL;
			case ENGINE_SDL:
				g_engine = new EngineSDL;
				break;
		}

		if(!g_engine->isSupported()){
			DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_WARNING, "The selected graphics engine is not supported. Falling back to SDL.");
			delete g_engine;
			options.engine = ENGINE_SDL;
			g_engine = new EngineSDL;
		}


		DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Starting main menu...\n"); // perhaps these statuses should be moved in a constructor?


		RSA::getInstance()->setPublicKey(RSAKey_otserv, "65537");
//		g_game = new GM_MainMenu();
		g_game = new GM_Debug(); // ivucica: this is for testing -- choice should be a cmd line option


		DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Running\n");
		SDL_WM_SetCaption("YATC v0.1", NULL);

		SDL_Event event;
		while(running){
			SDL_Delay(100); //limit to 10fps
			while(SDL_PollEvent(&event)){
				switch (event.type){
					case SDL_VIDEORESIZE:
						g_engine->doResize(event.resize.w, event.resize.h);
						break;

					case SDL_QUIT:
						running = false;
						break;

					case SDL_KEYUP:
						if(event.key.keysym.sym == SDLK_LSHIFT ||
							event.key.keysym.sym == SDLK_RSHIFT){
							keymods = keymods & ~(uint32_t)KMOD_SHIFT;
						}
						break;

					case SDL_KEYDOWN:
						onKeyDown(event);
						break;

					case SDL_MOUSEBUTTONUP:
					case SDL_MOUSEBUTTONDOWN:
						g_game->mouseEvent(event);
						break;

					case SDL_MOUSEMOTION:
						ptrx = event.motion.x;
						ptry = event.motion.y;
						break;
					default:
						break;
				}
			}
			if(g_connection){
				g_connection->executeNetwork();
			}
			g_game->renderScene();
			g_engine->Flip();
		}
	} catch (std::string errtext) {
		printf("ERROR: %s\n", errtext.c_str());
	}

	printf("Game over\n");
	printf("Unloading data...\n");
	Objects::getInstance()->unloadDat();
	printf("Shutting down SDL...\n");
	SDL_Quit();
	
#ifdef WIN32
	WSACleanup();
#endif

	printf("Thanks for playing!\n");

	return 0;
}
