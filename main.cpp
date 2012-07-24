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

#include <SDL/SDL_syswm.h>

unsigned int MAXFPS=50;

#if !defined(WIN32) && !defined(__APPLE__)
#include <signal.h>
#include <execinfo.h>

/* get REG_EIP from ucontext.h */
//#define __USE_GNU
#include <ucontext.h>
#endif


#ifndef __USE_GLSDL__
#include <SDL/SDL.h>
#else
#include <SDL/glSDL.h>
#endif
#ifdef __APPLE__
	#include <SDL_gfx/SDL_framerate.h>
#else
	#include <SDL/SDL_framerate.h>
#endif

#include <GLICT/globals.h>
#include <sstream>
#include <stdlib.h>

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif
#if defined(HAVE_LIBINTL_H)
	#include <libintl.h>
#else
	#define gettext(x) (x)
#endif
#include <locale.h>

#include "debugprint.h"
#include "options.h"
#include "engine.h"
#include "objects.h"
#include "gamemode.h"
#include "gm_mainmenu.h"
#include "gm_debug.h"
#include "util.h"
#include "skin.h"
#include "spritesdl.h" // to load icon
#include "product.h"

#include "clipboard.h"

#include "net/connection.h"
#include "net/connectionreplay.h"
#include "net/protocollogin.h"
#include "net/protocolgame.h"
#include "gamecontent/creature.h"
#include "gamecontent/map.h"
#include "gamecontent/inventory.h"
bool g_running = false;
//uint32_t keymods = 0;

Connection* g_connection = NULL;
uint32_t g_frameTime = 0;
uint32_t g_frameDiff = 0;

int g_frames;
FPSmanager g_fpsmgr; // for sdl_gfx's fps management functions
yatcClipboard g_clipboard;
std::string g_recordfilename="debugrecord.rec";

extern float g_replayspeed;

// NOTE (nfries88): This should really have been included by SDL as a modifier key, but wasn't
//              So I'll make my own cruddy implementation. :D
//              This is apple's "command" key and microsoft's "windows" key.
// false is down, true is up
bool superkey_state = false;


void onKeyDown(const SDL_Event& event)
{
	int key = event.key.keysym.sym;
	switch(key){
	case 0:
		{
			// TODO (nfries88): Some key combinations (IE, CTRL+TAB) result in scancodes that
			//	can't be represented using  ascii for proper UI compatibility we need to
			//	interpret some of those keys combinations.
		}
		break;
	case SDLK_ESCAPE:
		//g_running = false;
		g_game->specKeyPress(event.key.keysym);
		break;

	case SDLK_LSHIFT:
	case SDLK_RSHIFT:
		// ignore shiftpresses
		break;

    case SDLK_LSUPER: case SDLK_RSUPER:
        superkey_state = true;
        break;

	case SDLK_LEFT: case SDLK_RIGHT: case SDLK_UP: case SDLK_DOWN: case SDLK_KP1: case SDLK_KP2: case SDLK_KP3: case SDLK_KP4: case SDLK_KP6: case SDLK_KP7: case SDLK_KP8: case SDLK_KP9:
	case SDLK_PAGEUP: case SDLK_PAGEDOWN:  case SDLK_HOME: case SDLK_END:
	case SDLK_F1: case SDLK_F2: case SDLK_F3: case SDLK_F4: case SDLK_F5: case SDLK_F6: case SDLK_F7: case SDLK_F8: case SDLK_F9: case SDLK_F10: case SDLK_F11: case SDLK_F12:
		if (g_game->specKeyPress(event.key.keysym))
            break;
	default:
		// glict expects what glut usually serves: completely prepared keys,
		// with shift already parsed and all that
		// we'll try to do the parsing here or elsewhere

		if(key < 32){
			switch(key){
			case SDLK_BACKSPACE:
			case SDLK_TAB:
			case SDLK_RETURN:
			case SDLK_ESCAPE:
				break;
			default:
				return;
			}
		}

		if((event.key.keysym.mod & KMOD_NUM) && key >= 256 && key <= 271){ //Numeric keypad
			switch(key){
			case SDLK_KP_PERIOD:
				key = SDLK_PERIOD;
				break;
			case SDLK_KP_DIVIDE:
				key = SDLK_SLASH;
				break;
	 		case SDLK_KP_MULTIPLY:
	 			key = SDLK_ASTERISK;
		 		break;
			case SDLK_KP_MINUS:
				key = SDLK_MINUS;
				break;
			case SDLK_KP_PLUS:
				key = SDLK_PLUS;
				break;
			case SDLK_KP_ENTER:
				key = SDLK_RETURN;
				break;
			default:
				key = event.key.keysym.unicode;
				break;
			}
		} else {

#ifdef __APPLE__
			// for some retarded reason, apparently SDL on MacOSX gives SDLK_DELETE on backspace.
			// whatever. this should be the fix.
			if (key != SDLK_BACKSPACE)
#endif
		    key = event.key.keysym.unicode;
		}

		if(key > 255)
			return;
		g_game->keyPress(key);
	}
}

