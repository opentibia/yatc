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

	desktop.AddObject(&btnExit);
	btnExit.SetBGColor(1,0,0,1);

	background = engine->createSprite("yatc.bmp");


}
GM_MainMenu::~GM_MainMenu()
{

}
#include "spritegl.h"
void GM_MainMenu::renderScene()
{
	if (background) background->Blit(0,0);
	engine->drawRectangle(150,150,200,200,oRGBA(255,255,255,255));
	desktop.Paint();

}
