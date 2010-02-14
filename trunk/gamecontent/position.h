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

#ifndef __YATC_POSITION_H__
#define __YATC_POSITION_H__

#include "../stdinttypes.h"
class Position
{
public:
	Position()
	{
		x = y = z = 0;
	}
	Position(uint32_t _x, uint32_t _y, uint32_t _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}
	~Position(){};
	bool operator!=(const Position& pos) const {
		return (pos.x != x || pos.y != y || pos.z != z);
	}

	bool operator==(const Position& pos2) const {
	    return (pos2.x == x && pos2.y == y && pos2.z == z);
    }

	uint32_t x, y, z;
};


#endif
