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

#include "effectui.h"
#include "debugprint.h"
#include "fassert.h"

extern uint32_t g_frameTime;

EffectUI::EffectUI(uint16_t id) : ThingUI()
{
	m_obj = Objects::getInstance()->getEffectType(id);

	m_startTime = g_frameTime;

	if(!m_obj){
		DEBUGPRINT(DEBUGPRINT_ERROR, DEBUGPRINT_LEVEL_OBLIGATORY, "[EffectUI::EffectUI] Invalid effect %d\n", id);
		return;
	}

	m_obj->loadGfx();
	m_obj->instancesOnMap++;
}

EffectUI::~EffectUI()
{
	if(m_obj){
		m_obj->instancesOnMap--;
		if (!m_obj->instancesOnMap) m_obj->unloadGfx();
	}
}

bool EffectUI::canBeDeleted()
{
	if(!m_obj){
		return true;
	}
	if((g_frameTime - m_startTime)/100 >= m_obj->animcount){
		return true;
	}
	else{
		return false;
	}
}

void EffectUI::Blit(int x, int y, float scale, int map_x, int map_y) const
{
	if(!m_obj)
		return;

	uint32_t activeframe;
	uint32_t spriteSize = m_obj->width * m_obj->height * m_obj->blendframes;
	uint32_t animationTime = (g_frameTime - m_startTime)/100;

	activeframe = (map_x % m_obj->xdiv + (map_y % m_obj->ydiv)*m_obj->xdiv +
					(animationTime % m_obj->animcount)*m_obj->xdiv*m_obj->ydiv)*
					spriteSize;

    if (!m_obj->isGfxLoaded())
        m_obj->loadGfx();

	for(uint32_t k = 0; k < m_obj->blendframes; ++k){
		for(uint32_t i = 0; i < m_obj->height; ++i){
			for(uint32_t j = 0; j < m_obj->width; ++j){

				ASSERT(activeframe < m_obj->numsprites);

				m_obj->getGfx()[activeframe]->Blit(x - j*32*scale, y - i*32*scale, 0, 0, 32, 32, 32*scale, 32*scale);
				activeframe++;
			}
		}
	}
}
