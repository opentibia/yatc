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

#ifndef __GM_MAINMENU_H
#define __GM_MAINMENU_H

#include <GLICT/container.h>
#include <GLICT/button.h>
#include <GLICT/panel.h>
#include <GLICT/textbox.h>
#include <GLICT/window.h>
#include <GLICT/messagebox.h>
#include "options.h"
#include "gamemode.h"
#include "sprite.h"

#include "skin.h"
#include "ui/mainmenu.h"
#include "ui/login.h"
#include "ui/charlist.h"
#include "ui/options.h"
#include "ui/optionsgeneral.h"
#include "ui/optionsgraphics.h"
#include "ui/optionsnetwork.h"

class GM_MainMenu : public GameMode
{
public:
	GM_MainMenu();
	~GM_MainMenu();

	void renderScene();
	void mouseEvent (SDL_Event&);
	void keyPress (char key);
	void doResize(float w, float h);

	void msgBox (const char* mbox, const char* title);
	void centerWindow (glictWindow *win);

	void onConnectionError(int message, const char* text);
	void openMessageWindow(WindowMessage_t type, const std::string& message);
	void openMOTD(int motdnum, const std::string& message);
	void openCharactersList(const std::list<CharacterList_t>& list, int premDays);

	void onEnterGame();

private:
	glictContainer desktop;
	pnlMainMenu_t pnlMainMenu;
	winLogin_t winLogin;
	winCharlist_t winCharlist;
	winOptions_t winOptions;
	winOptionsGeneral_t winOptionsGeneral;
	winOptionsGraphics_t winOptionsGraphics;
	winOptionsNetwork_t winOptionsNetwork;


	glictMessageBox winStatus;

	Sprite* background;
protected:
	static void btnHelp_OnClick(glictPos* relmousepos, glictContainer* callerclass);

	static void pnlMainMenu_btnLogIn_OnClick(glictPos* relmousepos, glictContainer* callerclass);
	static void pnlMainMenu_btnOptions_OnClick(glictPos* relmousepos, glictContainer* callerclass);
	static void pnlMainMenu_btnAbout_OnClick(glictPos* relmousepos, glictContainer* callerclass);
	static void pnlMainMenu_btnExit_OnClick(glictPos* relmousepos, glictContainer* callerclass);

	static void winLogin_btnOk_OnClick(glictPos* relmousepos, glictContainer* callerclass);
	static void winLogin_btnCancel_OnClick(glictPos* relmousepos, glictContainer* callerclass);

	static void winCharlist_btnOk_OnClick(glictPos* relmousepos, glictContainer* callerclass);
	static void winCharlist_btnCancel_OnClick(glictPos* relmousepos, glictContainer* callerclass);

	static void winOptions_btnGeneral_OnClick(glictPos* relmousepos, glictContainer* callerclass);
	static void winOptions_btnGraphics_OnClick(glictPos* relmousepos, glictContainer* callerclass);
	static void winOptions_btnNetwork_OnClick(glictPos* relmousepos, glictContainer* callerclass);
	static void winOptions_btnMotd_OnClick(glictPos* relmousepos, glictContainer* callerclass);
	static void winOptions_btnOk_OnClick(glictPos* relmousepos, glictContainer* callerclass);

	static void winOptionsGeneral_btnOk_OnClick(glictPos* relmousepos, glictContainer* callerclass);
	static void winOptionsGeneral_btnCancel_OnClick(glictPos* relmousepos, glictContainer* callerclass);

	static void winOptionsGraphics_btnOk_OnClick(glictPos* relmousepos, glictContainer* callerclass);
	static void winOptionsGraphics_btnCancel_OnClick(glictPos* relmousepos, glictContainer* callerclass);

	static void winOptionsNetwork_btnOk_OnClick(glictPos* relmousepos, glictContainer* callerclass);
	static void winOptionsNetwork_btnCancel_OnClick(glictPos* relmousepos, glictContainer* callerclass);


	static void winMotd_OnDismiss(glictPos* relmousepos, glictContainer* callerclass);


	static void MBOnDismiss(glictPos* pos, glictContainer* caller);

	friend class Notifications;
};

#endif

