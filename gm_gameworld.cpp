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

extern Connection* g_connection;

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
	pnlTraffic.SetWidth(150);
	pnlTraffic.SetHeight(40);
	pnlTraffic.SetBGActiveness(false);
	pnlTraffic.SetFont("gamefont", 10);
	pnlTraffic.SetCaption("...");
	desktop.AddObject(&txtConsoleEntry);
	txtConsoleEntry.SetHeight(12);
	txtConsoleEntry.SetCaption("");

	m_preWalk = false;
	m_walkState = 0;

	m_consoles.insert(m_consoles.end(), Console());

	doResize(glictGlobals.w, glictGlobals.h);
}

GM_Gameworld::~GM_Gameworld ()
{
    delete ui;
}


void GM_Gameworld::doResize(float w, float h)
{
	desktop.SetWidth(glictGlobals.w);
	desktop.SetHeight(glictGlobals.h);
	desktop.ResetTransformations();

	txtConsoleEntry.SetWidth(w);
	txtConsoleEntry.SetPos(0,h-12);
	updateScene(); // FIXME (ivucica#2#) potentially dangerous during call inside constructor (map possibly not loaded yet) -- gotta check with mips if we may draw map while it still isn't received from server via initial 0x64 packet

}


void GM_Gameworld::updateScene()
{
	#ifdef WINCE
	{
	//DEBUGPRINT(0,0,"Beginning draw\n");
        Position pos = GlobalVariables::getPlayerPosition();

	for(uint32_t i = 0; i < 7; ++i){
		for(uint32_t j = 0; j < 7; ++j){
	//		DEBUGPRINT(0,0,"%d %d\n", i, j);
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

	float scale = 1.f;
	float scaledSize = 32*scale;
	// TODO (ivucica#2#) test on edge of map
	Position pos = GlobalVariables::getPlayerPosition();

	for(uint32_t i = 0; i < 18; ++i){
		for(uint32_t j = 0; j < 14; ++j){

			const Tile* tile = Map::getInstance().getTile(pos.x + i - 9, pos.y + j - 7, pos.z);
			if(!tile){
				//printf("No tile?\n");
				continue;
			}

			int screenx = (int)(i*scaledSize);
			int screeny = (int)(j*scaledSize);

			const Item* ground = tile->getGround();
			if(ground){
				ground->Blit(screenx, screeny, scale);
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
			int32_t drawIndex = 1, lastTopIndex;
			while(drawIndex <= thingsCount && drawIndex >= 1){

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
							drawIndex = 0; //force exit loop
							continue;
						}
						break;
					}

					thing->Blit(screenx, screeny, scale);

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


	// draw always-on-top things
	// (currently only creature names)

	for(uint32_t i = 0; i < 18; ++i){
		for(uint32_t j = 0; j < 14; ++j){
			const Tile* tile = Map::getInstance().getTile(pos.x + i - 9, pos.y + j - 7, pos.z);
			if(!tile){
				//printf("No tile?\n");
				continue;
			}

			int screenx = (int)(i*scaledSize);
			int screeny = (int)(j*scaledSize);

			int32_t thingsCount = tile->getThingCount() - 1;
			int32_t drawIndex = 1;
			while(drawIndex <= thingsCount){

				const Thing* thing = tile->getThingByStackPos(drawIndex);
				if(thing){
					if (thing->getCreature())
						thing->getCreature()->drawName(screenx, screeny, scale);
				}
				drawIndex++;
			}

		}
	}

	m_consoles[0].Paint(0, glictGlobals.h-150, glictGlobals.w, glictGlobals.h-12);

	{
		std::stringstream s;
		s << "TX: " << g_connection->getSent() << " bytes\n" <<
			 "RX: " << g_connection->getRecv()<< " bytes\n" <<
			 "++: " << g_connection->getTraffic() << " bytes\n";
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

	switch (key) {
	case SDLK_LEFT:
		m_preWalk = true;
		m_protocol->sendMove(DIRECTION_WEST);
		break;
	case SDLK_RIGHT:
		m_preWalk = true;
		m_protocol->sendMove(DIRECTION_EAST);
		break;
	case SDLK_UP:
		m_preWalk = true;
		m_protocol->sendMove(DIRECTION_NORTH);
		break;
	case SDLK_DOWN:
		m_preWalk = true;
		m_protocol->sendMove(DIRECTION_SOUTH);
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


void GM_Gameworld::onWalk()
{
	m_preWalk = false;
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
