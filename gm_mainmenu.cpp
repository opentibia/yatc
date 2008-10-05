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

#include <GLICT/globals.h>
#include <GLICT/messagebox.h>

#include "gm_mainmenu.h"
#include "gm_gameworld.h"
#include "defines.h"
#include "engine.h"
#include "sprite.h"
#include "options.h"
#include "product.h"

#include "net/connection.h"
#include "net/protocollogin.h"
#include "net/protocolgame80.h"

extern Connection* g_connection;
extern bool g_running;


void resetDefaultCursor();

GM_MainMenu::GM_MainMenu()
{
	DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Starting main menu...\n");
    //resetDefaultCursor();
    SDL_SetCursor(g_engine->m_cursorBasic);

    /* ****************** MAIN MENU *********************** */
	desktop.AddObject(&pnlMainMenu.mainmenu);
	pnlMainMenu.btnLogIn.SetOnClick(GM_MainMenu::pnlMainMenu_btnLogIn_OnClick);
	pnlMainMenu.btnNetwork.SetOnClick(GM_MainMenu::winOptions_btnNetwork_OnClick); // FIXME
	pnlMainMenu.btnOptions.SetOnClick(GM_MainMenu::pnlMainMenu_btnOptions_OnClick);
	pnlMainMenu.btnAbout.SetOnClick(GM_MainMenu::pnlMainMenu_btnAbout_OnClick);
	pnlMainMenu.btnExit.SetOnClick(GM_MainMenu::pnlMainMenu_btnExit_OnClick);

	/* ****************** LOGIN SCREEN *********************** */
	desktop.AddObject(&winLogin.window);
	winLogin.btnOk.SetOnClick(GM_MainMenu::winLogin_btnOk_OnClick);
	winLogin.btnCancel.SetOnClick(GM_MainMenu::winLogin_btnCancel_OnClick);

	/* ****************** CHARLIST SCREEN *********************** */
	desktop.AddObject(&winCharlist.window);
	winCharlist.btnOk.SetOnClick(GM_MainMenu::winCharlist_btnOk_OnClick);
	winCharlist.btnCancel.SetOnClick(GM_MainMenu::winCharlist_btnCancel_OnClick);

	/* ********************** OPTIONS *************************** */
    winOptions.initiateAll(&desktop);


    Font* mf = (Font*)(glictFindFont("minifont")->GetFontParam());
    mf->resetColor();
    mf = (Font*)(glictFindFont("system")->GetFontParam());
    mf->addColor(.75,.75,.75);

	pnlMainMenu.btnLogIn.Focus(NULL);

	if (options.ui_compat) {
		glictGlobals.drawFocus = true;
	} else {
		glictGlobals.drawFocus = false;
	}

	if (g_engine) {
		background = g_engine->createSprite("Tibia.pic",0);
		if(!background->isLoaded()){
			delete background;
			background = NULL;
		}
	}
	else{
		background = NULL;
	}

	doResize(glictGlobals.w, glictGlobals.h);

	renderScene();
}

GM_MainMenu::~GM_MainMenu()
{
	delete background;
}


void GM_MainMenu::updateScene()
{
    if (g_engine->hasGL()) {
        renderScene();
    }
}

void GM_MainMenu::doResize(float w, float h)
{
	pnlMainMenu.mainmenu.SetPos(60, glictGlobals.h - 240);
	desktop.SetWidth(glictGlobals.w);
	desktop.SetHeight(glictGlobals.h);
	desktop.ResetTransformations();
	centerWindow(&winLogin.window);
	centerWindow(&winCharlist.window);
	centerWindow(&winOptions.window);
	centerWindow(&winOptions.winOptionsGeneral.window);
	centerWindow(&winOptions.winOptionsConsole.window);
	centerWindow(&winOptions.winOptionsGraphics.window);
	centerWindow(&winOptions.winOptionsGraphicsAdvanced.window);
	centerWindow(&winOptions.winOptionsHotkeys.window);
	centerWindow(&winOptions.winOptionsNetwork.window);
	centerWindow(&winStatus);

	renderScene();
}

void GM_MainMenu::renderScene()
{
	if(background)
		background->Blit(0,0,0,0,background->getWidth(),background->getHeight(),glictGlobals.w, glictGlobals.h);

	renderUI();
}

