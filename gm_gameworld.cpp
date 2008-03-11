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

#ifdef _MSC_VER
#define time _time64
#endif

extern Connection* g_connection;
extern uint32_t g_frameTime;



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

	{
		std::stringstream s, count;

		if (item)
			if (item->getCount()>1)
				count << (int)item->getCount() << "x";

		s << "Slot " << (int)slot << "\n" << count.str();


		g_engine->drawText(s.str().c_str(), "minifont", (int)real->left + 2, (int)real->top + 2, 0);
	}

}


GM_Gameworld::GM_Gameworld()
{
	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Starting gameworld...\n");

	ui = g_engine->createSprite("Tibia.pic", 3);
	m_protocol = (ProtocolGame*)g_connection->getProtocol();

	#ifndef WINCE
	desktop.AddObject(&pnlInventory.panel);
	pnlInventory.panel.SetPos(10, 10);
	desktop.AddObject(&winSkills.window);
	winSkills.window.SetPos(600,5);
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


	m_consoles.insert(m_consoles.end(), Console());

	m_startTime = time(NULL);

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


	m_consoles[0].Paint(0, glictGlobals.h-150, glictGlobals.w, glictGlobals.h-12);

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
}



void GM_Gameworld::keyPress (char key)
{

	txtConsoleEntry.Focus(NULL);
	if (key==13) {
		if (txtConsoleEntry.GetCaption().size())
			m_protocol->sendSay(SPEAK_SAY, txtConsoleEntry.GetCaption());
		txtConsoleEntry.SetCaption("");
	} else {
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
		if (Creatures::getInstance().getPlayer()->getWalkState() == 1.) {
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
		this->m_consoles[0].Insert(std::string("No debugging action"));
	}

}

void GM_Gameworld::mouseEvent(SDL_Event& event)
{
	glictPos pos;
	// FIXME (ivucica#3#) this is incorrect, we should be refreshing ptrx and ptry here as well, not just read the old versions ...
	// who knows how the platforms with a different pointing device (e.g. touchscreen?) would behave!
	pos.x = ptrx;
	pos.y = ptry;


	if (event.button.state == SDL_PRESSED){
		if (SDL_GetModState() & KMOD_CTRL) {

			printf("It's a click\n");
			const Thing* thing;
			int x,y,z;
			int stackpos;
			bool isextended;

			m_mapui.useItem(pos.x, pos.y, thing, x, y, z, stackpos, isextended);
			if (stackpos != -1) {
				if (!isextended) {
					printf("Click on %d %d %d, on stackpos %d, on id %d\n", x, y, z, stackpos, thing->getID());
					m_protocol->sendUseItem(Position(x,y,z), thing->getID(), stackpos );
				} else {
					m_consoles[0].Insert(ConsoleEntry("No support for extended use yet"));
				}
			}
		}
	}


	desktop.TransformScreenCoords(&pos);

	if (event.button.state == SDL_PRESSED)
		desktop.CastEvent(GLICT_MOUSEDOWN, &pos, 0);
	if (event.button.state != SDL_PRESSED)
		desktop.CastEvent(GLICT_MOUSEUP, &pos, 0);

	// Scene();

}

/////////////// PROTOCOL EVENTS ///////////////////////

void GM_Gameworld::onWalk()
{
	Creatures::getInstance().getPlayer()->confirmWalk();
}

void GM_Gameworld::onTextMessage(MessageType_t type, const std::string& message)
{
	m_consoles[0].Insert(ConsoleEntry(message));
}


//SAY,WHISPER, YELL, MONSTER_SAY, MONSTER_YELL
void GM_Gameworld::onCreatureSpeak(SpeakClasses_t type, int n, const std::string& name, int level, const Position& pos, const std::string& message)
{
	m_consoles[0].Insert(ConsoleEntry(message, name));
}
//SPEAK_CHANNEL_Y, SPEAK_CHANNEL_R1, SPEAK_CHANNEL_R2, SPEAK_CHANNEL_O
void GM_Gameworld::onCreatureSpeak(SpeakClasses_t type, int n, const std::string& name, int level, int channel, const std::string& message)
{
	m_consoles[0].Insert(ConsoleEntry(message, name));
}
//SPEAK_PRIVATE, SPEAK_PRIVATE_RED, SPEAK_BROADCAST
void GM_Gameworld::onCreatureSpeak(SpeakClasses_t type, int n, const std::string& name, int level, const std::string& message)
{
	m_consoles[0].Insert(ConsoleEntry(message, name));
}

void GM_Gameworld::onCreatureMove(uint32_t id)
{
	if (id != GlobalVariables::getPlayerID() || !Creatures::getInstance().getPlayer()->isPreWalking() )
		Creatures::getInstance().getCreature(id)->startWalk();
	Creatures::getInstance().getCreature(id)->confirmWalk();
	printf("Moving %d\n", id);

}

void GM_Gameworld::onChangeSkills()
{
	winSkills.updateSelf();
}
void GM_Gameworld::onChangeStats()
{
	winSkills.updateSelf();
}