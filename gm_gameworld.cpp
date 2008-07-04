//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
// Gameworld gamemode
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
#include <sstream>
#include <time.h>
#include <iomanip>
#include "gm_gameworld.h"
#include "gm_mainmenu.h"
#include "engine.h"
#include "options.h"
#include "objects.h"
#include "gamecontent/globalvars.h"
#include "gamecontent/map.h"
#include "gamecontent/item.h"
#include "gamecontent/creature.h"
#include "net/connection.h"
#include "net/protocolgame.h"
#include "console.h"
#include "gamecontent/inventory.h"
#include "util.h"

#include "ui/container.h"

#ifdef _MSC_VER
#define time _time64
#endif

extern Connection* g_connection;
extern uint32_t g_frameTime;

void resetDefaultCursor();

/* TODO (ivucica#5#) move this to ui/inventory.cpp (don't do it until compiling of stuff in ui/ has been assured with autoconf)*/
void pnlInventory_t::inventoryItemOnPaint(glictRect *real, glictRect *clipped, glictContainer *caller)
{
	static Sprite* ui = NULL; // FIXME (ivucica#4#) move this local ui, GM_Gameworld::ui and GM_MainMenu::ui to Engine::m_ui, since over there it can be global and not reloaded every now and then
	if (ui == NULL) {
		ui = g_engine->createSprite("Tibia.pic", 3);
	}

	slots_t slot = (slots_t)((glictPanel*)caller - (glictPanel*)caller->GetCustomData() + 1); // customdata stores the address of pnlItem[0], and caller is pnlItem[slot]...


	Item* item=Inventory::getInstance().getItem(slot);
	if (item){
		item->Blit((int)real->left, (int)real->top, 1.f); // TODO (ivucica#5#) if item is not 32x32, scale appropriately
	} else {
		int int_slot;
		switch (slot) {
		default:
		case SLOT_NECKLACE:
			int_slot = 0;
			break;
		case SLOT_HEAD:
			int_slot = 1;
			break;
		case SLOT_BACKPACK:
			int_slot = 2;
			break;
		case SLOT_LEFT:
			int_slot = 3;
			break;
		case SLOT_RIGHT:
			int_slot = 4;
			break;
		case SLOT_ARMOR:
			int_slot = 5;
			break;
		case SLOT_LEGS:
			int_slot = 6;
			break;
		case SLOT_RING:
			int_slot = 7;
			break;
		case SLOT_AMMO:
			int_slot = 8;
			break;
		case SLOT_FEET:
			int_slot = 9;
			break;
		}

		ui->Blit((int)real->left, (int)real->top, 96 + 32 * (int_slot % 5), 32 * (int_slot / 5), 32, 32);

	}

	if (0) {
		std::stringstream s, count;

		if (item)
			if (item->getCount()>1)
				count << (int)item->getCount() << "x";

		s << "Slot " << (int)slot << "\n" << count.str();


		g_engine->drawText(s.str().c_str(), "minifont", (int)real->left + 2, (int)real->top + 2, TEXTCOLOR_WHITE);
	}

}
void pnlInventory_t::inventoryItemOnClick(glictPos *relmousepos,
	glictContainer* callerclass)
{
	if(SDL_GetModState() & KMOD_CTRL)
	{
		slots_t slotid = (slots_t)((glictPanel*)callerclass -
			(glictPanel*)callerclass->GetCustomData() + 1);
		if(slotid >= 0 && slotid <= 10) {
			Item* item = Inventory::getInstance().getItem(slotid);
			if(item != NULL) {
				GM_Gameworld* gameclass = (GM_Gameworld*)g_game;
				gameclass->m_protocol->sendUseItem(Position(0xFFFF, slotid, 0),
					item->getID(), 0);
			}
		}
	}
}

