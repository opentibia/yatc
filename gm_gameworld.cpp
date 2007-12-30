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

extern Connection* g_connection;


void pnlInventory_t::inventoryItemOnPaint(glictRect *real, glictRect *clipped, glictContainer *caller)
{
	uint8_t slot = (glictPanel*)caller - (glictPanel*)caller->GetCustomData() + 1; // customdata stores the address of pnlItem[0], and caller is pnlItem[slot]...


	Item* item=Inventory::getInstance().getItem(slot);
	if (item){
		item->Blit((int)real->left, (int)real->top, 1.f); // TODO (ivucica#5#) if item is not 32x32, scale appropriately
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

	updateScene(); // FIXME (ivucica#2#) potentially dangerous during call inside constructor (map possibly not loaded yet) -- gotta check with mips if we may draw map while it still isn't received from server via initial 0x64 packet

}

int GM_Gameworld::getMinZ() { // find out how far can we render... if anything is directly above player, then don't render above that floor
	// FIXME (ivucica#2#) minz should be m_minz and thus be cached. code below should be called only in onWalk()
	Position pos = GlobalVariables::getPlayerPosition();
	const Tile* tile = Map::getInstance().getTile(pos.x, pos.y, pos.z);

    int minz = 0;
    for (int z = pos.z-1; z>=0; z--) {

        tile = Map::getInstance().getTile(pos.x-(z-pos.z), pos.y-(z-pos.z), z);
        if (tile && tile->getThingCount() ) {
        	printf("Player Z: %d, minz: %d, tc: %d\n", pos.z, minz, tile->getThingCount());
            minz = z+1;

            return minz;
        }
    }
    printf("Player Z: %d, nothing above player\n", pos.z);
	return 0;
}


void GM_Gameworld::updateScene()
{
	// TODO (ivucica#2#) test on edge of map

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

	for(int i = 0; i < options.w; i += 96){
		for(int j = 0; j < options.h; j += 96){
			ui->Blit(i, j, 0, 0, 96, 96);
		}
	}

	// set up scale
	float scale = 1.f;
	float scaledSize = 32*scale;

	// get player position
	Position pos = GlobalVariables::getPlayerPosition();

	// find out how far above the player shall be visible
	int m = getMinZ();
	int sz;
	if(pos.z > 7){ // underground
		sz = pos.z + 3;
	}
	else{
		sz = 7;
	}

	// find out scrolling offset
	float walkoffx = 0.f, walkoffy = 0.f;
	Creature* player = Creatures::getInstance().getPlayer();
	if(player){
		player->getWalkOffset(walkoffx, walkoffy, scale);
	}
	walkoffx *= -1; walkoffy *= -1;

	for(int z = sz; z >= m; z--){

		ASSERT(z >= 0);
		int offset = z - pos.z;

		for(uint32_t i = 0; i < 18; ++i){
			for(uint32_t j = 0; j < 14; ++j){

				uint32_t tile_height = 0;

				uint32_t tile_x = pos.x + i - 9 - offset;
				uint32_t tile_y = pos.y + j - 7 - offset;

				const Tile* tile = Map::getInstance().getTile(tile_x, tile_y, z);
				if(!tile){
					//printf("No tile?\n");
					continue;
				}

				int screenx = (int)(i*scaledSize + walkoffx);
				int screeny = (int)(j*scaledSize + walkoffy);

				const Item* ground = tile->getGround();
				if(ground){
					ground->Blit(screenx, screeny, scale, tile_x, tile_y);
				}

				enum drawingStates_t{
					DRAW_ITEMTOP1 = 1,
					DRAW_ITEMTOP2,
					DRAW_ITEMDOWN,
					DRAW_CREATURE,
					DRAW_ITEMTOP3
				};
				drawingStates_t drawState = DRAW_ITEMTOP1;

				int32_t thingsCount = tile->getThingCount() - 1;
				int32_t drawIndex = ground ? 1 : 0, lastTopIndex = 0;

				while(drawIndex <= thingsCount && drawIndex >= 0){

					const Thing* thing = tile->getThingByStackPos(drawIndex);
					if(thing){
						int32_t thingOrder = thing->getOrder();

						switch(drawState){
						case DRAW_ITEMTOP1: //topItems 1,2
						case DRAW_ITEMTOP2:
							if(thingOrder > 2){
								drawState = DRAW_ITEMDOWN;
								lastTopIndex = drawIndex;
								drawIndex = thingsCount;
								continue;
							}
							break;

						case DRAW_ITEMDOWN: //downItems
							if(thingOrder != 5){
								drawState = DRAW_CREATURE;
								continue;
							}
							break;

						case DRAW_CREATURE: //creatures
							if(thingOrder != 4){
								drawState = DRAW_ITEMTOP3;
								drawIndex = lastTopIndex;
								continue;
							}
							break;

						case DRAW_ITEMTOP3: //topItems 3
							if(thingOrder != 3){
								drawIndex = -1; //force exit loop
								continue;
							}
							break;
						}

						thing->Blit(screenx - (int)(tile_height*8.*scale),
									screeny - (int)(tile_height*8.*scale),
									scale, tile_x, tile_y);

						if(const Item* item = thing->getItem()){
							if(item->hasHeight())
								tile_height++;
						}

						switch(drawState){
						case DRAW_ITEMTOP1: //topItems 1,2
						case DRAW_ITEMTOP2:
							drawIndex++;
							break;
						case DRAW_ITEMDOWN: //downItems
						case DRAW_CREATURE: //creatures
							drawIndex--;
							break;
						case DRAW_ITEMTOP3: //topItems 3
							drawIndex++;
							break;
						}

					}
					else{
						DEBUGPRINT(DEBUGPRINT_WARNING, DEBUGPRINT_LEVEL_OBLIGATORY, "Thing invalid %d\n", drawIndex);
						break;
					}
				}
			}
		}
	}

	// draw always-on-top things
	// (currently only creature names)
	int playerspeed;
	for(uint32_t i = 0; i < 18; ++i){
		for(uint32_t j = 0; j < 14; ++j){
			const Tile* tile = Map::getInstance().getTile(pos.x + i - 9, pos.y + j - 7, pos.z);
			if(!tile){
				//printf("No tile?\n");
				continue;
			}

			int screenx = (int)(i*scaledSize + walkoffx);
			int screeny = (int)(j*scaledSize + walkoffy);
			int groundspeed = tile->getGround() ? Objects::getInstance()->getItemType( tile->getGround()->getID() )->speed : 500;

			int32_t thingsCount = tile->getThingCount() - 1;
			int32_t drawIndex = 1;
			while(drawIndex <= thingsCount){

				Thing* thing = (Thing*)tile->getThingByStackPos(drawIndex); // FIXME (ivucica#3#) getThingByStackPos() should allow changing (should not be returning const)
				if(thing){
					if (thing->getCreature()) {
						thing->getCreature()->drawName(screenx, screeny, scale);
						if (!player || thing->getCreature()->getId() != player->getId())
							thing->getCreature()->advanceWalk(groundspeed);
						else
							playerspeed = groundspeed;
					}
				}
				drawIndex++;
			}

		}
	}
	if (player)
		player->advanceWalk(playerspeed);

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
			 " m: " << m << "\n" <<
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

void GM_Gameworld::specKeyPress (int key)
{
	Direction dir; int action;
	switch (key) {
	case SDLK_LEFT:
		action = 0; dir = DIRECTION_WEST;
		break;
	case SDLK_RIGHT:
		action = 0; dir = DIRECTION_EAST;
		break;
	case SDLK_UP:
		action = 0; dir = DIRECTION_NORTH;
		break;
	case SDLK_DOWN:
		action = 0; dir = DIRECTION_SOUTH;
		break;
	}

	switch (action) {
	case 0: // do move
		if (Creatures::getInstance().getPlayer()->getWalkState() == 1.) {
			Creatures::getInstance().getPlayer()->setLookDir(dir);
			Creatures::getInstance().getPlayer()->startWalk();
			m_protocol->sendMove(dir);
}
		break;
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
