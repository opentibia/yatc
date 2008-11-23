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

#ifndef __GM_GAMEWORLD_H
#define __GM_GAMEWORLD_H

#include "gamemode.h"
#include "sprite.h"
#include "mapui.h"
#include "ui/uiinventory.h"
#include "ui/skills.h"
#include "ui/health.h"
#include "ui/uicontainer.h"
#include "ui/itemmove.h"
#include "ui/uishop.h"
#include "ui/uitrade.h"
#include "ui/uioutfit.h"
#include "automap.h"

class Console;
class Tile;
class ProtocolGame;
class Popup;

typedef void(*PopupProducerCallback)(Popup*popup,void*owner,void*arg);

class GM_Gameworld : public GameMode {
public:
	GM_Gameworld();
	~GM_Gameworld();

	void updateScene();
	void keyPress (char key);
	bool specKeyPress (const SDL_keysym& key);
	void doResize(float w, float h);
	void mouseEvent(SDL_Event& event);

    void msgBox (const char* mbox, const char* title, glictContainer *focusondismiss = NULL);
    static void MBOnDismiss(glictPos* pos, glictContainer* caller);

    // user interface reactions
    static void onSetOutfit(struct PopupItem *parent);

    // net/gamecontent reactions
	virtual void onConnectionClosed();

	void onWalk();
	void onCancelWalk();
	void onTextMessage(MessageType_t type, const std::string& message);
	void onCreatureSpeak(SpeakClasses_t type, int n, const std::string& name, int level, const Position& pos, const std::string& message);
	void onCreatureSpeak(SpeakClasses_t type, int n, const std::string& name, int level, int channel, const std::string& message);
	void onCreatureSpeak(SpeakClasses_t type, int n, const std::string& name, int level, const std::string& message);
	void onCreatureMove(uint32_t id, const Position& oldPos, const Position& newPos);
	void onChangeSkills();
	void onChangeStats();
	void onUpdatePlayerCash(uint32_t newcash);
    void openContainer(uint32_t cid);
    void closeContainer(uint32_t cid);
    void openShopWindow(const std::list<ShopItem>& itemlist);
    void closeShopWindow();
    void openOutfitWindow(const Outfit_t& current, const std::list<AvailOutfit_t>& available);
    void onTileUpdate(const Position& pos);

    void showTutorial(uint8_t id);

	void openTradeWindow(bool ack);
	void closeTradeWindow();

    void beginExtendedUse(const Thing* thing, int stackpos, const Position& pos);
    void performExtendedUse(const Position& destpos, const Thing* destthing, int deststackpos);
    bool isExtendedUsing() const { return (m_extendedthing != NULL); }

	bool isDragging() const {return m_dragging;}
	void dismissDrag() { if (m_dragging) SDL_SetCursor(g_engine->m_cursorBasic); m_dragging = false; m_draggingPrep = false; }
	void getDragData(const Thing*& dragThing, Position& dragPos, int& dragStackPos) const {
	    dragThing = m_dragThing;
	    dragPos = m_dragPos;
	    dragStackPos = m_dragStackPos;
	}
	void setDragInv(slots_t slot);
	void setDragCtr(uint32_t containerid, uint32_t slotid);
	void dragComplete(Position& toPos);

    void performPopup(PopupProducerCallback cb,void*owner,void*arg);

protected:

private:
    std::vector<Console*>::iterator getDefaultConsole_it() { return m_consoles.begin(); }
    std::vector<Console*>::iterator findConsole_it(uint32_t channelid);
    std::vector<Console*>::iterator findConsole_it(const std::string& speaker);
    std::vector<Console*>::iterator findConsole_it(const Console* c);



    Console* getDefaultConsole() { return *getDefaultConsole_it(); }
    Console* findConsole(uint32_t channelid) { return *findConsole_it(channelid); }
    Console* findConsole(const std::string& speaker) { return *findConsole_it(speaker); }

    static void pnlConsoleButton_OnClick(glictPos* relmousepos, glictContainer* caller);

    void createConsole(uint32_t channelid=0, const std::string& speaker="");
    void createConsole(const std::string& speaker) { createConsole(0,speaker); }

    Console* getActiveConsole() const {return m_activeconsole;}
    void setActiveConsole(Console* i);


    /* PRIMARY GUI */
	glictContainer desktop;
	pnlInventory_t pnlInventory;
	pnlHealth_t pnlHealth;
	winSkills_t winSkills;
	winShop_t winShop;
	winTrade_t winTrade;
	winOutfit_t winOutfit;
	glictPanel pnlTraffic;
	glictTextbox txtConsoleEntry;
	std::vector<glictPanel*> pnlConsoleButtons;
    glictPanel pnlConsoleButtonContainer;

    std::vector<winContainer_t*> containers;

    winItemMove_t winMove;
    /* END PRIMARY GUI */


	std::vector<Console*> m_consoles;
	Console* m_activeconsole;

	uint32_t m_startTime;


	const Thing* m_extendedthing; // when this is not NULL, we're in extended use mode
    Position m_extendedpos;
    int m_extendedstackpos;

    glictPos m_dragBegin;
    bool m_draggingPrep;
    bool m_dragging;


    const Thing* m_dragThing;
    Position m_dragPos;
    int m_dragStackPos;

    slots_t m_draggingInv;
    uint32_t m_draggingCtrId, m_draggingCtrSlot;

    Automap m_automap;

	ProtocolGame* m_protocol;
	friend class pnlInventory_t;
	friend class winContainer_t;
	friend class winItemMove_t;
	friend class winShop_t;
	friend class winTrade_t;
	friend class winOutfit_t;
	friend class MapUI;

	MapUI m_mapui;

    Popup* m_popup;

};

#endif