void pnlInventory_t::inventoryItemOnMouseUp(glictPos *relmousepos,
	glictContainer* callerclass)
{
    GM_Gameworld *gw = ((GM_Gameworld*)g_game);
	if (gw->isDragging())
	{
        slots_t slotid = (slots_t)((glictPanel*)callerclass -
			(glictPanel*)callerclass->GetCustomData() + 1);
		if(slotid >= 0 && slotid <= 10) {
            Position dest(0xFFFF, slotid, 0);
            if (gw->m_dragThing)
                gw->m_protocol->sendThrow(gw->m_dragPos, gw->m_dragThing->getID(), gw->m_dragStackPos, dest, 1);
		}
        gw->dismissDrag();
	}
}

void pnlInventory_t::inventoryItemOnMouseDown(glictPos *relmousepos,
	glictContainer* callerclass)
{
    GM_Gameworld *gw = ((GM_Gameworld*)g_game);

    slots_t slotid = (slots_t)((glictPanel*)callerclass -
        (glictPanel*)callerclass->GetCustomData() + 1);
    if(slotid >= 0 && slotid <= 10) {
        gw->setDragInv(slotid);
    } else {
        gw->setDragInv(SLOT_NONE);
    }
}

// TODO (nfries88) move these to ui/container.cpp?
// TODO (ivucica#3#) yes, please! don't forget to update automake, codeblocks AND devcpp projects
void winContainer_t::containerItemOnPaint(glictRect *real, glictRect *clipped, glictContainer *caller)
{
	winContainer_t* window = (winContainer_t*)caller->GetCustomData();
	uint32_t slot_id = window->getSlotId(caller);
	Item* item = window->container->getItem(slot_id);

	if(item != NULL)
	{
		item->Blit((int)real->left, (int)real->top, 1.f);
	}
}

void winContainer_t::containerItemOnClick(glictPos *relmousepos, glictContainer* callerclass)
{
	winContainer_t* window = (winContainer_t*)callerclass->GetCustomData();
	uint32_t slot_id = window->getSlotId(callerclass);
	Item* item = window->container->getItem(slot_id);

	if(SDL_GetModState() & KMOD_CTRL && item != NULL)
	{
		GM_Gameworld* gameclass = (GM_Gameworld*)g_game;
		gameclass->m_protocol->sendUseItem(
			Position(0xFFFF, window->containerId | 0x40, slot_id),
			item->getID(), slot_id);
	}
}


void winContainer_t::containerItemOnMouseUp(glictPos *relmousepos,
	glictContainer* callerclass)
{
    GM_Gameworld *gw = ((GM_Gameworld*)g_game);

	winContainer_t* window = (winContainer_t*)callerclass->GetCustomData();
    uint32_t slot_id = window->getSlotId(callerclass);
	Item* item = window->container->getItem(slot_id);
	uint32_t id = window->container->getId();

	if (gw->isDragging())
	{
	    if (gw->m_dragThing)
	    {
            Position dest(0xFFFF, id | 0x40, slot_id);
            printf("Throwing %d %d %d (item %d) onto %d %d %d\n", gw->m_dragPos.x, gw->m_dragPos.y, gw->m_dragPos.z, gw->m_dragThing->getID(), dest.x, dest.y, dest.z);
            gw->m_protocol->sendThrow(gw->m_dragPos, gw->m_dragThing->getID(), gw->m_dragStackPos, dest, 1);
	    }
        gw->dismissDrag();

	}
}

void winContainer_t::containerItemOnMouseDown(glictPos *relmousepos,
	glictContainer* callerclass)
{
    GM_Gameworld *gw = ((GM_Gameworld*)g_game);

    winContainer_t* window = (winContainer_t*)callerclass->GetCustomData();
    uint32_t slot_id = window->getSlotId(callerclass);
	uint32_t id = window->container->getId();

    gw->setDragCtr(id, slot_id);

}


