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

#include "gm_mainmenu.h"
#include "defines.h"
#include "engine.h"
#include "sprite.h"
GM_MainMenu::GM_MainMenu()
{
	desktop.AddObject(&winLogin.login);
	winLogin.login.SetPos(50,50);
	winLogin.login.SetHeight(100);
	winLogin.login.SetWidth(100);
	desktop.AddObject(&btnExit);
	btnExit.SetBGColor(1,0,0,1);
	desktop.SetWidth(640);
	desktop.SetHeight(480);

	background = engine->createSprite("yatc.bmp");
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

	printf("Casting click on %g %g\n", pos.x, pos.y);
}

void GM_MainMenu::keyPress (char key)
{
	desktop.CastEvent(GLICT_KEYPRESS, 0, key);
}

