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

#include "gm_debug.h"
#include "defines.h"
#include "engine.h"
#include "sprite.h"

#include "net/connection.h"
#include "net/protocollogin.h"
#include "net/protocolgame80.h"

extern Connection* g_connection;

void GM_Debug::ButtonOnClick(glictPos* relmousepos, glictContainer* callerclass)
{
	EncXTEA* crypto = new EncXTEA;
	//Protocol* protocol = new ProtocolLogin(1000, "test");
	Protocol* protocol = new ProtocolGame80(1000, "test", "Player", 0);
	g_connection = new Connection("localhost", 7171, crypto, protocol);
}

void GM_Debug::ExitOnClick(glictPos* relmousepos, glictContainer* callerclass)
{
	exit(0); // this should be done a bit smarter like terminating loop in "main"...
}

GM_Debug::GM_Debug()
{

	desktop.SetWidth(640);
	desktop.SetHeight(480);


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

	if(g_engine){
		background = g_engine->createSprite("yatc.bmp");
	}
	else{
		background = NULL;
	}
}

GM_Debug::~GM_Debug()
{

}

void GM_Debug::renderScene()
{
	if(background)
		background->Blit(0,0);

	desktop.Paint();
}

void GM_Debug::mouseEvent(SDL_Event& event)
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

void GM_Debug::keyPress (char key)
{
	desktop.CastEvent(GLICT_KEYPRESS, 0, key);
}