GM_Gameworld::GM_Gameworld()
{
	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Starting gameworld...\n");

	ui = g_engine->createSprite("Tibia.pic", 3);
	m_protocol = (ProtocolGame*)g_connection->getProtocol();

	#ifndef WINCE
	desktop.AddObject(&pnlInventory.panel);
	pnlInventory.panel.SetPos(600, 20);
	desktop.AddObject(&winSkills.window);
	winSkills.window.SetPos(600, 180);
	desktop.AddObject(&pnlHealth.panel);
	pnlHealth.panel.SetPos(600, 350);

	#endif
	desktop.AddObject(&pnlTraffic);
	pnlTraffic.SetPos(10,10);
	pnlTraffic.SetWidth(200);
	pnlTraffic.SetHeight(40);
	pnlTraffic.SetBGActiveness(false);
	pnlTraffic.SetFont("gamefont", 10);
	pnlTraffic.SetCaption("...");
	desktop.AddObject(&txtConsoleEntry);
	txtConsoleEntry.SetHeight(12);
	txtConsoleEntry.SetCaption("");

    desktop.AddObject(&pnlConsoleButtonContainer);
    pnlConsoleButtonContainer.SetBGActiveness(false);

    createConsole(); // add default console
	m_activeconsole = getDefaultConsole();

	m_startTime = time(NULL);

	m_extendedthing = NULL;
    m_dragging = false;
    m_draggingInv = SLOT_NONE;
    m_draggingCtrId = m_draggingCtrSlot = -1;
    m_dragThing = false;

    m_cursorBasic = ui->createCursor(290,12,11,19, 1, 1);
    m_cursorUse = ui->createCursor(310,12,19,19, 9, 9);


    SDL_SetCursor(m_cursorBasic);

	doResize(glictGlobals.w, glictGlobals.h);

}

GM_Gameworld::~GM_Gameworld ()
{
	delete ui;
}


void GM_Gameworld::doResize(float w, float h)
{
	desktop.SetWidth(w);
	desktop.SetHeight(h);
	desktop.ResetTransformations();

	txtConsoleEntry.SetWidth(w);
	txtConsoleEntry.SetPos(0,h-12);

	pnlTraffic.SetPos(w-200, 0);

    pnlConsoleButtonContainer.SetPos(0,glictGlobals.h-150-14);
    pnlConsoleButtonContainer.SetWidth(glictGlobals.w);
    pnlConsoleButtonContainer.SetHeight(14);

	DEBUGPRINT(0,0,"Updating scene\n");
	updateScene(); // FIXME (ivucica#2#) potentially dangerous during call inside constructor (map possibly not loaded yet) -- gotta check with mips if we may draw map while it still isn't received from server via initial 0x64 packet
	DEBUGPRINT(0,0,"Scene updated\n");


}