void checkFile(const char *filename)
{
    printf("Checking for %s...", filename);
	if (fileexists(filename)) {
		printf("[ok]\n");
	} else {
		printf("[no]\n");

		std::string fetcherfn = "";

        if ((fetcherfn=yatc_findfile("tdffetcher"))=="") {
		    if ((fetcherfn=yatc_findfile("tdffetcher.exe"))=="") {
                std::string forreplace;
#ifndef WIN32
                forreplace = gettext("Loading the data file '$$FILENAME$$' has failed.\n"
                        "Please place '$$FILENAME$$' in the same folder as $$PRODUCTSHORT$$.\n"
                        "If you are a Debian user, you may have forgotten to install\n"
                        "the 'tibia-data' or 'yatc-data' package.");
#else
				                forreplace = gettext("Loading the data file '$$FILENAME$$' has failed.\n"
                        "Please place '$$FILENAME$$' in the same folder as $$PRODUCTSHORT$$.\n");
#endif

                forreplace = str_replace("$$FILENAME$$", filename, forreplace);
                forreplace = str_replace("$$PRODUCTSHORT$$", PRODUCTSHORT, forreplace);
                NativeGUIError(forreplace.c_str(), str_replace("$$PRODUCTSHORT$$", PRODUCTSHORT, gettext("$$PRODUCTSHORT$$ Fatal Error")).c_str());
                printf("EXITING\n");
                exit(1);
		    }
        }

        std::string forreplace;

        forreplace = gettext("You are missing '$$FILENAME$$'.\n"
                "We will launch Tibia Data File Fetcher which should automatically install\n"
                "data files required for $$PRODUCTSHORT$$.\n"
                "\n"
                "You will have to manually restart $$PRODUCTSHORT$$ afterwards.\n"
                "\n");
#ifndef WIN32
        forreplace += gettext("If you are a Debian user, you may have forgotten to install\n"
                              "the 'tibia-data' or 'yatc-data' package.");
#endif
        forreplace = str_replace("$$FILENAME$$", filename, forreplace);
        forreplace = str_replace("$$PRODUCTSHORT$$", PRODUCTSHORT, forreplace);

        NativeGUIError(forreplace.c_str(), str_replace("$$PRODUCTSHORT$$", PRODUCTSHORT, gettext("$$PRODUCTSHORT$$ Fatal Error")).c_str());

        // hack to make the cmdline box disappear
        if (fetcherfn == "tdffetcher.exe") fetcherfn = "start tdffetcher.exe";

        int dummy = system(fetcherfn.c_str());
        dummy = 0;


		exit(1);

	}

}

void checkFiles()
{
	checkFile("Tibia.pic");
	checkFile("Tibia.spr");
	checkFile("Tibia.dat");
}