void GM_MainMenu::renderUI()
{
#ifdef USE_OPENGL
	if (g_engine->hasGL()) {
		glEnable(GL_SCISSOR_TEST);
		desktop.Paint();
		glDisable(GL_SCISSOR_TEST);
	} else {
		desktop.Paint();
	}
#else
	desktop.Paint();
#endif
}
void GM_MainMenu::mouseEvent(SDL_Event& event)
{
	glictPos pos;
	// FIXME (ivucica#3#) this is incorrect, we should be refreshing ptrx and ptry here as well, not just read the old versions ...
	// who knows how the platforms with a different pointing device (e.g. touchscreen?) would behave!
	// (above is because we handle mousedown and mouseup too, and not ust mousemove)
	pos.x = ptrx;
	pos.y = ptry;

	desktop.TransformScreenCoords(&pos);

    if (event.type != SDL_MOUSEMOTION) {
        if (event.button.state == SDL_PRESSED)
            desktop.CastEvent(GLICT_MOUSEDOWN, &pos, 0);
        else
            desktop.CastEvent(GLICT_MOUSEUP, &pos, 0);
    } else {
        #if (GLICT_APIREV >= 67)
        desktop.CastEvent(GLICT_MOUSEMOVE, &pos, 0);
        #else
        #warning We need GLICT apirev 67 or greater to support basic movable windows.
        #endif
    }
    g_engine->resetClipping();

	renderScene();

}

void GM_MainMenu::keyPress (char key)
{
	//printf("Key %c %d\n", key, key);
	if (!options.ui_compat) {
		if (glictGlobals.topFocused == &winLogin.txtUsername && key == 13)
			winLogin.btnOk.Focus(NULL);
		else if (glictGlobals.topFocused == &winLogin.txtPassword && key == 13)
			winLogin.btnOk.Focus(NULL);
	} else {
		if (glictGlobals.topFocused == &winLogin.txtUsername && key == 13) {
			winLogin.txtPassword.Focus(NULL);
			renderUI();
			return;
		}
		else if (glictGlobals.topFocused == &winLogin.txtPassword && key == 13)
			winLogin.btnOk.Focus(NULL);

	}

	desktop.CastEvent(GLICT_KEYPRESS, &key, 0);
	renderUI();
}

void GM_MainMenu::centerWindow (glictWindow *win)
{
	glictSize s;
	win->GetSize(&s);
	win->SetPos(glictGlobals.w / 2 - s.w / 2, glictGlobals.h/2 - s.h / 2);
}

void GM_MainMenu::msgBox (const char* mbox, const char* title, glictContainer* focusondismiss)
{
	glictSize s;
	glictMessageBox *mb;
	desktop.AddObject(mb = new glictMessageBox);

	mb->SetCaption(title);
	mb->SetMessage(mbox);

	mb->SetHeight(glictFontNumberOfLines(mbox)*12 + 35);
	int size1 = (int)glictFontSize(title, "system");
	int size2 = (int)glictFontSize(mbox, "system");
	mb->SetWidth(MAX(size1, size2));
	mb->Focus(NULL);

	mb->GetSize(&s);

	mb->SetPos(glictGlobals.w / 2 - s.w / 2, glictGlobals.h/ 2 - s.h / 2);

	mb->SetOnDismiss(GM_MainMenu::MBOnDismiss);

	mb->SetCustomData(focusondismiss);

}

void GM_MainMenu::MBOnDismiss(glictPos* pos, glictContainer* caller)
{
	GM_MainMenu* m = (GM_MainMenu*)g_game;
	if (caller->GetCustomData()) {
		glictContainer* focusOnDismiss = (glictContainer*)caller->GetCustomData();
		focusOnDismiss->SetVisible(true);
		focusOnDismiss->Focus(NULL);
	}
	DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Repaint.\n");
	m->renderScene();
	//delete caller;
}