void GM_Gameworld::updateScene()
{
	// TODO (ivucica#2#) test on edge of map
	#if 0
	#ifdef WINCE
	{
		Position pos = GlobalVariables::getPlayerPosition();

		for(uint32_t i = 0; i < 7; ++i){
			for(uint32_t j = 0; j < 7; ++j){
				const Tile* tile = Map::getInstance().getTile(pos.x + i - 3, pos.y + j - 3, pos.z);
				int screenx = (int)(i*32);
				int screeny = (int)(j*32);
				const Item* ground = tile->getGround();
				if(ground){
					ground->Blit(screenx, screeny, 1.);
				}
			}
		}
		return;
	}

	#endif
	#endif

	#ifndef WINCE
	for(int i = 0; i < options.w; i += 96){
		for(int j = 0; j < options.h; j += 96){
			ui->Blit(i, j, 0, 0, 96, 96);
		}
	}
	#endif


	m_mapui.renderMap();


	getActiveConsole()->paintConsole(0, glictGlobals.h-150, glictGlobals.w, glictGlobals.h-12);

	if (time(NULL)-m_startTime) {
		double txbph, rxbph, trbph;
		std::stringstream s;
		std::string txs, rxs, trs;

		txbph = (g_connection->getSent() * 3600. / (time(NULL)-m_startTime));
		rxbph = (g_connection->getRecv() * 3600. / (time(NULL)-m_startTime));
		trbph = (g_connection->getTraffic() * 3600. / (time(NULL)-m_startTime));

		txs = (txbph < 1024 ? "Bph" : (txbph < 1024*1024 ? "KBph" : "MBph"));
		rxs = (rxbph < 1024 ? "Bph" : (rxbph < 1024*1024 ? "KBph" : "MBph"));
		trs = (trbph < 1024 ? "Bph" : (trbph < 1024*1024 ? "KBph" : "MBph"));

		txbph /= (txbph < 1024 ? 1 : (txbph < 1024*1024 ? 1024 : 1024*1024));
		rxbph /= (rxbph < 1024 ? 1: (rxbph < 1024*1024 ? 1024 : 1024*1024));
		trbph /= (trbph < 1024 ? 1: (trbph < 1024*1024 ? 1024 : 1024*1024));


		s << "TX: " << g_connection->getSent() << " bytes (" << std::setprecision(3) << txbph << txs << ")\n" <<
			 "RX: " << g_connection->getRecv()<< " bytes (" << std::setprecision(3) << rxbph << rxs << ")\n" <<
			 "++: " << g_connection->getTraffic() << " bytes (" << std::setprecision(3) << trbph << trs << ")\n" <<
			 "";
		pnlTraffic.SetCaption(s.str());
	}

	desktop.Paint();
	g_engine->resetClipping();
}



void GM_Gameworld::keyPress (char key)
{

	txtConsoleEntry.Focus(NULL);
	if (key==13) {
		if (txtConsoleEntry.GetCaption().size()) {
		    bool sent = false;
		    std::string msg = txtConsoleEntry.GetCaption();
		    if (getActiveConsole() == getDefaultConsole()) {
		        if (msg[0] == '*') {
                    int i = msg.find('*',1);
                    printf("second * found at %d\n", i);
                    if (i!=-1) {
                        std::string recipient = msg.substr(1,i-1);
                        std::string newmsg = msg.substr(i+1);
                        printf("recipient: %s\n", recipient.c_str());
                        m_protocol->sendSay(SPEAK_PRIVATE, recipient,newmsg);
                        Console*c = findConsole(recipient);
                        if (c) {
                            c->insertEntry(ConsoleEntry(newmsg, "You",TEXTCOLOR_LIGHTBLUE)); // FIXME (ivucica#3#) "you" should be name of player's character
                        }
                        sent = true;
                    }
		        }

		    } else {
		        if (getActiveConsole()->getSpeakerName().size()) {
		            getActiveConsole()->insertEntry(ConsoleEntry(msg, "You", TEXTCOLOR_LIGHTBLUE));
		            m_protocol->sendSay(SPEAK_PRIVATE, getActiveConsole()->getSpeakerName(), msg);
		            sent = true;
		        }
		    }
		    if (!sent) // if we havent sent yet then use the default ...
                m_protocol->sendSay(SPEAK_SAY, msg);
		}

		txtConsoleEntry.SetCaption("");
	} else if(key != 0) {
		// ALT and CTRL are 0.
		// pressing ALT or CTRL will otherwise cause the text console to lose append with (char)0
		// which is bad because it causes the string to terminate the console textbox to lose focus.
		// It should always have focus
		desktop.CastEvent(GLICT_KEYPRESS, &key, 0);
	}

}

