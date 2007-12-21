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

class Position;
class GameMode
{
protected:
    GameMode() {}
public:
	virtual ~GameMode() {}

	virtual void updateScene() {}
	virtual void mouseEvent (SDL_Event&) {}
	virtual void keyPress (char key) {}
	virtual void specKeyPress (int key) {}
	virtual void doResize(float w, float h) {}

	virtual void onConnectionError(int message, const char* errortext) {}
	virtual void openMOTD(int motdnum, const std::string& message) {}
	virtual void openMessageWindow(WindowMessage_t type, const std::string& message) {}
	virtual void openCharactersList(const std::list<CharacterList_t>& list, int premDays) {}
	virtual void onEnterGame() {}
	virtual void onWalk() {}
	virtual void onTextMessage(MessageType_t type, const std::string& message) {printf("Textmessage[%d]: %s\n", type, message.c_str());}

	//SAY,WHISPER, YELL, MONSTER_SAY, MONSTER_YELL
	virtual void onCreatureSpeak(SpeakClasses_t type, int n, const std::string& name, int level,
		const Position& pos, const std::string& message) {}
	//SPEAK_CHANNEL_Y, SPEAK_CHANNEL_R1, SPEAK_CHANNEL_R2, SPEAK_CHANNEL_O
	virtual void onCreatureSpeak(SpeakClasses_t type, int n, const std::string& name, int level,
		int channel, const std::string& message) {}
	//SPEAK_PRIVATE, SPEAK_PRIVATE_RED, SPEAK_BROADCAST
	virtual void onCreatureSpeak(SpeakClasses_t type, int n, const std::string& name, int level,
		const std::string& message) {}

	virtual void onCreatureMove(uint32_t id) {}
};

extern GameMode* g_game;

#endif
