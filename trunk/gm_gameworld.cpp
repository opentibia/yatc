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
	ui = g_engine->createSprite("Tibia.pic", 3);
	m_protocol = (ProtocolGame*)g_connection->getProtocol();
}

GM_Gameworld::~GM_Gameworld ()
{

}

void GM_Gameworld::updateScene()
{
	for(int i = 0; i < options.w; i += 96){
		for(int j = 0; j < options.h; j += 96){
			ui->Blit(i, j, 0, 0, 96, 96);
		}
	}

	float scale=1.;
	// TODO (ivucica#2#) test on edge of map
	Position pos = GlobalVariables::getPlayerPosition();

	for(uint32_t i = 0; i < 18; ++i){
		for(uint32_t j = 0; j < 14; ++j){

			const Tile* tile = Map::getInstance().getTile(pos.x + i - 9, pos.y + j - 7, 7);
			if(!tile){
				//printf("No tile?\n");
				continue;
			}

			const Item* ground = tile->getGround();
			if(ground){
				ground->Blit(i*32*scale,j*32*scale, scale);
			}


			// FIXME (mips#1#) error: find the right draw order
			// ivucica says: i think that it's slightly better if we go 0 => n than n => 0
			//for(int32_t k = tile->getThingCount() - 1; k >= 1 ; --k){
			for(int32_t k = 0; k < tile->getThingCount(); ++k){
				const Thing* thing = tile->getThingByStackPos(k);
				if(thing){
					thing->Blit(i*32, j*32);
				}
				else{
					printf("Thing invalid %d\n", k);
				}
			}
		}
	}

}


void GM_Gameworld::keyPress (char key)
{
	printf("K%d\n", key);
//	desktop.CastEvent(GLICT_KEYPRESS, &key, 0);
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