void GM_Gameworld::specKeyPress (const SDL_keysym& key)
{
	Direction dir = DIRECTION_NORTH;
	int action = 0;
	switch (key.sym) {
	case SDLK_LEFT:
		action = 1; dir = DIRECTION_WEST;
		break;
	case SDLK_RIGHT:
		action = 1; dir = DIRECTION_EAST;
		break;
	case SDLK_UP:
		action = 1; dir = DIRECTION_NORTH;
		break;
	case SDLK_DOWN:
		action = 1; dir = DIRECTION_SOUTH;
		break;

    case SDLK_ESCAPE:
    	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Terminating protocol connection...\n");
        delete g_connection;
        g_connection = NULL;
        DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Destroying map...\n");
        Map::getInstance().clear();
        DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Destroying inventory...\n");
        Inventory::getInstance().clear();
        DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Switch to main menu\n");
        delete g_game;
        g_game = new GM_MainMenu;
        return;
	case SDLK_F11:
		action = 255;
		break;
	default:
		action = 0;
	}

	switch (action) {
	case 0: // invalid action / ignore
		break;
	case 1: // do move or turn
		if (!Creatures::getInstance().getPlayer()->isPreWalking()) {
			Creatures::getInstance().getPlayer()->setLookDir(dir);

			if (key.mod & KMOD_CTRL)
				m_protocol->sendTurn(dir);
			else {
				Creatures::getInstance().getPlayer()->startWalk();
				m_protocol->sendMove(dir);
			}

		}
		break;

	case 255: // debugging action
		// FIXME (ivucica#1#) debugging action, just for testing (ctrl+a)
		getDefaultConsole()->insertEntry(std::string("No debugging action"));
	}

}

