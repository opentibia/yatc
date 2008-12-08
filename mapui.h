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
#include "popup.h"

class MapUI {
	public:
		MapUI();
		~MapUI();

		void renderMap();

		void setPos(int x, int y) { m_x = x; m_y = y; }
		void setSize(int w, int h) { m_w = w; m_h = h; }

		void useItem(int x, int y, const Thing* &thing, uint32_t &retx, uint32_t &rety, uint32_t &retz, int &stackpos, bool &extended);
		void attackCreature(int x, int y, const Creature* &creature);
		void lookAtItem(int x, int y, const Thing* &thing, uint32_t &retx, uint32_t &rety, uint32_t &retz, int &stackpos);
		void dragThing(int x, int y, const Thing* &thing, uint32_t &retx, uint32_t &rety, uint32_t &retz, int &stackpos);

		void useItem(Tile* tile, const Thing* &thing, int &stackpos, bool &extended);

		bool handlePopup(int x, int y);

        Tile* translateClickToTile(int x, int y, uint32_t &retx, uint32_t &rety, uint32_t &retz);
        Tile* translateClickToTile(int x, int y) {uint32_t ax,ay,az; return translateClickToTile(x,y,ax,ay,az);}

        static void makePopup(Popup*popup,void*owner,void*arg);


	private:
		int m_x, m_y;
		int m_w, m_h;

		uint32_t m_vpw;
		uint32_t m_vph;

		Position m_lastRightclickTilePos;
		uint32_t m_popupCreatureID;

		static void onLookAt(Popup::Item *parent);
		static void onUse(Popup::Item *parent);
		static void onAttack(Popup::Item *parent);
		static void onFollow(Popup::Item *parent);

		static void onUnimplemented(Popup::Item *parent);

	protected:
		void drawTileEffects(Tile* tile, int screenx, int screeny, float scale, uint32_t tile_height);
        void drawTileGhosts(int x, int y, int z, int screenx, int screeny, float scale, uint32_t tile_height);

		int getMinZ();


};

#endif