//static bool s_alreadyloggedinonce = false; //  REMOVEME this serves only to prevent people from logging in twice in same session since this crashes the game
/* ****** EVENT CALLBACKS ****** */
void GM_MainMenu::pnlMainMenu_btnLogIn_OnClick(glictPos* relmousepos, glictContainer* callerclass)
{
	GM_MainMenu* m = (GM_MainMenu*)g_game;

    //if (s_alreadyloggedinonce && !options.ui_compat) {
        //m->msgBox("Sorry, you can't log in twice due to a bug in the client.", "Bug in " PRODUCTSHORT " :(");
        //return;
    //}
	
	
	
	ClientVersion_t proto = m->getActiveProtocol();
	if(!proto)
		return;
	
	if (proto >= 830)
		m->winLogin.permitAccountName(true);
	else
		m->winLogin.permitAccountName(false);
	
	
	m->winLogin.window.SetVisible(true);
	m->winLogin.txtUsername.SetCaption(options.account);
	m->winLogin.txtPassword.SetCaption(options.password);
	m->winLogin.txtUsername.Focus(NULL);
	
}

void GM_MainMenu::pnlMainMenu_btnOptions_OnClick(glictPos* relmousepos, glictContainer* callerclass)
{
	GM_MainMenu* m = (GM_MainMenu*)g_game;
	m->winOptions.window.SetVisible(true);
	m->winOptions.window.Focus(NULL);
}

void GM_MainMenu::pnlMainMenu_btnAbout_OnClick(glictPos* relmousepos, glictContainer* callerclass)
{
	std::stringstream txt;
	unsigned char c = 169;
    unsigned char b = 149; // bullet

	GM_MainMenu* m = (GM_MainMenu*)g_game;
	SDL_version sdl_compilever;
	SDL_VERSION(&sdl_compilever);
	txt << PRODUCTLONG << "\n"
		<< PRODUCTVERSION << "\n"
		<< "\n"
		<< c <<" 2007-2008 OpenTibia Team\n"
		<< "\n"
		<< "Current engine: " << g_engine->getName() << "\n"
		<< "Uses SDL: " << int(SDL_Linked_Version()->major) << "." << int(SDL_Linked_Version()->minor) << "." << int(SDL_Linked_Version()->patch) << " (compiled with: " << int(sdl_compilever.major) <<"."<< int(sdl_compilever.minor) <<"."<< int(sdl_compilever.patch) << ")\n"
		<< "Uses GLICT API rev: " << int(GLICT_APIREV) << "\n"
		#ifdef USE_OPENGL
		<< "Uses OpenGL: ";
		if (g_engine->hasGL())
            txt << glGetString(GL_VENDOR)  << " " << glGetString(GL_VERSION) << "\n";
		else
            txt << "[for info run with GL engine]\n";
        txt
		#endif
		<< "\n"
		<< "Programmed by (in no particular order):\n"
		<< b << " Ivan Vucica - Khaos\n"
		<< b << " Nate Fries - nfries88\n"
		<< "with inactive developers:\n"
		<< b << " mips\n"
		<< b << " Magnus KL - Smygflik\n"
		<< "\n"
		<< "Contributors:\n"
		<< b << " mrauter\n"
		<< b << " Mariusz Gliwinski - shd\n"
		<< "\n"
		<< PRODUCTSHORT << " comes with ABSOLUTELY NO WARRANTY; \n"
		<< "for details see sections 11 and 12 in COPYING.\n"
		<< "This is free software, and you are welcome \n"
		<< "to redistribute it under certain conditions;\n"
		<< "see COPYING for details.";

	m->msgBox(txt.str().c_str(), "About " PRODUCTSHORT, &m->pnlMainMenu.btnAbout);

}

void GM_MainMenu::pnlMainMenu_btnExit_OnClick(glictPos* relmousepos, glictContainer* callerclass)
{
	g_running = 0;
}
void GM_MainMenu::btnHelp_OnClick(glictPos* relmousepos, glictContainer* callerclass) {
	std::string *helptext = (std::string*)callerclass->GetCustomData();
	GM_MainMenu* m = (GM_MainMenu*)g_game;
	m->msgBox(helptext->c_str(), "Help", callerclass);
}

