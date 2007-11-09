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
GM_Gameworld::GM_Gameworld()
{
	ui = g_engine->createSprite("Tibia.pic", 3);
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

	// TODO (ivucica#2#) test on edge of map
	printf("Painting...\n");
	Position pos = GlobalVariables::getPlayerPosition();

	for ( int i = 0; i < 18; i++) {
		for ( int j = 0; j < 14; j++) {

			Tile *t = Map::getInstance().getTile(pos.x+i - 9, pos.y+j - 7, 7);
			if (!t) {
				printf("No tile?\n");
				continue;
			}
			// FIXME (ivucica#1#) error: passing 'const Item' as 'this' argument of 'virtual const void ItemUI::Blit(int, int)' discards qualifiers
			// I cast it into Item* temporarily

			t->getGround()->Blit(i*32,j*32);

			for (int k=0; k<t->getThingCount(); k++) {
				Thing *th = t->getThingByStackPos(k);
				if (th)
					th->Blit(i*32,j*32);
				else
					printf("Thing invalid\n");
			}
			printf("Painting %d %d\n", i, j);
		}
	}

}
