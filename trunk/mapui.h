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

#ifndef __MAPUI_H
#define __MAPUI_H

#include "gamecontent/map.h"

class MapUI {
	public:
		MapUI();
		~MapUI();

		void renderMap();

		void setPos(int x, int y) { m_x = x; m_y = y; }
		void setSize(int w, int h) { m_w = w; m_h = h; }

		void useItem(int x, int y, const Thing* &thing, int &retx, int &rety, int &retz, int &stackpos, bool &extended);
		void attackCreature(int x, int y, const Creature* &creature);
		void lookAtItem(int x, int y, const Thing* &thing, int &retx, int &rety, int &retz, int &stackpos);
		void dragThing(int x, int y, const Thing* &thing, int &retx, int &rety, int &retz, int &stackpos);

        Tile* translateClickToTile(int x, int y, int &absx, int &absy, int &absz);
        Tile* translateClickToTile(int x, int y) {int ax,ay,az; return translateClickToTile(x,y,ax,ay,az);}

	private:
		int m_x, m_y;
		int m_w, m_h;

		uint32_t m_vpw;
		uint32_t m_vph;
	protected:
		void drawTileEffects(Tile* tile, int screenx, int screeny, float scale, uint32_t tile_height);
        void drawTileGhosts(int x, int y, int z, int screenx, int screeny, float scale, uint32_t tile_height);

		int getMinZ();

};

#endif
