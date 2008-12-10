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
#include <vector>
#include <map>
class Sprite;

class Automap
{

    typedef struct {
        int x,y,z;
        uint8_t color,speedindex;
    } posAndColor;

public:
    Automap();
    ~Automap();

    void updateSelf();
    void setPos(int x, int y, int z);
    void renderSelf(int x, int y, int w, int h);
    void setTileColor(int x, int y, int z, uint8_t color,uint8_t speedindex);
    void flushTiles();
    void getTileColor(int x, int y, int z, uint8_t &color,uint8_t &speedindex);

    inline int tileCount() const { return m_tileCount; }
private:
	Sprite* map[4];
	int mapw, maph, px, py, pz, mapcount;
	std::string mapfns[4];

    typedef std::map<std::string, std::vector<posAndColor> >  writeFilesMap;
	writeFilesMap writeFiles;
	int m_tileCount;
};

#endif