/* **********LOGIN******* */
void GM_MainMenu::winLogin_btnOk_OnClick(glictPos* relmousepos, glictContainer* callerclass)
{
	GM_MainMenu* m = (GM_MainMenu*)g_game;
	m->winLogin.window.SetVisible(false);

	if(g_connection){
	    printf("Destroying connection from mainmenu for login\n");
		delete g_connection;
		g_connection = NULL;
	}

	ClientVersion_t proto = m->getActiveProtocol();
	if(!proto)
		return;
	
	ProtocolConfig::getInstance().setServerType(options.otkey ? SERVER_OTSERV : SERVER_CIP ); // perhaps this should go to options, too?
	ProtocolConfig::getInstance().setVersion(CLIENT_OS_WIN, proto); // TODO (ivucica#3#) see if we can freely tell the 'real' OS, meaning "linux" on unices, and "windows" on windows
	ProtocolConfig::getInstance().setVersionOverride(options.overrideversion);
	ProtocolConfig::getInstance().setServer(options.server, options.port);

	ProtocolConfig::createLoginConnection(m->winLogin.txtUsername.GetCaption(), m->winLogin.txtPassword.GetCaption());
	m->winStatus.SetCaption("Logging in");
	m->winStatus.SetMessage("Connecting to the server...\n");
	m->winStatus.SetWidth(320);
	m->winStatus.SetEnabled(false);
	m->centerWindow(&m->winStatus);
	m->desktop.AddObject(&m->winStatus);

	//DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "SetVisible...\n");
}
ClientVersion_t GM_MainMenu::getActiveProtocol(){
	ClientVersion_t proto = options.protocol;
	if (!proto)
		proto = ProtocolConfig::detectVersion();
	
	if (!proto) {
		
		std::stringstream t;
		unsigned char c = 149; // bullet
		t << "Data files in the directory either:\n" <<
		c << "do not belong to same protocol version, or\n" <<
		c << "are modified and with unknown signatures.\n" <<
		"\n" <<
		"Either get correct files from a single known\n" <<
		"client version, or make sure that you have\n" <<
		"correct signatures in the files, or go to\n" <<
		"Options->Network and choose the correct\n" <<
		"protocol.";
		msgBox(t.str().c_str(),"Protocol detection failed");
		
	}
	return proto;
}

void GM_MainMenu::winLogin_btnCancel_OnClick(glictPos* relmousepos, glictContainer* callerclass)
{
	GM_MainMenu* m = (GM_MainMenu*)g_game;
	m->winLogin.window.SetVisible(false);
}

/* **********CHARLIST******* */
void GM_MainMenu::winCharlist_btnOk_OnClick(glictPos* relmousepos, glictContainer* callerclass)
{
	// FIXME (ivucica#1#) mips should check if this switching to gameworld is written properly

	GM_MainMenu* m = (GM_MainMenu*)g_game;
	uint32_t ipnum = m->winCharlist.currentChar.ip;
	std::stringstream ips;
	std::string ip;

	if(!m->winCharlist.lsiChars.size()) {
		//printf("This account has no characters!\n");
		m->winCharlist.window.SetVisible(false);
		return;
	}

	ips << (ipnum & 0xFF) << "." << ((ipnum & 0xFF00) >> 8) << "." << ((ipnum & 0xFF0000) >> 16) << "." << ((ipnum & 0xFF000000) >> 24);
	ip = ips.str();

	m->winCharlist.window.SetVisible(false);
	if(g_connection){
	    printf("Destroying connection from main menu for entering game\n");
		delete g_connection;
		g_connection = NULL;
	}

	ClientVersion_t proto = options.protocol;
	if (!proto) proto = ProtocolConfig::detectVersion();
	ASSERT(proto);

	ProtocolConfig::getInstance().setVersion(CLIENT_OS_WIN, proto); // TODO (ivucica#3#) see if we can freely tell the 'real' OS, meaning "linux" on unices, and "windows" on windows
	ProtocolConfig::getInstance().setServer(ip, m->winCharlist.currentChar.port);
	ProtocolConfig::getInstance().setVersionOverride(options.overrideversion);
	ProtocolConfig::createGameConnection(m->winLogin.txtUsername.GetCaption(), m->winLogin.txtPassword.GetCaption(), m->winCharlist.currentChar.name, false /* isgm*/);

	m->desktop.AddObject(&m->winStatus);
	m->centerWindow(&m->winStatus);
	m->winStatus.SetCaption("Entering game");
	m->winStatus.SetMessage("Connecting to the server...");

}

