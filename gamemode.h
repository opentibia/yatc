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


#ifndef __GAMEMODE_H
#define __GAMEMODE_H

#include <SDL/SDL_events.h>
#include <string>
#include <list>
#include "gamecontent/enums.h"
class GameMode
{
protected:
	GameMode() {}
public:
	virtual ~GameMode() {}

	virtual void renderScene() = 0;
	virtual void mouseEvent (SDL_Event&) {}
	virtual void keyPress (char key) {}

	virtual void onConnectionError(int message, const char* errortext) {}
	virtual void openMOTD(int motdnum, const std::string& message) {}
	virtual void openMessageWindow(WindowMessage_t type, const std::string& message) {}
	virtual void openCharactersList(const std::list<CharacterList_t>& list, int premDays) {}
};

extern GameMode* g_game;

#endif
