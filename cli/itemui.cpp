//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
// Item instance class
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

#include "../itemui.h"
#include "fassert.h"

#include "util.h"
#include "gamecontent/enums.h"
#include <cmath>


extern uint32_t g_frameTime;


enum FluidColors_t {
	FLUID_EMPTY   = 0x00,
	FLUID_BLUE    = 0x01,
	FLUID_RED     = 0x02,
	FLUID_BROWN   = 0x03,
	FLUID_GREEN   = 0x04,
	FLUID_YELLOW  = 0x05,
	FLUID_WHITE   = 0x06,
	FLUID_PURPLE  = 0x07
};

int fluidColorMap[] = {
	FLUID_EMPTY,
	FLUID_BLUE,
	FLUID_PURPLE,
	FLUID_BROWN,
	FLUID_BROWN,
	FLUID_RED,
	FLUID_GREEN,
	FLUID_BROWN,
	FLUID_YELLOW,
	FLUID_WHITE,
	FLUID_PURPLE,
	FLUID_RED,
	FLUID_YELLOW,
	FLUID_BROWN,
	FLUID_YELLOW,
	FLUID_WHITE,
	FLUID_BLUE,
};

ItemUI::ItemUI(uint16_t id) : ThingUI()
{
  m_id = id;
}

ItemUI::~ItemUI()
{
}

void ItemUI::BlitItem(int x, int y, uint8_t count, ObjectType* obj, float scale, int map_x, int map_y) const
{
}

void ItemUI::BlitCount(int x, int y, uint8_t count, float scale) const
{
}