void GM_MainMenu::winCharlist_btnCancel_OnClick(glictPos* relmousepos, glictContainer* callerclass)
{
	GM_MainMenu* m = (GM_MainMenu*)g_game;
	m->winCharlist.destroyList();
	m->winCharlist.window.SetVisible(false);
}

/* ************NETWORK*************** */
void GM_MainMenu::winOptions_btnNetwork_OnClick(glictPos* relmousepos, glictContainer* callerclass)
{
	winOptions_t* winOptions = g_game->getOptionsWindow();
	winOptions->winOptionsNetwork.Init();
	winOptions->winOptionsNetwork.window.SetVisible(true);
	winOptions->winOptionsNetwork.window.Focus(NULL);
}
void GM_MainMenu::winOptionsNetwork_btnOk_OnClick(glictPos* relmousepos, glictContainer* callerclass)
{
	winOptions_t* winOptions = g_game->getOptionsWindow();
	winOptions->winOptionsNetwork.Store();
	//((*GM_MainMenu)g_game)->desktop.SetFocus(NULL);
	winOptions->winOptionsNetwork.window.SetVisible(false);
}
void GM_MainMenu::winOptionsNetwork_btnCancel_OnClick(glictPos* relmousepos, glictContainer* callerclass)
{
	winOptions_t* winOptions = g_game->getOptionsWindow();
	//((*GM_MainMenu)g_game)->desktop.SetFocus(NULL);
	winOptions->winOptionsNetwork.window.SetVisible(false);
}

/* ************* OTHER **************** */
void GM_MainMenu::winMotd_OnDismiss(glictPos* relmousepos, glictContainer* callerclass)
{
	GM_MainMenu* m = (GM_MainMenu*)g_game;
	m->winCharlist.window.SetVisible(true);
	m->renderScene();
}
void GM_MainMenu::winStatus_ErrorOnDismiss(glictPos* relmousepos, glictContainer* callerclass)
{
	GM_MainMenu* m = (GM_MainMenu*)g_game;
	m->pnlMainMenu.btnLogIn.Focus(NULL);
	m->renderScene();

}

/* ********** Responses to notifications *********** */
void GM_MainMenu::onConnectionError(int message, const char* errortext)
{
	std::stringstream s;
	s << //"There was an error while connecting." << std::endl <<
		 //std::endl <<
	     //"The following may help you identify error:" << std::endl <<
	     errortext;

	winStatus.SetCaption("Error");
	winStatus.SetMessage(s.str());
	winStatus.SetEnabled(true);
	renderScene();
}
void GM_MainMenu::openMOTD(int motdnum, const std::string& text)
{

	if (options.motdnum == motdnum) {
		DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Motd numbers matching, not displaying motd.\n");
		desktop.RemoveObject(&winStatus);
		options.motdtext = text;
		winMotd_OnDismiss(NULL, NULL);
		renderScene();
		return;
	}

	options.motdnum = motdnum;
	options.motdtext = text;
	options.Save();

	winStatus.SetCaption("Message of the Day");
	winStatus.SetMessage(text);
	winStatus.SetEnabled(true);
	winStatus.Focus(NULL);

	winStatus.SetOnDismiss(winMotd_OnDismiss);
	renderScene();

}
void GM_MainMenu::openMessageWindow(WindowMessage_t type, const std::string& text)
{
	if (type == MESSAGE_ERROR)
		winStatus.SetCaption("Error");
	else
		winStatus.SetCaption("Information");

	winStatus.SetMessage(text);
	winStatus.SetEnabled(true);
	winStatus.Focus(NULL);
	winStatus.SetOnDismiss(winStatus_ErrorOnDismiss);
	renderScene();
}

void GM_MainMenu::openCharactersList(const std::list<CharacterList_t>& list, int premDays)
{
    winCharlist.destroyList();
	winCharlist.generateList(list, premDays);
	renderUI();
}

void GM_MainMenu::onEnterGame()
{
    //s_alreadyloggedinonce = true;
    winCharlist.destroyList();

	delete g_game;
	g_game = new GM_Gameworld;
}
