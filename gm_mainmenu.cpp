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

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif

#if defined(HAVE_LIBINTL_H)
    #include <libintl.h>
#else
    #define gettext(x) (x)
#endif
#include "gm_mainmenu.h"
#include "gm_gameworld.h"
#include "gm_debug.h"
#include "defines.h"
#include "engine.h"
#include "sprite.h"
#include "options.h"
#include "product.h"
#include "clipboard.h"

#include "net/connection.h"
#include "net/protocollogin.h"
#include "net/protocolgame80.h"

extern yatcClipboard g_clipboard;

extern Connection* g_connection;
extern bool g_running;

extern bool superkey_state;


GM_MainMenu::GM_MainMenu()
{
	DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Starting main menu...\n");
    //resetDefaultCursor();
    SDL_SetCursor(g_engine->m_cursorBasic);
    mGotMotd = false;

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

    /* ********************** OTHERS ***************************** */
    #ifdef DEBUG_BUILD
    desktop.AddObject(&btnGoToDebug);
    btnGoToDebug.SetCaption("GM_Debug");
    btnGoToDebug.SetWidth(84);
    btnGoToDebug.SetHeight(20);
    btnGoToDebug.SetPos(16,16);
    btnGoToDebug.SetBGColor(.8,.8,.8,1.);
    btnGoToDebug.SetFont("minifont",8);
    btnGoToDebug.SetOnClick(btnGoToDebug_OnClick);

    #endif

    YATCFont* mf = (YATCFont*)(glictFindFont("minifont")->GetFontParam());
    mf->resetColor();
    mf = (YATCFont*)(glictFindFont("system")->GetFontParam());
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
    GameModeOptions::doResize(w,h);
	pnlMainMenu.mainmenu.SetPos(60, glictGlobals.h - 240);
	desktop.SetWidth(glictGlobals.w);
	desktop.SetHeight(glictGlobals.h);
	desktop.ResetTransformations();
	centerWindow(&winLogin.window);
	centerWindow(&winCharlist.window);
	centerWindow(&winStatus);

	renderScene();
}

void GM_MainMenu::renderScene()
{
    g_engine->resetClipping();
	if(background)
		background->Blit(0,0,0,0,background->getBasicWidth(),background->getBasicHeight(),glictGlobals.w, glictGlobals.h);

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
	int px, py;
	SDL_GetMouseState(&px, &py);
	pos.x = px;
	pos.y = py;

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

    if (dynamic_cast<GM_MainMenu*>(g_game))
        renderScene();

}

void GM_MainMenu::keyPress (int key)
{
	if(key == 0 || key == SDLK_LCTRL || key == SDLK_RCTRL) return; // don't pass these values to glict
	// NOTE (nfries88): I have no clue why, but I get 22 and 3 when pressing CTRL+V and CTRL+C respectively.
	if(ISPASTEEVENT(key))
	{
		std::string text = g_clipboard.getText();
		glictTextbox* textbox = dynamic_cast<glictTextbox*>(glictGlobals.topFocused);
		if(textbox) textbox->SetCaption(textbox->GetCaption()+text);
		renderUI();
		return;
	}
	else if(ISCOPYEVENT(key))
	{
	    // FIXME (nfries88): Selection is not available in glict,
            // so we are using the textbox's whole text.
        glictTextbox* textbox = dynamic_cast<glictTextbox*>(glictGlobals.topFocused);
        if(textbox) g_clipboard.setText(textbox->GetCaption());
		return;
	}
	else if(ISCUTEVENT(key)) // CTRL+X
    {
        // FIXME (nfries88): Selection is not available in glict,
            // so we are using the textbox's whole text.
		glictTextbox* textbox = dynamic_cast<glictTextbox*>(glictGlobals.topFocused);
		if(textbox){
		    g_clipboard.setText(textbox->GetCaption());
		    textbox->SetCaption("");
		}
		return;
	}
    // login assistance for login box
	else if (!options.ui_compat) {
		if (glictGlobals.topFocused == &winLogin.txtUsername && key == 13 && glictGlobals.topFocused->GetParent()->GetVisible())
			winLogin.btnOk.Focus(NULL);
		else if (glictGlobals.topFocused == &winLogin.txtPassword && key == 13 && glictGlobals.topFocused->GetParent()->GetVisible())
			winLogin.btnOk.Focus(NULL);
	} else {
		if (glictGlobals.topFocused == &winLogin.txtUsername && key == 13 && glictGlobals.topFocused->GetParent()->GetVisible()) {
			winLogin.txtPassword.Focus(NULL);
			renderUI();
			return;
		}
		else if (glictGlobals.topFocused == &winLogin.txtPassword && key == 13 && glictGlobals.topFocused->GetParent()->GetVisible())
			winLogin.btnOk.Focus(NULL);
	}
	if (glictGlobals.topFocused && glictGlobals.topFocused->GetParent() && (
        glictGlobals.topFocused == &winLogin.window || glictGlobals.topFocused->GetParent() == &winLogin.window) &&
        key == 27 && (glictGlobals.topFocused->GetVisible() && glictGlobals.topFocused->GetParent()->GetVisible())) {
        key=13;
        winCharlist.btnCancel.Focus(NULL);
        }

	// login assistance for charlist
    if (glictGlobals.topFocused == &winCharlist.lstChars && glictGlobals.topFocused->GetVisible()) {
        if (key == 13) {
            winCharlist.btnOk.Focus(NULL);
        } else
        if (key == 27) {
            winCharlist.btnCancel.Focus(NULL);
        }

    }

    if(superkey_state)
        NativeGUIError(yatc_itoa(key).c_str(), "!");

	desktop.CastEvent(GLICT_KEYPRESS, &key, 0);

	renderUI();
}

