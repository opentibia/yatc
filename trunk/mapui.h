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

struct vertex;

class MapUI {
	public:
		MapUI();
		~MapUI();

		void renderMap();

		inline void setPos(int x, int y) { m_x = x; m_y = y; }
		inline void setSize(int w, int h) { m_w = w; m_h = h; }
		inline int getWidth() const { return m_w; }
		inline int getHeight() const { return m_h; }

		void useItem(int x, int y, const Thing* &thing, uint32_t &retx, uint32_t &rety, uint32_t &retz, int &stackpos, bool &extended);
		void useItemExtended(int x, int y, const Thing* &thing, uint32_t &retx, uint32_t &rety, uint32_t &retz, int &stackpos);
		void attackCreature(int x, int y, const Creature* &creature);
		void lookAtItem(int x, int y, const Thing* &thing, uint32_t &retx, uint32_t &rety, uint32_t &retz, int &stackpos);
		void dragThing(int x, int y, const Thing* &thing, uint32_t &retx, uint32_t &rety, uint32_t &retz, int &stackpos);

		void useItem(Tile* tile, const Thing* &thing, int &stackpos, bool &extended);
        void lookAtItem(Tile* tile, const Thing* &thing, int &stackpos);

		bool handlePopup(int x, int y);

        Tile* translateClickToTile(int x, int y, uint32_t &retx, uint32_t &rety, uint32_t &retz);
        Tile* translateClickToTile(int x, int y) {uint32_t ax,ay,az; return translateClickToTile(x,y,ax,ay,az);}

        static void makePopup(Popup*popup,void*owner,void*arg);

        std::list<Direction> getPathTo(int scrx, int scry);


		int getMinZ();
		void MapUI::drawPublicMessages(Position pos, float walkoffx, float walkoffy);


	private:
		int m_x, m_y;
		int m_w, m_h;

		uint32_t m_vpw;
		uint32_t m_vph;

		int m_minz;

		float m_scale;

		Position m_lastRightclickTilePos;
		uint32_t m_popupCreatureID;

		static void onLookAt(Popup::Item *parent);
		static void onUse(Popup::Item *parent);
		static void onAttack(Popup::Item *parent);
		static void onFollow(Popup::Item *parent);
		static void onMessageTo(Popup::Item *parent);
		static void onAddVIP(Popup::Item *parent);
		static void onInviteToParty(Popup::Item *parent);
		static void onRevokeInvite(Popup::Item *parent);
		static void onAcceptInvite(Popup::Item *parent);
		static void onSharedExp(Popup::Item *parent);
		static void onPassLeadership(Popup::Item *parent);
		static void onLeaveParty(Popup::Item *parent);

		static void onUnimplemented(Popup::Item *parent);

		vertex* lightmap;

	protected:
		void drawTileEffects(Tile* tile, int screenx, int screeny, float scale, uint32_t tile_height);
        void drawTileGhosts(int x, int y, int z, int screenx, int screeny, float scale, uint32_t tile_height);
        void fillLightCircle(int x, int y, int radius, uint16_t color);
        bool isVisible(const Position& pos);

};

#endif