void GM_Gameworld::mouseEvent(SDL_Event& event)
{
	glictPos pos;
	// FIXME (ivucica#3#) this is incorrect, we should be refreshing ptrx and ptry here as well, not just read the old versions ...
	// who knows how the platforms with a different pointing device (e.g. touchscreen?) would behave!
	pos.x = ptrx;
	pos.y = ptry;

	desktop.TransformScreenCoords(&pos);

    if (event.type == SDL_MOUSEMOTION) {
        #if (GLICT_APIREV >= 67)
        desktop.CastEvent(GLICT_MOUSEMOVE, &pos, 0);
        #else
        #warning We need GLICT apirev 67 or greater to support basic movable windows.
        #endif
        if (m_draggingPrep && !m_dragging) {
            printf("Move %g %g compared to %g %g\n", pos.x, pos.y, m_dragBegin.x, m_dragBegin.y);
            if (abs(int(pos.x - m_dragBegin.x)) > 2 || abs(int(pos.y - m_dragBegin.y)) > 2) {
                int x,y,z;
                m_dragging = true; // TODO (ivucica#5#) kick out m_dragging; m_dragThing can be NULL when we're not draggging
                SDL_SetCursor(m_cursorUse);

                if (m_draggingInv == SLOT_NONE && m_draggingCtrId == -1){ // not throwing from inventory nor from a container?
                    m_mapui.dragThing(m_dragBegin.x, m_dragBegin.y, m_dragThing, x,y,z, m_dragStackPos);
                    m_dragPos = Position(x,y,z);
                }
                else if (m_draggingInv != SLOT_NONE){ // throwing from inventory
                    m_dragPos = Position(0xFFFF, m_draggingInv, 0);
                } else {
                    m_dragPos = Position(0xFFFF, m_draggingCtrId | 0x40, m_draggingCtrSlot);
                }

            }
        }
    } else {

        if (event.button.state == SDL_PRESSED){
            if (desktop.CastEvent(GLICT_MOUSEDOWN, &pos, 0)){ // if event got handled by glict
                // just ignore

            } else if (m_extendedthing){ // otherwise handle as appropriate
                printf("Performing extended use\n");
                const Thing* thing;
                int x,y,z;
                int stackpos;
                bool isextended;

                m_mapui.useItem(pos.x, pos.y, thing, x, y, z, stackpos, isextended);
                if (stackpos != -1)
                    m_protocol->sendUseItemWith(m_extendedpos, m_extendedthing->getID(), m_extendedstackpos,
                                                Position(x,y,z), thing->getID(), stackpos);
                SDL_SetCursor(m_cursorBasic);
                m_extendedthing = NULL;
            } else if (SDL_GetModState() & KMOD_ALT) {
                printf("Attacking!\n");
                const Creature* creature = NULL;
                m_mapui.attackCreature(pos.x, pos.y, creature);
                if(creature != NULL) {
                    if(creature->getID() != GlobalVariables::getPlayerID()) {
                        m_protocol->sendAttackCreature(creature->getID());
                        GlobalVariables::setAttackID(creature->getID());
                    }
                }
            } else if (SDL_GetModState() & KMOD_CTRL) {
                printf("It's a use\n");
                const Thing* thing;
                int x,y,z;
                int stackpos;
                bool isextended;

                m_mapui.useItem(pos.x, pos.y, thing, x, y, z, stackpos, isextended);

                if(stackpos != -1){
                    if(!isextended){
                        printf("Click on %d %d %d, on stackpos %d, on id %d\n", x, y, z, stackpos, thing->getID());
                        m_protocol->sendUseItem(Position(x,y,z), thing->getID(), stackpos );
                    } else {
                        m_extendedthing = thing;
                        m_extendedstackpos = stackpos;
                        m_extendedpos = Position(x,y,z);
                        SDL_SetCursor(m_cursorUse);
                    }
                }
            } else if (SDL_GetModState() & KMOD_SHIFT) {
                printf("It's a look\n");
                const Thing* thing;
                int x,y,z;
                int stackpos;

                m_mapui.lookAtItem(pos.x, pos.y, thing, x, y, z, stackpos);
                if (stackpos != -1) {
                    printf("Click on %d %d %d, on stackpos %d, on id %d\n", x, y, z, stackpos, thing->getID());
                    m_protocol->sendLookItem(Position(x,y,z), thing->getID(), stackpos );
                }
            } else {
                // just remember where we began the drag, for later comparison
                m_dragBegin = pos;
                m_draggingPrep = true;
                m_draggingInv = SLOT_NONE; // we're NOT dragging from inventory
                m_draggingCtrId = m_draggingCtrSlot = -1;
            }
        } else if (event.button.state == SDL_RELEASED) {
            if (desktop.CastEvent(GLICT_MOUSEUP, &pos, 0)){
                // ignore if it was handled by glict
                dismissDrag();
            } else {
                // TODO (nfries88): drag items
                // TODO (nfries88): walk by clicking
                if (m_dragging){

                    int dx,dy,dz;



                    m_mapui.translateClickToTile(pos.x,pos.y,dx,dy,dz);
                    //TODO (ivucica#3#): drag count window
                    m_protocol->sendThrow(m_dragPos, m_dragThing->getID(), m_dragStackPos, Position(dx,dy,dz), 1);

                    dismissDrag();
                    printf("Released from drag\n");

                }
                m_draggingPrep = false;
            }
        } else {
            printf("unknown event\n");
        }

    }

	// Scene();

}

////////////// CONNECTION EVENTS //////////////////////
void GM_Gameworld::onConnectionClosed()
{
	delete g_game;
	g_game = new GM_MainMenu();
	//todo: return to character list
}

/////////////// PROTOCOL EVENTS ///////////////////////

void GM_Gameworld::onWalk()
{
	Creatures::getInstance().getPlayer()->confirmWalk();
}

void GM_Gameworld::onCancelWalk()
{
	Creatures::getInstance().getPlayer()->cancelWalk();
}

void GM_Gameworld::onTextMessage(MessageType_t type, const std::string& message)
{
	switch (type){
	case MSG_INFO_DESCR:
		getDefaultConsole()->insertEntry(ConsoleEntry(message, TEXTCOLOR_LIGHTGREEN));
		break;
	default:
		getDefaultConsole()->insertEntry(ConsoleEntry(message));
	}
}