bool GM_MainMenu::specKeyPress (const SDL_keysym& key)
{
    // login assistance for charlist
	if (glictGlobals.topFocused == &winCharlist.lstChars) {
	    if (key.sym == SDLK_DOWN) {
            int32_t newchar = winCharlist.currentCharId+1;
            if (newchar >= (int32_t)winCharlist.lsiChars.size()) newchar = 0;
	        winCharlist_t::OnListbox(NULL, winCharlist.lsiChars[newchar]);
	        renderUI();
	        return true;
	    }
	    if (key.sym == SDLK_UP) {
            int32_t newchar = winCharlist.currentCharId-1;
            if (newchar < 0) newchar = winCharlist.lsiChars.size()-1;
	        winCharlist_t::OnListbox(NULL, winCharlist.lsiChars[newchar]);
	        renderUI();
	        return true;
	    }
	}
	if(SDL_GetModState() & KMOD_ALT && key.sym == SDLK_F4){
	    onExitAttempt();
	}
	return false;
}

void GM_MainMenu::onExitAttempt()
{
	// let our main loop handle this.
	g_running = false;
}


void GM_MainMenu::MBOnDismiss(glictPos* pos, glictContainer* caller)
{
	if (caller->GetCustomData()) {
		glictContainer* focusOnDismiss = (glictContainer*)caller->GetCustomData();
		focusOnDismiss->SetVisible(true);
		focusOnDismiss->Focus(NULL);
	}

	g_game->renderScene();
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
	m->winOptions.btnOk.Focus(NULL);
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
		<< gettext("Compiled on: ") << PRODUCTDATE << " " << PRODUCTTIME << "\n"
		<< "\n"
		<< c <<" 2007-2010 OpenTibia Team\n"
		<< "\n"
		<< gettext("Current engine") << ": " << g_engine->getName() << "\n"
		<< gettext("Uses SDL") << ": " << int(SDL_Linked_Version()->major) << "." << int(SDL_Linked_Version()->minor) << "." << int(SDL_Linked_Version()->patch) << " (" << gettext("compiled with") << ": " << int(sdl_compilever.major) <<"."<< int(sdl_compilever.minor) <<"."<< int(sdl_compilever.patch) << ")\n"
		<< gettext("Uses GLICT") << " API rev: " << int(GLICT_APIREV) << "\n"
		#ifdef USE_OPENGL
		<< gettext("Uses OpenGL") << ": ";
		if (g_engine->hasGL())
            txt << glGetString(GL_VENDOR)  << " " << glGetString(GL_VERSION) << "\n";
		else
            txt << gettext("[for info run with GL engine]\n");
        txt
		#endif
		<< "\n"
		<< gettext("Programmed by") << " " << gettext("(in no particular order)") << ":\n"
		<< b << " Ivan Vucica - Khaos\n"
		<< b << " Nate Fries - nfries88\n"
		<< b << " Alexandre Severino - Kilouco\n"
		<< gettext("with inactive developers") << ":\n"
		<< b << " mips\n"
		<< "\n"
		<< gettext("Contributors") << ":\n"
		<< b << " mrauter\n"
		<< b << " Mariusz Gliwinski - shd\n"
		<< b << " Vindeamatrix510\n"
		<< b << " FightingElf\n"
		<< b << " Magnus KL - Smygflik\n"
		<< b << " Alexandru Pitis - Ormin\n"
                << b << " Adelae\n"
		<< "\n"
		<< PRODUCTSHORT << gettext(" comes with ABSOLUTELY NO WARRANTY; \n"
		   "for details see sections 11 and 12 in file COPYING.\n"
		   "This is free software, and you are welcome \n"
		   "to redistribute it under certain conditions;\n"
		   "see file COPYING for details.");

	m->msgBox(txt.str().c_str(), gettext("Info"), &m->pnlMainMenu.btnAbout);

}