void setIcon()
{
	//TODO (nfries88): Change so that icon is independent of .dat and .spr
	//NOTE (ivucica): I disagree. I think icon is much nicer when loaded from .spr. And we distribute one file less.
	printf("Setting icon\n");
	g_engine = NULL;
	SDL_WM_SetCaption(PRODUCTNAME, PRODUCTNAME);
	
#if 1 //load a sprite from .spr as an icon

	ObjectType* o = Objects::getInstance()->getOutfitType(58);// priestess

    	SpriteSDL *s = new SpriteSDL("Tibia.spr", o->imageData[(o->width * o->height)* o->blendframes*2]);

	// use this only if templated (colorizable) sprite is used
	//SpriteSDL *st = new SpriteSDL("Tibia.spr", o->imageData[(o->width * o->height)* o->blendframes*2+1]);
	//s->templatedColorize(st, 79, 94, 88, 82);

#else // load .ico -- a bad idea because then we must distribute .ico
	SpriteSDL* s = new SpriteSDL("yatc.ico", 0);
#endif
	if(s) s->setAsIcon();
	delete s;
	//delete st;
}

void resetDefaultCursor()
{
    static SDL_Cursor* defaultcursor = NULL;
    if (!defaultcursor)
        defaultcursor = SDL_GetCursor();
    else
        SDL_SetCursor(defaultcursor);

}


#if !defined(WIN32) && !defined(__APPLE__)
#ifdef _GLIBCXX_DEBUG
#if __WORDSIZE != 64
#define INSTRUCTION_POINTER_REGISTER REG_EIP
#else
#define INSTRUCTION_POINTER_REGISTER REG_RIP
#endif //__WORDSIZE

void crashhndl(int sig, siginfo_t *info,
				   void *secret) {

    void *trace[32];
    char **messages = (char **)NULL;
    int i, trace_size = 0;
    ucontext_t *uc = (ucontext_t *)secret;
    static char addr2line_buffer[32];

    printf("CRASH\n");

    /* Do something useful with siginfo_t */
    if (sig == SIGSEGV)
    printf("Got signal %d, faulty address is %p, "
           "from %p\n", sig, (void*)info->si_addr,
           (void*)uc->uc_mcontext.gregs[INSTRUCTION_POINTER_REGISTER]);
    else
    printf("Got signal %d\n", sig);

    trace_size = backtrace(trace, 32);
    /* overwrite sigaction with caller's address */
    trace[1] = (void *) uc->uc_mcontext.gregs[INSTRUCTION_POINTER_REGISTER];



    messages = backtrace_symbols(trace, trace_size);
    /* skip first stack frame (points here) */
    printf("[bt] Execution path:\n");
    for (i=1; i<trace_size; ++i) {
        printf("[bt] %s\n[bt]\t\t", messages[i]);
        fflush(stdout);
        std::stringstream x;
        x << "addr2line -C -e yatc " << std::hex << trace[i];
        system(x.str().c_str());
        /*
		NOTE (nfries88): This code is only commented because it is untested, I am not
				on a Unix-like system at the moment.
			If this is found to work as expected it should be used instead of the
				stringstream code above, as this shouldn't use dynamic allocations.
		// FIXME (nfries88): Test this and use it instead of stringstream.
		sprintf(addr2line_buffer, "addr2line -C -e yatc %8x", trace[i]);
		system(addr2line_buffer);
		*/
    }
    SDL_WM_GrabInput(SDL_GRAB_OFF);


    exit(0);
}
#endif
#endif



