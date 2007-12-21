//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
// Main
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
/// This file contains the main(int,char**) function.

#define MAXFPS 35


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
#include "util.h"
#include "skin.h"
#include "config.h"
#include "spritesdl.h" // to load icon


#include "net/connection.h"
#include "net/protocollogin.h"
#include "net/protocolgame.h"

bool g_running = false;
uint32_t keymods = 0;

Connection* g_connection = NULL;
uint32_t g_frameTime = 0;

int g_frames;

void onKeyDown(const SDL_Event& event)
{
	switch(event.key.keysym.sym){
	case SDLK_ESCAPE:
		g_running = false;
		break;
	case SDLK_LSHIFT:
	case SDLK_RSHIFT:
		keymods = keymods | KMOD_SHIFT;
		break;
	// TODO (ivucica#1#) Add pageup, pagedown, home, end below
	case SDLK_LEFT:
	case SDLK_RIGHT:
	case SDLK_UP:
	case SDLK_DOWN:
		g_game->specKeyPress(event.key.keysym.sym);
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
		if (key > 0xFF)
			key = (key & 0xFF) + '0';

		// TODO (mips_act#1#) Use SDLK_ constants instead of numeric values
		if(key < 32 && key != 8 && key != 9 && key != 27 && key != 13 && key != 10) // most special keys won't be passed here, but enter, backspace and friends will
			return;

		g_game->keyPress(key);
	}
}


#ifdef WIN32


/*  Declare Windows procedure  */
LRESULT CALLBACK win32splashWindowProcedure (HWND, UINT, WPARAM, LPARAM);
HWND win32splashhwnd;               /* This is the handle for our window */

int showSplashScreen() {
    HWND hwnd;               /* This is the handle for our window */
//    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = NULL;//hThisInstance;
    wincl.lpszClassName = "YATCSplash";
    wincl.lpfnWndProc = win32splashWindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           "YATCSplash",         /* Classname */
           "Loading YATC",       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           NULL,//hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, 1);//nCmdShow);


}


LRESULT CALLBACK win32splashWindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

void hideSplashScreen() {
    DestroyWindow(win32splashhwnd);
}
#endif

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
		DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Winsock startup failed!!");
		return -1;
	}

	if((LOBYTE(wsaData.wVersion) != 2) || (HIBYTE(wsaData.wVersion) != 2)){
		WSACleanup( );
		DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "No Winsock 2.2 found!");
		return -1;
	}

	//showSplashScreen(); // useful on WINCE and generally on slow computers...
#endif


	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "YATC -- YET ANOTHER TIBIA CLIENT\n");
	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "================================\n");
	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "version 0.1\n");
#ifdef BUILDVERSION
	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "build %s\n", BUILDVERSION);
