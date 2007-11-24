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
#include "gm_gameworld.h"
#include "engine.h"
#include "options.h"
#include "objects.h"
#include "gamecontent/globalvars.h"
#include "gamecontent/map.h"
#include "gamecontent/item.h"
#include "net/connection.h"
#include "net/protocolgame.h"

extern Connection* g_connection;

GM_Gameworld::GM_Gameworld()
{
	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Starting gameworld...\n");

	ui = g_engine->createSprite("Tibia.pic", 3);
	m_protocol = (ProtocolGame*)g_connection->getProtocol();

	desktop.AddObject(&pnlInventory.panel);
	pnlInventory.panel.SetPos(10, 10);

	doResize(glictGlobals.w, glictGlobals.h);
}

GM_Gameworld::~GM_Gameworld ()
{

}


void GM_Gameworld::doResize(float w, float h)
{
	desktop.SetWidth(glictGlobals.w);
	desktop.SetHeight(glictGlobals.h);
	desktop.ResetTransformations();

	updateScene();
}


void GM_Gameworld::updateScene()
{


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
					printf("Thing invalid %d\n", drawIndex);
					break;
				}
			}

		}
	}

	desktop.Paint();
}


void GM_Gameworld::keyPress (char key)
{
	printf("K%d\n", key);
	desktop.CastEvent(GLICT_KEYPRESS, &key, 0);
}

void GM_Gameworld::specKeyPress (int key)
{
	printf("S%d\n", key);
	switch (key) {
	case SDLK_LEFT:
		m_protocol->sendMove(DIRECTION_WEST);
		break;
	case SDLK_RIGHT:
		m_protocol->sendMove(DIRECTION_EAST);
		break;
	case SDLK_UP:
		m_protocol->sendMove(DIRECTION_NORTH);
		break;
	case SDLK_DOWN:
		m_protocol->sendMove(DIRECTION_SOUTH);
		break;
	}
}