/// \brief Main program function
///
/// This function does very little on its own. It manages some output to
/// player's console, directs subsystems to initialize themselves and makes
/// choice of rendering engine. Then it runs the main game loop, processing
/// events and sending them further into the game.
int main(int argc, char *argv[])
{

    #if !defined(WIN32) && !defined(__APPLE__)
    #ifdef _GLIBCXX_DEBUG
	/*#if __WORDSIZE != 64*/

    /* Install our signal handler */
    struct sigaction sa;

    sa.sa_sigaction = /*(void *)*/crashhndl;
    sigemptyset (&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_SIGINFO;

    sigaction(SIGSEGV, &sa, NULL);
    sigaction(SIGFPE, &sa, NULL);
    /*#endif*/
	#endif
    #endif



#ifdef WIN32
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD( 2, 2 );

	if(WSAStartup(wVersionRequested, &wsaData) != 0){
		DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Winsock startup failed!!");
		return -1;
	}

	if((LOBYTE(wsaData.wVersion) != 2) || (HIBYTE(wsaData.wVersion) != 2)){
		WSACleanup( );
		DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "No Winsock 2.2 found!");
		return -1;
	}

#endif

	//setenv("SDL_VIDEODRIVER", "aalib", 0);
	//setenv("AAOPTS","-width 200 -height 70 -dim -reverse -bold -normal -boldfont  -eight -extended ",0);
	//setenv("AAFont","-*-fixed-bold-*-*-*-*-55-*-*-*-*-*-*",0);
	DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, PRODUCTLONG "\n");
	DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "================================\n");
	DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "version " PRODUCTVERSION "\n");
	DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "compiled on " __DATE__ " " __TIME__ "\n");
#ifdef BUILDVERSION
	DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "build %s\n", BUILDVERSION);
#endif
	DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, " This is free software: you are free to change and redistribute it.\n"
		" There is NO WARRANTY, to the extent permitted by law. \n"
		" Review LICENSE in " PRODUCTSHORT " distribution for details.\n");


    yatc_fopen_init(argv[0]);



	options.Load();
	MAXFPS = options.maxfps;



#if HAVE_LIBINTL_H
    // set up i18n stuff
    if(options.lang.size())
    {
        std::string l("LANG=");
        l+=options.lang;
        putenv((char*)l.c_str());
    }
    setlocale( LC_ALL, "");//options.lang.c_str() );
    setlocale( LC_NUMERIC, "C");
    bindtextdomain( "yatc", "./translations" ); // bindtextdomain( "hello", "/usr/share/locale" );
    textdomain( "yatc" );
    bind_textdomain_codeset("yatc","windows-1252");
