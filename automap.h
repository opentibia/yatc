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
#include "stdinttypes.h"
class Sprite;

class MiniMapArea{
public:
	MiniMapArea(int x, int y, int z);
	~MiniMapArea();

	void setTileColor(uint16_t x, uint16_t y, uint8_t color, uint8_t speedindex);

	bool save();

	Sprite* getSprite(){ return m_bitmap;}

protected:
	bool load();
	void getRGB(uint8_t color, uint8_t& r, uint8_t& g, uint8_t& b);

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
	Sprite* m_bitmap;
};

class Automap
{
public:
    Automap();
    ~Automap();

    void renderSelf(int x, int y, int w, int h, const Position& pos, double zoom=1);
    void setTileColor(int x, int y, int z, uint8_t color,uint8_t speedindex);

private:
	MiniMapArea* getArea(int x, int y, int z);

	std::map<uint32_t, MiniMapArea*> m_areas;
};

#endif
