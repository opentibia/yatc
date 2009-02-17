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

#ifndef __AUTOMAP_H
#define __AUTOMAP_H

#include <string>
#include <list>
#include <map>
#include "gamecontent/position.h"
class Sprite;

class MiniMapArea{
public:
	MiniMapArea(int x, int y, int z){
		m_basepos.x = x & 0xFF00;
		m_basepos.y = y & 0xFF00;
		m_basepos.z = z & 0xFF;
		load();
	}
	~MiniMapArea(){
		save();
	}

	void getTileColor(uint16_t x, uint16_t y, uint8_t& color, uint8_t& speedindex){
		color = m_color[x][y];
		speedindex = m_speed[x][y];
	}

	void setTileColor(uint16_t x, uint16_t y, uint8_t color, uint8_t speedindex){
		m_color[x][y] = color;
		m_speed[x][y] = speedindex;
	}

	bool save();

protected:
	bool load();

	uint8_t m_color[256][256];
	uint8_t m_speed[256][256];

	struct MapMark{
		MapMark(uint32_t _x, uint32_t _y, uint32_t _type, const char* _text){
			x = _x; y = _y; type = _type; text = _text;
		}
		uint32_t x, y, type;
		std::string text;
	};

	std::list<MapMark*> m_marks;
	Position m_basepos;
};

class Automap
{
public:
    Automap();
    ~Automap();

    void updateSelf();
    void setPos(const Position& pos);
    void renderSelf(int x, int y, int w, int h);

	void flushTiles();

    void setTileColor(int x, int y, int z, uint8_t color,uint8_t speedindex);
    void getTileColor(int x, int y, int z, uint8_t &color,uint8_t &speedindex);

private:
	std::map<uint32_t, MiniMapArea*> m_areas;
	Position m_pos;
	Sprite* m_bitmap;

	int m_mapw, m_maph;
};

#endif
