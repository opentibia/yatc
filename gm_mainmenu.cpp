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

	/* ****************** MAIN MENU *********************** */
	desktop.AddObject(&pnlMainMenu.mainmenu);

	pnlMainMenu.mainmenu.SetWidth(118);
	pnlMainMenu.mainmenu.SetHeight(170);
	pnlMainMenu.mainmenu.SetBGColor(.4,.4,.4,1.);

	pnlMainMenu.mainmenu.AddObject(&pnlMainMenu.btnLogIn);
	pnlMainMenu.mainmenu.AddObject(&pnlMainMenu.btnOptions);
	pnlMainMenu.mainmenu.AddObject(&pnlMainMenu.btnAbout);
	pnlMainMenu.mainmenu.AddObject(&pnlMainMenu.btnExit);

	pnlMainMenu.btnLogIn.SetWidth(84);
	pnlMainMenu.btnLogIn.SetHeight(20);
	pnlMainMenu.btnLogIn.SetCaption("Enter Game");
	pnlMainMenu.btnLogIn.SetOnClick(GM_MainMenu::pnlMainMenu_btnLogIn_OnClick);
	pnlMainMenu.btnLogIn.SetPos(16,16);
	pnlMainMenu.btnLogIn.SetBGColor(.2,.2,.2,1.);

	pnlMainMenu.btnOptions.SetWidth(84);
	pnlMainMenu.btnOptions.SetHeight(20);
	pnlMainMenu.btnOptions.SetCaption("Options");
	pnlMainMenu.btnOptions.SetOnClick(GM_MainMenu::pnlMainMenu_btnOptions_OnClick);
	pnlMainMenu.btnOptions.SetPos(16,76);
	pnlMainMenu.btnOptions.SetBGColor(.2,.2,.2,1.);

	pnlMainMenu.btnAbout.SetWidth(84);
	pnlMainMenu.btnAbout.SetHeight(20);
	pnlMainMenu.btnAbout.SetCaption("Info");
	pnlMainMenu.btnAbout.SetOnClick(GM_MainMenu::pnlMainMenu_btnAbout_OnClick);
	pnlMainMenu.btnAbout.SetPos(16,106);
	pnlMainMenu.btnAbout.SetBGColor(.2,.2,.2,1.);

	pnlMainMenu.btnExit.SetWidth(84);
	pnlMainMenu.btnExit.SetHeight(20);
	pnlMainMenu.btnExit.SetCaption("Exit Game");
	pnlMainMenu.btnExit.SetOnClick(GM_MainMenu::pnlMainMenu_btnExit_OnClick);
	pnlMainMenu.btnExit.SetPos(16,136);
	pnlMainMenu.btnExit.SetBGColor(.2,.2,.2,1.);



	/* ****************** LOGIN SCREEN *********************** */
	desktop.AddObject(&winLogin.window);
	winLogin.window.SetWidth(231-4);
	winLogin.window.SetHeight(173-17);
	winLogin.window.SetCaption("Enter Game");
	winLogin.window.SetVisible(false);

	winLogin.window.AddObject(&winLogin.pnlUsername);
	winLogin.window.AddObject(&winLogin.txtUsername);
	winLogin.window.AddObject(&winLogin.pnlPassword);
	winLogin.window.AddObject(&winLogin.txtPassword);
	winLogin.window.AddObject(&winLogin.pnlCreateAc);
	winLogin.window.AddObject(&winLogin.btnCreateAc);
	winLogin.window.AddObject(&winLogin.pnlSeparator); /* **** */
	winLogin.window.AddObject(&winLogin.btnOk);
	winLogin.window.AddObject(&winLogin.btnCancel);

	winLogin.pnlUsername.SetWidth(131-17);
	winLogin.pnlUsername.SetHeight(12);
	winLogin.pnlUsername.SetCaption("Account number:");
	winLogin.pnlUsername.SetPos(13,17);
	winLogin.pnlUsername.SetBGActiveness(false);

	winLogin.txtUsername.SetPos(128,15);
	winLogin.txtUsername.SetWidth(216-134);
	winLogin.txtUsername.SetHeight(46-33);

	winLogin.pnlPassword.SetWidth(131-17);
	winLogin.pnlPassword.SetHeight(12);
	winLogin.pnlPassword.SetCaption("Password:");
	winLogin.pnlPassword.SetPos(13,47);
	winLogin.pnlPassword.SetBGActiveness(false);

	winLogin.txtPassword.SetPos(128,45);
	winLogin.txtPassword.SetWidth(216-134);
	winLogin.txtPassword.SetHeight(46-33);

	winLogin.pnlCreateAc.SetWidth(131-17);
	winLogin.pnlCreateAc.SetHeight(12);
	winLogin.pnlCreateAc.SetCaption("If you don't have\nan account yet::");
	winLogin.pnlCreateAc.SetPos(13,85);
	winLogin.pnlCreateAc.SetBGActiveness(false);

	winLogin.btnCreateAc.SetPos(128,94);
	winLogin.btnCreateAc.SetWidth(216-133);
	winLogin.btnCreateAc.SetHeight(112-95);
	winLogin.btnCreateAc.SetCaption("Create Account");
	winLogin.btnCreateAc.SetBGColor(.2,.2,.2,1.);

	winLogin.pnlSeparator.SetPos(9, 121);
	winLogin.pnlSeparator.SetBGColor(.1,.1,.1,1.);
	winLogin.pnlSeparator.SetWidth(222-13);
	winLogin.pnlSeparator.SetHeight(2);

	winLogin.btnOk.SetPos(127-4,148-17);
	winLogin.btnOk.SetWidth(40);
	winLogin.btnOk.SetHeight(166-149);
	winLogin.btnOk.SetCaption("Ok");
	winLogin.btnOk.SetBGColor(.1,.1,.1,1.);

	winLogin.btnCancel.SetPos(180-4,148-17);
	winLogin.btnCancel.SetWidth(40);
	winLogin.btnCancel.SetHeight(166-149);
	winLogin.btnCancel.SetCaption("Cancel");
	winLogin.btnCancel.SetBGColor(.1,.1,.1,1.);
	winLogin.btnCancel.SetOnClick(GM_MainMenu::winLogin_btnCancel_OnClick);

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
	//GM_MainMenu* m = (GM_MainMenu*)g_game;
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

