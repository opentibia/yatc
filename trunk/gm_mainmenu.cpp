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
#include "gm_mainmenu.h"
#include "defines.h"
#include "engine.h"
#include "sprite.h"

GM_MainMenu::GM_MainMenu()
{
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
	//pnlMainMenu.btnLogIn.SetOnClick(GM_MainMenu_LogIn);
	pnlMainMenu.btnLogIn.SetPos(16,16);

	pnlMainMenu.btnOptions.SetWidth(84);
	pnlMainMenu.btnOptions.SetHeight(20);
	pnlMainMenu.btnOptions.SetCaption("Options");
	//pnlMainMenu.btnOptions.SetOnClick(GM_MainMenu_Options);
	pnlMainMenu.btnOptions.SetPos(16,76);

	pnlMainMenu.btnAbout.SetWidth(84);
	pnlMainMenu.btnAbout.SetHeight(20);
	pnlMainMenu.btnAbout.SetCaption("Info");
	//pnlMainMenu.btnAbout.SetOnClick(GM_MainMenu_About);
	pnlMainMenu.btnAbout.SetPos(16,106);

	pnlMainMenu.btnExit.SetWidth(84);
	pnlMainMenu.btnExit.SetHeight(20);
	pnlMainMenu.btnExit.SetCaption("Exit Game");
	//pnlMainMenu.btnExit.SetOnClick(GM_MainMenu_Exit);
	pnlMainMenu.btnExit.SetPos(16,136);




	// TODO (Khaos#1#) These should be moved to an "onresize" function which should be called here, but we don't catch a window resize event yet...
	pnlMainMenu.mainmenu.SetPos(60, glictGlobals.h - 240);
	desktop.SetWidth(glictGlobals.w);
	desktop.SetHeight(glictGlobals.h);
	desktop.ResetTransformations();



	if(g_engine){
		background = g_engine->createSprite("yatc.bmp");
	}
	else{
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
	desktop.CastEvent(GLICT_KEYPRESS, 0, key);
}

