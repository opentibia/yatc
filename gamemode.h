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
#include "ui/optionsui.h"
#include "ui/optionsgeneral.h"
#include "ui/optionsconsole.h"
#include "ui/optionsgraphics.h"
#include "ui/optionsnetwork.h"


#if defined(_MSC_VER) && !defined(__PRETTY_FUNCTION__)
#define __PRETTY_FUNCTION__ "(msvc)"
#endif

class winOptions_t;
class glictContainer;

class Position;
class ShopItem;
class GameMode
{
protected:
    GameMode() {}
    glictContainer desktop;
public:
	virtual ~GameMode() {}

    /* real functions utilized by all children */

    void centerWindow (glictWindow *win);
    virtual void msgBox (const char* mbox, const char* title, glictContainer *focusondismiss = NULL);
    static void MBOnDismiss(glictPos* pos, glictContainer* caller);

    /* various functions to be implemented by children, if needed*/
	virtual void updateScene() {} // can be used for both updating and painting
	virtual void renderScene() {} // should only paint the scene. optional
	virtual void mouseEvent (SDL_Event&) {}
	virtual void keyPress (int key) {}
	virtual bool specKeyPress (const SDL_keysym&) { return false;}
	virtual void doResize(float w, float h) {}
	virtual void onExitAttempt() {}
	virtual void selectHotkeyObj() {}


	virtual winOptions_t* getOptionsWindow() { return NULL; }

	virtual void onConnectionError(int message, const char* errortext) {}
	virtual void onConnectionClosed() {}
	virtual void openMOTD(int motdnum, const std::string& message) {}
	virtual void openMessageWindow(WindowMessage_t type, const std::string& message) {}
	virtual void openCharactersList(const std::list<CharacterList_t>& list, int premDays) {}
	virtual void onEnterGame() {}
	virtual void onWalk() {}
	virtual void onCancelWalk() {}
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
    virtual void onUpdatePlayerCash(uint32_t newcash) {}

	virtual void onAddCreature(uint32_t id){}
	virtual void onRemoveCreature(uint32_t id){}
	virtual void onCreatureChangeHealth(uint32_t id, uint32_t health){}
	virtual void onCreatureMove(uint32_t id, const Position& oldPos, const Position& newPos) {}

	virtual void onChangeSkills() {}
	virtual void onChangeStats() {}

	virtual void openContainer(uint32_t cid) {};
    virtual void closeContainer(uint32_t cid) {};

    virtual void openShopWindow(const std::list<ShopItem>& itemlist) {}
    virtual void closeShopWindow() {}

    virtual void openTradeWindow(bool ack){};
    virtual void closeTradeWindow(){};

    virtual void onOpenItemText(int windowId, int itemid, int maxTextLenght,
		const std::string& text, const std::string& lastChange,
		const std::string& lastChangeDate){}

    virtual void openOutfitWindow(const Outfit_t& current, const std::list<AvailOutfit_t>& available) {}


    virtual void showTutorial(uint8_t id) {}

    virtual void openDeathWindow() {}

    virtual void onTileUpdate(const Position& pos) {}

};

class GameModeOptions : public GameMode
{
    // class used to extend a gamemode with proper Options functionality
public:
    GameModeOptions();
	virtual winOptions_t* getOptionsWindow() { return &winOptions; }
	virtual void doResize(float w, float h);

    virtual ~GameModeOptions() {}
protected:
    static void winOptions_btnNetwork_OnClick(glictPos* relmousepos, glictContainer* callerclass);
	static void winOptionsNetwork_btnOk_OnClick(glictPos* relmousepos, glictContainer* callerclass);
	static void winOptionsNetwork_btnCancel_OnClick(glictPos* relmousepos, glictContainer* callerclass);



    winOptions_t winOptions;
};


extern GameMode* g_game;

#endif
