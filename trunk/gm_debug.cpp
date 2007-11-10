//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
// Debugging gamemode
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

#include "gm_debug.h"
#include "defines.h"
#include "engine.h"
#include "sprite.h"
#include "options.h"
#include "itemui.h"
#include "util.h"

#include "net/connection.h"
#include "net/protocollogin.h"
#include "net/protocolgame80.h"

extern Connection* g_connection;

#include "gamecontent/container.h"
#include "gamecontent/creature.h"
#include "gamecontent/globalvars.h"
#include "gamecontent/inventory.h"
#include "gamecontent/map.h"

extern bool g_running;
void GM_Debug::ButtonOnClick(glictPos* relmousepos, glictContainer* callerclass)
{
	ProtocolConfig::getInstance().setServerType(SERVER_OTSERV);
	ProtocolConfig::getInstance().setServer("localhost", 7172);
	ProtocolConfig::createLoginConnection(1000, "test");
}

void GM_Debug::ExitOnClick(glictPos* relmousepos, glictContainer* callerclass)
{
	g_running = false;
}

void GM_Debug::UpdateOnClick(glictPos* relmousepos, glictContainer* callerclass)
{
	GM_Debug *gd = (GM_Debug*)g_game;
	gd->spr = g_engine->createSprite("Tibia.spr", atoi(gd->txtSprite.GetCaption().c_str()));
	//gd->thing = new ItemUI(atoi(gd->txtItem.GetCaption().c_str()), 1);
}

GM_Debug::GM_Debug()
{

	desktop.SetWidth(options.w);
	desktop.SetHeight(options.h);
	desktop.ResetTransformations();

	desktop.AddObject(&btnButton);
	btnButton.SetPos(100,100);
	btnButton.SetWidth(128);
	btnButton.SetHeight(16);
	btnButton.SetCaption("Button");
	btnButton.SetBGColor(.75,.75,.75,1);
	btnButton.SetOnClick(GM_Debug::ButtonOnClick);

	desktop.AddObject(&btnExit);
	btnExit.SetPos(100,116);
	btnExit.SetWidth(128);
	btnExit.SetHeight(16);
	btnExit.SetCaption("Exit");
	btnExit.SetBGColor(1,0,0,1);
	btnExit.SetOnClick(GM_Debug::ExitOnClick);

	desktop.AddObject(&txtSprite);
	txtSprite.SetPos(100,132);
	txtSprite.SetWidth(128);
	txtSprite.SetHeight(16);
	txtSprite.SetCaption("200");

	desktop.AddObject(&txtItem);
	txtItem.SetPos(100,148);
	txtItem.SetWidth(128);
	txtItem.SetHeight(16);
	txtItem.SetCaption("100");

	desktop.AddObject(&btnUpdate);
	btnUpdate.SetPos(100,164);
	btnUpdate.SetWidth(128);
	btnUpdate.SetHeight(16);
	btnUpdate.SetCaption("Update gfx");
	btnUpdate.SetBGColor(1,0,0,1);
	btnUpdate.SetOnClick(GM_Debug::UpdateOnClick);


	if(g_engine){
		background = g_engine->createSprite("Tibia.pic", 0);
		spr = g_engine->createSprite("Tibia.spr", 200);
		//thing = new ItemUI(6401, 1);
	}
	else{  // i think that if g_engine does not exist, we might as well crash. what do you think, guys? ivucica
		NativeGUIError("Somehow, engine managed to not initialize.", "YATC Fatal Error");
		exit(1);
	}
}

GM_Debug::~GM_Debug()
{
	delete background;
	delete spr;
	delete thing;
}
void GM_Debug::updateScene()
{
	renderScene();
}
void GM_Debug::renderScene()
{
	if(background)
		background->Blit(0,0,0,0,background->getWidth(),background->getHeight(),glictGlobals.w, glictGlobals.h);
	if(spr)
		spr->Blit(50,50);
	if(thing)
		thing->Blit(400,50);

	desktop.RememberTransformations();
	desktop.Paint();

}

void GM_Debug::mouseEvent(SDL_Event& event)
{
	glictPos pos;
	pos.x = ptrx;
	pos.y = ptry;

	desktop.TransformScreenCoords(&pos);

	printf("Casting click on %g %g (%d %d)\n", pos.x, pos.y, ptrx, ptry);

	if (event.button.state == SDL_PRESSED)
		desktop.CastEvent(GLICT_MOUSEDOWN, &pos, 0);
	if (event.button.state != SDL_PRESSED)
		desktop.CastEvent(GLICT_MOUSEUP, &pos, 0);


}

void GM_Debug::keyPress (char key)
{
	desktop.CastEvent(GLICT_KEYPRESS, &key, 0);
}
