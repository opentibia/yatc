//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
// Distanceui instance class
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

#ifndef __DISTANCEUI_H
#define __DISTANCEUI_H

#include "thingui.h"
#include "objects.h"
#include "gamecontent/position.h"


class DistanceUI : virtual public ThingUI
{
public:
	DistanceUI(const Position& from, const Position& to, uint32_t type);
	virtual ~DistanceUI();

	virtual void Blit(int x, int y, float scale = 1., int map_x = 0, int map_y = 0) const;

	const Position& getFromPos() { return m_from;}
	const Position& getToPos() { return m_to;}
	float getFlightProgress();

protected:

	Position m_from;
	Position m_to;
	uint32_t m_direction;

	ObjectType *m_obj;
	uint32_t m_startTime;
	float m_flightTime;
};


#endif
