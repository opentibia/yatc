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
	private:
		int m_x, m_y;
		int m_w, m_h;

		uint32_t m_vpw;
		uint32_t m_vph;
	protected:
		void drawTileEffects(Tile* tile, int screenx, int screeny, float scale, uint32_t tile_height);
		int getMinZ();
};

#endif