#endif


	DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Checking graphics files existence...\n");
	checkFiles();
	DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "All graphics files were found.\n");


	DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Initializing windowing...\n");

	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0){
		std::stringstream out;
		out << "Couldn't initialize SDL: " << SDL_GetError() << std::endl;
		DEBUGPRINT(DEBUGPRINT_ERROR, DEBUGPRINT_LEVEL_OBLIGATORY, out.str().c_str());

		NativeGUIError(out.str().c_str(), PRODUCTSHORT " Fatal Error");
		exit(1);
	}

    // We are no longer dependant on .dat for this!
    // ivucica asks: nate, not dependant for what? why is this removed?
    // because of .ico? disagree.
	DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Loading data...\n");
	if(!Objects::getInstance()->loadDat("Tibia.dat")){
		DEBUGPRINT(DEBUGPRINT_ERROR, DEBUGPRINT_LEVEL_OBLIGATORY, gettext("Loading data file failed!"));
		std::string forreplace = gettext("Loading the data file '$$FILENAME$$' has failed.\nPlease place '$$FILENAME$$' in the same folder as $$PRODUCTSHORT$$.\n");
		forreplace = str_replace("$$FILENAME$$", "Tibia.dat", forreplace);
		forreplace = str_replace("$$PRODUCTSHORT$$", PRODUCTSHORT, forreplace);
		NativeGUIError(forreplace.c_str(), str_replace("$$PRODUCTSHORT$$", PRODUCTSHORT, gettext("$$PRODUCTSHORT$$ Fatal Error")).c_str());
		exit(1);
	}


	setIcon(); // must be called prior to first call to SDL_SetVideoMode() (currently done in engine)

	SDL_EnableKeyRepeat(200, 50);
	SDL_EnableUNICODE(1);

	try{
	    g_engine = NULL; // set to null, in case anything that happens inside engine constructor wants to know we're just constructing
		switch(options.engine){
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
			g_engine = NULL; // set to null, in case anything that happens inside engine constructor wants to know we're just constructing
			options.engine = ENGINE_SDL;
			g_engine = new EngineSDL;
		}

		// NOTE (nfries88): Make sure the window is sized as per the options
		int w = MAX(options.w, 656);
		int h = MAX(options.h, 352+options.consoleh);
		g_engine->doResize(w, h);


		DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Loading skin...\n");
		g_skin.loadSkin();
		DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Skin has been loaded\n");


		DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Constructing gamemode...\n");
		resetDefaultCursor();
		if (argc == 1)
		{
		    g_game = new GM_MainMenu();
            //g_game = new GM_Debug(); // ivucica: this is for testing -- choice should be a cmd line option
		} else
		{
		    g_game = new GM_MainMenu();
            ProtocolGame* protocol = ProtocolConfig::createGameProtocol(854,"","","",false);
            g_connection = new ConnectionReplay(argv[1], protocol);
            if (argc==3)
            {
                sscanf(argv[2], "%f", &g_replayspeed);
            }

		}


        DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Initializing framerate manager...\n");
        SDL_initFramerate(&g_fpsmgr);
        SDL_setFramerate(&g_fpsmgr,MAXFPS);


		DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Running\n");
        g_running = true;

        #ifdef WIN32
        SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);
        #endif

		SDL_Event event;
		while(g_running){

            //g_engine->fpsMutexLock();

            //int beginticks = SDL_GetTicks();
            g_engine->performFpsCalc();

			//first process sdl events
			while(SDL_PollEvent(&event)){
				switch (event.type){
					case SDL_VIDEORESIZE:
						g_engine->doResize(event.resize.w, event.resize.h);
						g_game->doResize(event.resize.w, event.resize.h);
						break;

					case SDL_QUIT:
						g_game->onExitAttempt();
						break;

					case SDL_KEYDOWN:
						onKeyDown(event);
						break;
                    case SDL_KEYUP:
                        if((event.key.keysym.sym == SDLK_LSUPER)
                          || (event.key.keysym.sym == SDLK_RSUPER))
                            superkey_state = false;
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
						g_game->mouseEvent(event);
						break;
					default:
						break;
				}
			}
			//update current frame time
			g_frameDiff = SDL_GetTicks() - g_frameTime;
			g_frameTime = SDL_GetTicks();


            if (MAXFPS) {
                SDL_framerateDelay(&g_fpsmgr);

            }

			//check connection
			if(g_connection){
				g_connection->executeNetwork();
			}

            if (!(SDL_GetAppState() & SDL_APPACTIVE)) {// if the application is minimized
                #ifdef WIN32
                Sleep(100); // sleep a while, and don't paint
                #else
                usleep(100 * 1000);
                #endif
            } else { //otherwise update scene
                g_game->updateScene();
                g_engine->Flip();
            }
			g_frames ++;


			//g_engine->fpsMutexUnlock();

		}
	}
	catch(std::string errtext){
		DEBUGPRINT(DEBUGPRINT_ERROR, DEBUGPRINT_LEVEL_OBLIGATORY, "%s", errtext.c_str());
	}

	DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Game over\n");

	DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Terminating protocol connection from main...\n");
	delete g_connection;
	g_connection = NULL;
	DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Destroying map...\n");
	Map::getInstance().clear();
	DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Destroying creature cache...\n");
	Creatures::getInstance().clear();
	Creatures::destroyInstance();
	DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Destroying inventory...\n");
	Inventory::getInstance().clear();

	DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Unloading data...\n");
	Objects::getInstance()->unloadDat();
    Objects::destroyInstance();

    DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Unloading skin...\n");
	g_skin.unloadSkin();


	DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Saving options...\n");
	options.Save();
	DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Finishing engine...\n");
	delete g_engine;
	DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Ending game...\n");
	delete g_game;
	DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Shutting down SDL...\n");
	SDL_Quit();

#ifdef WIN32
	WSACleanup();
#endif

	DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Thanks for playing!\n");

	return 0;
}
