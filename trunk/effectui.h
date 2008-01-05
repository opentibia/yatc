//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
// Effectui instance class
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

#ifndef __EFFECTUI_H
#define __EFFECTUI_H

#include "thingui.h"
#include "objects.h"

class EffectUI : virtual public ThingUI
{
public:
	EffectUI(uint16_t id);
	virtual ~EffectUI();

	virtual void Blit(int x, int y, float scale = 1., int map_x = 0, int map_y = 0) const;

	bool canBeDeleted();

protected:

	ObjectType *m_obj;
	uint32_t m_startTime;
};


#endif