//SAY,WHISPER, YELL, MONSTER_SAY, MONSTER_YELL; from 8.2 on, NPC PMs
void GM_Gameworld::onCreatureSpeak(SpeakClasses_t type, int n, const std::string& name, int level, const Position& pos, const std::string& message)
{
    switch (type){
    case SPEAK_PRIVATE_NP:
    case SPEAK_PRIVATE_PN:
        findConsole("NPCs")->insertEntry(ConsoleEntry(message, name, TEXTCOLOR_LIGHTBLUE)); // this is bad; this way we disallow potential player called "NPCs"
        break; 
    case SPEAK_MONSTER_SAY:
    case SPEAK_MONSTER_YELL:
         getDefaultConsole()->insertEntry(ConsoleEntry(message, name, TEXTCOLOR_ORANGE));
         break;
    default:
        getDefaultConsole()->insertEntry(ConsoleEntry(message, name, TEXTCOLOR_YELLOW));
    }

}
//SPEAK_CHANNEL_Y, SPEAK_CHANNEL_R1, SPEAK_CHANNEL_R2, SPEAK_CHANNEL_O
void GM_Gameworld::onCreatureSpeak(SpeakClasses_t type, int n, const std::string& name, int level, int channel, const std::string& message)
{
    TextColor_t col;
    switch(type){
        case SPEAK_CHANNEL_O:
            col = TEXTCOLOR_ORANGE;
            break;
        case SPEAK_CHANNEL_R1:
        case SPEAK_CHANNEL_R2:
            col = TEXTCOLOR_RED;
            break;
        default:
            col = TEXTCOLOR_YELLOW;
    }
	getDefaultConsole()->insertEntry(ConsoleEntry(message, name, col));
}
//SPEAK_PRIVATE, SPEAK_PRIVATE_RED, SPEAK_BROADCAST
void GM_Gameworld::onCreatureSpeak(SpeakClasses_t type, int n, const std::string& name, int level, const std::string& message)
{
    TextColor_t col;
    switch(type){
        case SPEAK_CHANNEL_O:
            col = TEXTCOLOR_ORANGE;
            break;
        case SPEAK_CHANNEL_R1:
        case SPEAK_CHANNEL_R2:
            col = TEXTCOLOR_RED;
            break;
        default:
            col = TEXTCOLOR_YELLOW;
    }
    switch (type) {
        case SPEAK_PRIVATE:
            findConsole(name)->insertEntry(ConsoleEntry(message, name, TEXTCOLOR_LIGHTBLUE));
            break;
        default: {
            std::stringstream s;
            s << "(private msg of unknown speakclass " << type << ")";
            getDefaultConsole()->insertEntry(ConsoleEntry(message + s.str(), name, TEXTCOLOR_YELLOW));
        }
    }

}

void GM_Gameworld::onCreatureMove(uint32_t id)
{
	if (id != GlobalVariables::getPlayerID() || !Creatures::getInstance().getPlayer()->isPreWalking() )
		Creatures::getInstance().getCreature(id)->startWalk();
	Creatures::getInstance().getCreature(id)->confirmWalk();
//	printf("Moving %d\n", id);

}

void GM_Gameworld::onChangeSkills()
{
	winSkills.updateSelf();
}
void GM_Gameworld::onChangeStats()
{
    pnlHealth.updateSelf();
	winSkills.updateSelf();
}


std::vector<Console*>::iterator GM_Gameworld::findConsole_it(uint32_t channelid)
{
    for(std::vector<Console*>::iterator it=m_consoles.begin(); it != m_consoles.end(); it++) {
        if ((*it)->getChannelId() == channelid) {
            return it;
        }
    }
    createConsole(channelid);
    return m_consoles.end()-1;
}
std::vector<Console*>::iterator GM_Gameworld::findConsole_it(const std::string& speaker)
{
    for(std::vector<Console*>::iterator it=m_consoles.begin(); it != m_consoles.end(); it++) {
        if ((*it)->getSpeakerName() == speaker) {
            return it;
        }
    }
    printf("Creating console for speaker\n");
    createConsole(speaker);
    printf("Success\n");
    return m_consoles.end()-1;
}