void GM_MainMenu::pnlMainMenu_btnExit_OnClick(glictPos* relmousepos, glictContainer* callerclass)
{
	g_running = 0;
}
void GM_MainMenu::btnHelp_OnClick(glictPos* relmousepos, glictContainer* callerclass) {
	std::string *helptext = (std::string*)callerclass->GetCustomData();
	GM_MainMenu* m = (GM_MainMenu*)g_game;
	m->msgBox(helptext->c_str(), gettext("Help"), callerclass);
}

/* **********LOGIN******* */
void GM_MainMenu::winLogin_btnOk_OnClick(glictPos* relmousepos, glictContainer* callerclass)
{
	GM_MainMenu* m = (GM_MainMenu*)g_game;
	m->winLogin.window.SetVisible(false);
	options.account = m->winLogin.txtUsername.GetCaption();
	options.password = m->winLogin.txtPassword.GetCaption();
	m->login(options.account, options.password);

	m->mGotMotd=false;
}

void GM_MainMenu::login(const std::string& username, const std::string& password)
{
	if(g_connection){
	    printf("Destroying connection from mainmenu for login\n");
		delete g_connection;
		g_connection = NULL;
	}

	ClientVersion_t proto = getActiveProtocol();
	if(!proto)
		return;

	ProtocolConfig::getInstance().setServerType(options.otkey ? SERVER_OTSERV : SERVER_CIP ); // perhaps this should go to options, too?
	ProtocolConfig::getInstance().setVersion(CLIENT_OS_WIN, proto); // TODO (ivucica#3#) see if we can freely tell the 'real' OS, meaning "linux" on unices, and "windows" on windows
	ProtocolConfig::getInstance().setVersionOverride(options.overrideversion);
	ProtocolConfig::getInstance().setServer(options.server, options.port);

    std::string text;
	ProtocolConfig::createLoginConnection(username, password);
	winStatus.SetCaption(gettext("Connecting"));
	winStatus.SetMessage(text=gettext("Your character list is being loaded. Please wait."));
	#if (GLICT_APIREV >= 85)
	winStatus.SetTextOffset(10,10);
	#else
	#warning For nicer msgboxes get GLICT APIREV 85+.
	#endif
	#if (GLICT_APIREV >= 2020010401)
	winStatus.SetButtonCaption(gettext("Ok"));
	winStatus.SetButtonFont("minifont", 8);
	winStatus.EnableSeparator(&g_skin.chk, 2, 6, 8, .1, .1, .1, 1.0);
	winStatus.SetButtonHeight(20);
	winStatus.SetButtonMarginY(5);
	winStatus.SetButtonMarginX(5);
	winStatus.SetButtonAlignment(GLICT_VALIGN_RIGHT);
	#else
	#warning For nicer msgboxes get GLICT APIREV 2020010401+.
	#endif

    winStatus.SetWidth(glictFontSize(text.c_str(), "system") + 2*10); // 10 = size of text margin
	winStatus.SetHeight(glictFontNumberOfLines(text.c_str())*12 + 50);
	winStatus.SetEnabled(false);
	centerWindow(&winStatus);
	desktop.AddObject(&winStatus);

    updateScene();
    renderScene();
    g_engine->Flip();
    //DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "SetVisible...\n");
}
ClientVersion_t GM_MainMenu::getActiveProtocol(){
	ClientVersion_t proto = options.protocol;
	if (!proto)
		proto = ProtocolConfig::detectVersion();

	if (!proto) {

		std::string t;
		//unsigned char c[2] = { 149, 0} ; // bullet
		t = gettext("Data files in the directory either:\n"
		"* do not belong to same protocol version, or\n"
		"* are modified and with unknown signatures.\n"
		"\n"
		"Either get correct files from a single known\n"
		"client version, or make sure that you have\n"
		"correct signatures in the files, or go to\n"
		"Options->Network and choose the correct\n"
		"protocol.");
		t = str_replace("*", "\x95", t);
		msgBox(t.c_str(),gettext("Protocol detection failed"));

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
	ProtocolConfig::createGameConnection(options.account, options.password, m->winCharlist.currentChar.name, false /* isgm*/);

    std::string text;
	m->desktop.AddObject(&m->winStatus);
	m->centerWindow(&m->winStatus);
	m->winStatus.SetCaption(gettext("Connecting"));
	m->winStatus.SetMessage(text=gettext("Connecting to the game world. Please wait."));
    m->winStatus.SetWidth(glictFontSize(text.c_str(), "system") + 2*10); // 10 = size of text margin
	m->winStatus.SetHeight(glictFontNumberOfLines(text.c_str())*12 + 50);
	m->centerWindow(&m->winStatus);
}

void GM_MainMenu::winCharlist_btnCancel_OnClick(glictPos* relmousepos, glictContainer* callerclass)
{
	GM_MainMenu* m = (GM_MainMenu*)g_game;
	m->winCharlist.destroyList();
	m->winCharlist.window.SetVisible(false);
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
void GM_MainMenu::btnGoToDebug_OnClick(glictPos* relmousepos, glictContainer* callerclass)
{
    #ifdef DEBUG_BUILD
    delete g_game;
    g_game = new GM_Debug;
    #endif
}


/* ********** Responses to notifications *********** */
void GM_MainMenu::onConnectionError(int message, const char* errortext)
{
	std::stringstream s;
	s << gettext("Cannot connect to a login server.") << std::endl << // FIXME (ivucica#5#): should print different message for gameworld serv;
		std::endl <<
		gettext("Error") << ": " << errortext << std::endl <<
	     	std::endl <<
		gettext("For more information take a look at the "
		// FAQs in the\n Support section at yetanother.tibiaclient.com.
		"threads in the\n"
		"forums at otfans.net.");

	winStatus.SetCaption(gettext("Error"));
	winStatus.SetMessage(s.str());
	winStatus.SetEnabled(true);
    winStatus.SetWidth(glictFontSize(s.str().c_str(), "system") + 2*10); // 10 = size of text margin
	winStatus.SetHeight(glictFontNumberOfLines(s.str().c_str())*12 + 50);
	centerWindow(&winStatus);
	renderScene();
}
void GM_MainMenu::openMOTD(int motdnum, const std::string& text)
{
    mGotMotd=true;
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

    std::string displayText = text;
    {
        // translating motd
        displayText = str_replace("Welcome to Tibia!", gettext("Welcome to Tibia!"), displayText);
        displayText = str_replace("Due to a technical problem we had to reset\nthe game world", gettext("Due to a technical problem we had to reset\nthe game world"), displayText);
        displayText = str_replace("to the state of", gettext("to the state of"), displayText);
        displayText = str_replace("We are sorry for any inconvenience. Of course we\nwork hard to prevent such problems in the future.", gettext("We are sorry for any inconvenience. Of course we\nwork hard to prevent such problems in the future."), displayText);
        displayText = str_replace("For more information about Tibia visit our\nwebsite at", gettext("For more information about Tibia visit our\nwebsite at"), displayText);
        displayText = str_replace("Have fun in Tibia!", gettext("Have fun in Tibia!"), displayText);
        displayText = str_replace("Welcome to", gettext("Welcome to"), displayText);
        displayText = str_replace("Have fun in", gettext("Have fun in"), displayText);
        displayText = str_replace("Important Reminder", gettext("Important Reminder"), displayText);
    }

	winStatus.SetCaption(gettext("Message of the Day"));
	winStatus.SetMessage(displayText);
	winStatus.SetWidth(MAX(
		glictFontSize(gettext("Message of the Day"), "system"),
		glictFontSize(displayText.c_str(), "system")) + 2*10); // 10 = size of text margin
	winStatus.SetHeight(glictFontNumberOfLines(displayText.c_str())*12 + 50);
    winStatus.SetEnabled(true);
	winStatus.Focus(NULL);
	this->centerWindow(&winStatus);

	winStatus.SetOnDismiss(winMotd_OnDismiss);
	renderScene();

}
void GM_MainMenu::openMessageWindow(WindowMessage_t type, const std::string& text)
{
	if (type == MESSAGE_ERROR)
		winStatus.SetCaption(gettext("Error"));
	else
		winStatus.SetCaption(gettext("Information"));

	winStatus.SetMessage(text);
	winStatus.SetEnabled(true);
	winStatus.Focus(NULL);
	winStatus.SetOnDismiss(winStatus_ErrorOnDismiss);
    winStatus.SetWidth(glictFontSize(text.c_str(), "system") + 2*10); // 10 = size of text margin
	winStatus.SetHeight(glictFontNumberOfLines(text.c_str())*12 + 50);
	centerWindow(&winStatus);

	renderScene();
}

void GM_MainMenu::openCharactersList(const std::list<CharacterList_t>& list, int premDays)
{
    winCharlist.destroyList();
	winCharlist.generateList(list, premDays);
	if (!mGotMotd)
	{
        winCharlist.window.SetVisible(true);
        centerWindow(&winCharlist.window);
        winStatus.SetVisible(false);
	}
	renderUI();
}

void GM_MainMenu::onEnterGame()
{
    //s_alreadyloggedinonce = true;
    printf("Destroying list\n");
    winCharlist.destroyList();
    printf("Destroyed list\n");
	delete g_game;
	printf("Entering gameworld\n");
	g_game = new GM_Gameworld;
	printf("Entered\n");
}
