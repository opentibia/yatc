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
#include "defines.h"
#include "engine.h"
#include "sprite.h"

extern bool g_running;

GM_MainMenu::GM_MainMenu()
{
	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Starting main menu...\n");

	/* ****************** MAIN MENU *********************** */
	desktop.AddObject(&pnlMainMenu.mainmenu);
	pnlMainMenu.btnLogIn.SetOnClick(GM_MainMenu::pnlMainMenu_btnLogIn_OnClick);
	pnlMainMenu.btnOptions.SetOnClick(GM_MainMenu::pnlMainMenu_btnOptions_OnClick);
	pnlMainMenu.btnAbout.SetOnClick(GM_MainMenu::pnlMainMenu_btnAbout_OnClick);
	pnlMainMenu.btnExit.SetOnClick(GM_MainMenu::pnlMainMenu_btnExit_OnClick);


	/* ****************** LOGIN SCREEN *********************** */
	desktop.AddObject(&winLogin.window);
	winLogin.btnCancel.SetOnClick(GM_MainMenu::winLogin_btnCancel_OnClick);

	/* ******************* OPTIONS ************************* */
	desktop.AddObject(&winOptions.window);
	winOptions.btnOk.SetOnClick(GM_MainMenu::winOptions_btnOk_OnClick);

	// TODO (ivucica#1#) These should be moved to an "onresize" function which should be called here, but we don't catch a window resize event yet...
	pnlMainMenu.mainmenu.SetPos(60, glictGlobals.h - 240);
	desktop.SetWidth(glictGlobals.w);
	desktop.SetHeight(glictGlobals.h);
	desktop.ResetTransformations();


	if (g_engine) {
		background = g_engine->createSprite("yatc.bmp");
	}
	else {
		background = NULL;
	}
}

GM_MainMenu::~GM_MainMenu()
{

}

void GM_MainMenu::renderScene()
{
	if (background)
		background->Blit(0,0);

	desktop.Paint();
}

void GM_MainMenu::mouseEvent(SDL_Event& event)
{
	glictPos pos;
	pos.x = ptrx;
	pos.y = ptry;

	desktop.TransformScreenCoords(&pos);

	if (event.button.state == SDL_PRESSED)
		desktop.CastEvent(GLICT_MOUSEDOWN, &pos, 0);
	if (event.button.state != SDL_PRESSED)
		desktop.CastEvent(GLICT_MOUSEUP, &pos, 0);

}

void GM_MainMenu::keyPress (char key)
{
	desktop.CastEvent(GLICT_KEYPRESS, &key, 0);
}


void GM_MainMenu::msgBox (const char* mbox, const char* title) {
	glictSize s;
	glictMessageBox *mb;
	desktop.AddObject(mb = new glictMessageBox);

	mb->SetCaption(title);
	mb->SetMessage(mbox);

	mb->SetHeight(glictFontNumberOfLines(mbox)*16 + 35);
	mb->SetWidth((int)glictFontSize(mbox, "system"));

	mb->GetSize(&s);

	mb->SetPos(glictGlobals.w / 2 - s.w / 2, glictGlobals.h/ 2 - s.h / 2);

	mb->SetOnDismiss(GM_MainMenu::MBOnDismiss);

}

void GM_MainMenu::MBOnDismiss(glictPos* pos, glictContainer* caller) {
	GM_MainMenu* m = (GM_MainMenu*)g_game;
	m->desktop.RemoveObject(caller);
	delete caller;
}


/* ****** EVENT CALLBACKS ****** */
void GM_MainMenu::pnlMainMenu_btnLogIn_OnClick(glictPos* relmousepos, glictContainer* callerclass)
{
	GM_MainMenu* m = (GM_MainMenu*)g_game;
	m->winLogin.window.SetVisible(true);
	m->winLogin.txtUsername.SetCaption("");
	m->winLogin.txtPassword.SetCaption("");
}

void GM_MainMenu::pnlMainMenu_btnOptions_OnClick(glictPos* relmousepos, glictContainer* callerclass)
{
	GM_MainMenu* m = (GM_MainMenu*)g_game;
	m->winOptions.window.SetVisible(true);
}

void GM_MainMenu::pnlMainMenu_btnAbout_OnClick(glictPos* relmousepos, glictContainer* callerclass)
{
	GM_MainMenu* m = (GM_MainMenu*)g_game;
	m->msgBox(
		"YATC - Yet Another Tibia Client\n"
		"v1.0\n"
		"\n"
		"(c) 2007 OpenTibia Team\n"
		"\n"
		"Programmed by (in no particular order):\n"
		"mips\n"
		"Ivan Vucica\n"
		"Smygflik\n"
		"\n"
		"YATC comes with ABSOLUTELY NO WARRANTY; for details see sections 11 and 12 in LICENSE.\n"
		"This is free software, and you are welcome to redistribute it under certain conditions;\n"
		"see LICENSE for details.",

		"About YATC"
	);
}

void GM_MainMenu::pnlMainMenu_btnExit_OnClick(glictPos* relmousepos, glictContainer* callerclass)
{
	g_running = 0;
}
/* ***************** */
void GM_MainMenu::winLogin_btnCancel_OnClick(glictPos* relmousepos, glictContainer* callerclass)
{
	GM_MainMenu* m = (GM_MainMenu*)g_game;
	m->winLogin.window.SetVisible(false);
}
/* ******************* */

void GM_MainMenu::winOptions_btnOk_OnClick(glictPos* relmousepos, glictContainer* callerclass) {
	GM_MainMenu* m = (GM_MainMenu*)g_game;
	m->winOptions.window.SetVisible(false);
}