std::vector<Console*>::iterator GM_Gameworld::findConsole_it(const Console* c)
{
    for(std::vector<Console*>::iterator it=m_consoles.begin(); it != m_consoles.end(); it++) {
        if ((*it) == c) {
            return it;
        }
    }
    ASSERTFRIENDLY(c, "Code attempted to access a non-existing console")
    return m_consoles.end();
}
void GM_Gameworld::pnlConsoleButton_OnClick(glictPos* relmousepos, glictContainer* caller)
{
    Console* c = (Console*)caller->GetCustomData();
    GM_Gameworld* gw = (GM_Gameworld*)g_game;
    //std::vector<Console*>::iterator cit = gw->findConsole_it(c);
    gw->setActiveConsole(c);

}
void GM_Gameworld::createConsole(uint32_t channelid,const std::string& speaker)
{
    Console* nc;
    std::stringstream s;
    if (channelid) {
        nc = new Console(channelid);
        s << channelid;
    } else if (speaker.size()) {
        nc = new Console(speaker);
        s << speaker;
    } else {
        nc = new Console();
        s << "Console";
    }


    glictPanel* p = new glictPanel;
    p->SetCustomData(nc);
    p->SetHeight(14);
    p->SetCaption(s.str().c_str());
    p->SetWidth(g_engine->sizeText(s.str().c_str(),"system"));
    p->SetBGColor(.2,.2,.2,1.);
    int sum=0;
    for (std::vector<glictPanel*>::iterator it = pnlConsoleButtons.begin(); it != pnlConsoleButtons.end(); it++) {
        (*it)->SetPos(sum,0);
        sum += (*it)->GetWidth();
    }
    p->SetPos(sum,0);
    p->SetOnClick(pnlConsoleButton_OnClick);
    pnlConsoleButtonContainer.AddObject(p);
    pnlConsoleButtons.push_back(p);

    m_consoles.push_back(nc);
}

void GM_Gameworld::openContainer(uint32_t cid)
{
	Container* container = Containers::getInstance().getContainer(cid);
	winContainer_t* window = new winContainer_t(container, cid);

	desktop.AddObject(&window->window);
	containers.push_back(window);
}

void GM_Gameworld::closeContainer(uint32_t cid)
{
	winContainer_t* window = NULL;

	std::vector<winContainer_t*>::iterator it = containers.begin();
	for(;it != containers.end(); ++it)
	{
		if((*it)->containerId == cid)
		{
			window = (*it);
			break;
		}
	}

	if(window)
	{
		desktop.RemoveObject(&window->window);
		containers.erase(it);
		delete window;
	}
}

void GM_Gameworld::setDragInv(slots_t slotid) {
    m_draggingInv = slotid;
    Item* item = Inventory::getInstance().getItem(slotid);
    if(item != NULL) {
        printf("Dragging inv begins here %d %d\n", ptrx, ptry);
        m_dragging = false;
        m_draggingPrep = true;
        m_draggingCtrId = m_draggingCtrSlot = -1;
        m_dragBegin.x = ptrx;
        m_dragBegin.y = ptry;
        m_dragThing = item;
        m_dragPos = Position(0xFFFF, slotid, 0);
        m_dragStackPos = 0;
    }
}

void GM_Gameworld::setDragCtr(uint32_t containerid, uint32_t slotid) {

    m_draggingCtrId = containerid;
    m_draggingCtrSlot = slotid;
    Item* item = Containers::getInstance().getContainer(containerid)->getItem(slotid);
    if(item != NULL) {
        printf("Dragging ctr begins here %d %d\n", ptrx, ptry);
        m_dragging = false;
        m_draggingPrep = true;
        m_draggingInv = SLOT_NONE;
        m_dragBegin.x = ptrx;
        m_dragBegin.y = ptry;
        m_dragThing = item;
        m_dragPos = Position(0xFFFF, containerid | 0x40, slotid);
        m_dragStackPos = slotid;
    }
}
