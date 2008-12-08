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


#ifndef __GM_DEBUG_H
#define __GM_DEBUG_H

#include <GLICT/container.h>
#include <GLICT/button.h>
#include <GLICT/window.h>
#include <GLICT/textbox.h>
#include "gamemode.h"
#include "sprite.h"
#include "thingui.h"
#include "choicegrid.h"

#include "popup.h"
#include "automap.h"

class GM_Debug : public GameMode
{
public:
	GM_Debug();
	~GM_Debug();

	void updateScene();
	void renderScene();
	void mouseEvent (SDL_Event&);
	void keyPress (char key);


    void msgBox (const char* mbox, const char* title, glictContainer *focusondismiss = NULL);
    static void MBOnDismiss(glictPos* pos, glictContainer* caller);

private:

	static void ButtonOnClick(glictPos* relmousepos, glictContainer* callerclass);
	static void ExitOnClick(glictPos* relmousepos, glictContainer* callerclass);
	static void UpdateOnClick(glictPos* relmousepos, glictContainer* callerclass);
	static void UpdateMapOnClick(glictPos* relmousepos, glictContainer* callerclass);

	glictContainer desktop;
	glictButton btnButton;
	glictButton btnExit;
	glictTextbox txtSprite;
	glictTextbox txtItem;
	glictButton btnUpdate;

	glictTextbox txtLocX, txtLocY, txtLocZ;
	glictButton btnUpdateMap;

	Sprite* background;
	Sprite* spr;
	ThingUI* thing;
    Popup* popup;
    bool killpopup;

    Sprite* map[4];
	int mapw, maph, px, py, pz, mapcount;
	std::string mapfns[4];
	Automap a;

	ChoiceGrid grid;

};

#endif