#endif
	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, " This is free software: you are free to change and redistribute it.\n"
		" There is NO WARRANTY, to the extent permitted by law. \n"
		" Review LICENSE in YATC distribution for details.\n");


	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Checking graphics files existence...");
	{
		FILE*f = fopen("Tibia.pic","r");
		if (!f) {
			NativeGUIError("Loading the data file 'Tibia.pic' has failed.\nPlease place 'Tibia.pic' in the same folder as YATC.", "YATC Fatal Error");
			exit(1);
		} else fclose(f);
		f = fopen("Tibia.spr","r");
		if (!f) {
			NativeGUIError("Loading the data file 'Tibia.spr' has failed\nPlease place 'Tibia.spr' in the same folder as YATC.", "YATC Fatal Error");
		}
	}
	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, " passed\n");

	options.Load();

	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Loading data files...\n");
	if(!Objects::getInstance()->loadDat("Tibia.dat")){ //DONE (Smygflik#3#), inform the user with a messagebox
		DEBUGPRINT(DEBUGPRINT_ERROR, DEBUGPRINT_LEVEL_OBLIGATORY, "Loading data file failed!");
		NativeGUIError("Loading the data file 'Tibia.dat' has failed.\nPlease place 'Tibia.dat' in the same folder as YATC.", "YATC Fatal Error");
		exit(1);
	}

	if (!fileexists("Tibia.spr")) {
		DEBUGPRINT(DEBUGPRINT_ERROR, DEBUGPRINT_LEVEL_OBLIGATORY, "Locating sprite file failed!");
		NativeGUIError("Locating the data file 'Tibia.spr' has failed.\nPlease place 'Tibia.spr' in the same folder as YATC.", "YATC Fatal Error");
		exit(1);
	}

	//setenv("SDL_VIDEODRIVER", "aalib", 0);
	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Initializing windowing...\n");

	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
		std::stringstream out;
		out << "Couldn't initialize SDL: " << SDL_GetError() << std::endl;
		DEBUGPRINT(DEBUGPRINT_ERROR, DEBUGPRINT_LEVEL_OBLIGATORY, out.str().c_str());

		NativeGUIError(out.str().c_str(), "YATC Fatal Error");
		exit(1);
	}

	{
		g_engine = NULL;
		SDL_WM_SetCaption("YATC v0.1", "YATC v0.1");
		SpriteSDL *s = new SpriteSDL("Tibia.spr", 13855);
		SpriteSDL *st = new SpriteSDL("Tibia.spr", 13575);
		s->templatedColorize(st, 79, 94, 88, 82);
		s->setAsIcon();
		delete s;
		delete st;
	}

	SDL_EnableKeyRepeat(200, 50);

	try {

		switch(options.engine) {
			#ifdef USE_OPENGL
			case ENGINE_OPENGL:
				g_engine = new EngineGL;
				break;
			#endif

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


		DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Loading skin...\n");
		g_skin.loadSkin();
		DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Skin has been loaded\n");


		DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Constructing gamemode...\n");
		g_game = new GM_MainMenu();
		//g_game = new GM_Debug(); // ivucica: this is for testing -- choice should be a cmd line option


		DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Running\n");
        #ifdef WIN32
        hideSplashScreen();
        #endif
        g_running = true;

		SDL_Event event;
		while(g_running){

            int beginticks = SDL_GetTicks();
			//first process sdl events
			while(SDL_PollEvent(&event)){
				switch (event.type){
					case SDL_VIDEORESIZE:
						g_engine->doResize(event.resize.w, event.resize.h);
						g_game->doResize(event.resize.w, event.resize.h);
						break;

					case SDL_QUIT:
						g_running = false;
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
						#ifdef WINCE
						if (ptrx < 5 && ptry < 5)
							SDL_WM_IconifyWindow(); // appears to crash the application?! ah nevermind
						#endif
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
			//update current frame time
			g_frameTime = SDL_GetTicks();


            if (g_frameTime-beginticks < 1000/MAXFPS-10)
                SDL_Delay(1000/MAXFPS - (g_frameTime - beginticks)-10); //limit to 10fps

            while (abs((int)SDL_GetTicks() - beginticks) < 1000/MAXFPS);

			//check connection
			if(g_connection){
				g_connection->executeNetwork();
			}
			//and finally update scene
			g_game->updateScene();
			g_engine->Flip();
			g_frames ++;
		}
	} catch (std::string errtext) {
		DEBUGPRINT(DEBUGPRINT_ERROR, DEBUGPRINT_LEVEL_OBLIGATORY, "%s", errtext.c_str());
	}

	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Game over\n");

	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Unloading data...\n");
	Objects::getInstance()->unloadDat();
	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Saving options...\n");
	options.Save();
	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Finishing engine...\n");
	delete g_engine;
	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Ending game...\n");
	delete g_game;
	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Shutting down SDL...\n");
	SDL_Quit();

#ifdef WIN32
	WSACleanup();
#endif

	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Thanks for playing!\n");

	return 0;
}
