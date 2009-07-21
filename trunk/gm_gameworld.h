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
#include "ui/sbvlpanel.h"
#include "ui/health.h"
#include "ui/uicontainer.h"
#include "ui/itemmove.h"
#include "ui/uishop.h"
#include "ui/uitrade.h"
#include "ui/uioutfit.h"
#include "ui/minimap.h"
#include "ui/exitwarning.h"
#include "ui/deathnotification.h"
#include "stackpanel.h"
#include "automap.h"
#include "statusmsg.h"

#include "options.h"

#include "console.h"

class Tile;
class ProtocolGame;
class Popup;

typedef void(*PopupProducerCallback)(Popup*popup,void*owner,void*arg);

class GM_Gameworld : public GameModeOptions {
public:
	GM_Gameworld();
	~GM_Gameworld();

	void updateScene();
	void keyPress (int key);
	bool specKeyPress (const SDL_keysym& key);
	void doResize(float w, float h);
	void mouseEvent(SDL_Event& event);

	void onExitAttempt();
	void selectHotkeyObj();
	void selectHotkeyObject(Item* item);

    // user interface reactions
    static void onSetOutfit(struct Popup::Item *parent);
    static void onCopyName(struct Popup::Item *parent);

    // net/gamecontent reactions
	virtual void onConnectionClosed();

	void onWalk();
	void onCancelWalk();
	void onTextMessage(MessageType_t type, const std::string& message);
	void onCreatureSpeak(SpeakClasses_t type, int n, const std::string& name, int level, const Position& pos, const std::string& message);
	void onCreatureSpeak(SpeakClasses_t type, int n, const std::string& name, int level, int channel, const std::string& message);
	void onCreatureSpeak(SpeakClasses_t type, int n, const std::string& name, int level, const std::string& message);
	void onAddCreature(uint32_t id);
	void onRemoveCreature(uint32_t id);
	void onCreatureChangeHealth(uint32_t id, uint32_t health);
	void onCreatureMove(uint32_t id, const Position& oldPos, const Position& newPos);
	void onChangeSkills();
	void onChangeStats();
	void onUpdatePlayerCash(uint32_t newcash);
    void openContainer(uint32_t cid);
    void closeContainer(uint32_t cid);
    void openShopWindow(const std::list<ShopItem>& itemlist);
    void closeShopWindow();
    void openTradeWindow(bool ack);
    void closeTradeWindow();
    void openOutfitWindow(const Outfit_t& current, const std::list<AvailOutfit_t>& available);
    void onTileUpdate(const Position& pos);
    void openDeathWindow();

    void onVipLogin(uint32_t id);
    void onVipLogout(uint32_t id);
    void onVipState(uint32_t id, const std::string& name, bool state);

    void showTutorial(uint8_t id);

    bool isExtendedUsing() const { return (m_extendedThingId != 0); }
    bool isTrade() const { return (m_tradeItemId != 0); }
    bool isSelectingHotkeyObject() const { return m_selectingHotkeyObject; }

	bool isDragging() const {return m_dragging;}
	void dismissDrag() {
	    if(m_dragging){
            SDL_SetCursor(g_engine->m_cursorBasic);
	    }
        m_dragging = false;
        m_draggingPrep = false;
    }

	void startDrag(const Position& pos, int stackPos, uint32_t itemid, uint32_t count);
	void dragComplete(const Position& toPos);

    void performPopup(PopupProducerCallback cb,void*owner,void*arg);
    void doNotShowPopup(){m_showPopup = false;}

    Console* getDefaultConsole() { return *getDefaultConsole_it(); }
    Console* findConsole(uint32_t channelid) { return *findConsole_it(channelid); }
    Console* findConsole(const std::string& speaker) { return *findConsole_it(speaker); }
    Console* getActiveConsole() const {return m_activeconsole;}
    void setActiveConsole(Console* i);
    void removeConsole(Console* i);

	void beginExtendedUse(const Thing* thing, int stackpos, const Position& pos);
	void performExtendedUse(const Position& destpos, const Thing* destthing, int deststackpos);
	void performExtendedUseBattleWindow(uint32_t creatureid);

	void beginTrade(const Item* item, int stackpos, const Position& pos);

	void updateRightSide();

protected:

	void actionLook(const glictPos& pos);
	void actionUse(const glictPos& pos);
	void actionAttack(const glictPos& pos);
	void actionTrade(const glictPos& pos);
	void actionUseWith(const glictPos& pos);
	void actionWalk(const glictPos& pos);

private:
    std::vector<Console*>::iterator getDefaultConsole_it() { return m_consoles.begin(); }
    std::vector<Console*>::iterator findConsole_it(uint32_t channelid);
    std::vector<Console*>::iterator findConsole_it(const std::string& speaker);
    std::vector<Console*>::iterator findConsole_it(const Console* c);



    static void pnlConsoleButton_OnClick(glictPos* relmousepos, glictContainer* caller);

    void createConsole(uint32_t channelid=0, const std::string& speaker="");
    void createConsole(const std::string& speaker) { createConsole(0,speaker); }


    /* PRIMARY GUI */
	glictPanel pnlRightSide;
	yatcStackPanel yspRightSide;
	// note(nfries88): The official client doesn't allow dragging right-side windows over the moveable panels.
	// we're replicating that behavior.
	yatcStackPanel yspRightSideWindows;

	pnlInventory_t pnlInventory;
	pnlHealth_t pnlHealth;
	pnlMap_t pnlMap;
	//winSkills_t winSkills;
	sbvlPanel_t sbvlPanel;
	winShop_t winShop;
	winTrade_t winTrade;
	winOutfit_t winOutfit;
	glictPanel pnlTraffic;

	ConsolePanel pnlConsoleContainer;

    std::vector<winContainer_t*> containers;

    winItemMove_t winMove;

    exitWarning_t m_exitWarningWindow;
    deathNotice_t m_deathNotice;

    glictPanel pnlRightSideFiller;
    glictPanel pnlRightSidePanels;

    /* END PRIMARY GUI */


	std::vector<Console*> m_consoles;
	Console* m_activeconsole;
	std::vector<std::string> m_consolehistory;
	std::vector<std::string>::iterator m_consolehistorycurrent;

	uint32_t m_startTime;


	uint16_t m_extendedThingId; // when this is not NULL, we're in extended use mode
    Position m_extendedpos;
    int m_extendedstackpos;

    uint16_t m_tradeItemId;
    Position m_tradepos;
    int m_tradestackpos;

    glictPos m_dragBegin;
    bool m_draggingPrep;
    bool m_dragging;
    uint32_t m_dragThingId;
    uint32_t m_dragThingCount;
    Position m_dragPos;
    int m_dragStackPos;

    bool m_selectingHotkeyObject;

    Automap m_automap;

	ProtocolGame* m_protocol;
	friend class pnlInventory_t;
	friend class winItemMove_t;
	friend class winShop_t;
	friend class winTrade_t;
	friend class winOutfit_t;
	friend class MapUI;
	friend class winContainer_t;
	friend class ItemPanel;
	friend class exitWarning_t;
	friend class deathNotice_t;
	friend class sbvlPanel_t;
	friend class Notifications;
	friend class winBattle_t;

	MapUI m_mapui;

    Popup* m_popup;
    bool m_showPopup;

    StatusMsg m_statusStatMsg, m_lookatStatMsg, m_pmStatMsg, m_broadcastStatMsg;
};

#endif

