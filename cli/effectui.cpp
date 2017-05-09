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

#include "../effectui.h"
#include "../debugprint.h"
#include "../fassert.h"

extern uint32_t g_frameTime;

EffectUI::EffectUI(uint16_t id) : ThingUI()
{
	m_startTime = g_frameTime;

	if(!m_obj){
		DEBUGPRINT(DEBUGPRINT_ERROR, DEBUGPRINT_LEVEL_OBLIGATORY, "[EffectUI::EffectUI] Invalid effect %d\n", id);
		return;
	}
}

EffectUI::~EffectUI()
{
}

bool EffectUI::canBeDeleted()
{
  return true;
}

void EffectUI::Blit(int x, int y, float scale, int map_x, int map_y) const
{
}
