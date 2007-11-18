//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
// Creature instance class
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

#include "creatureui.h"
#include "engine.h"
#include "fassert.h"
CreatureUI::CreatureUI() : ThingUI() {

	//m_gfx.insert(m_gfx.end(), g_engine->createSprite("Tibia.spr", Objects::getInstance()->getOutfitType(1)->imageData[0]));
	m_obj = NULL;
}



void CreatureUI::Blit(int x,int y, float scale) const {
	int m_frame=0;
	struct { int x, y; } m_pos = {0, 0};

	if (!m_obj)
		return;



	uint32_t activeframe = m_frame *
							(m_obj->ydiv + m_pos.y % m_obj->ydiv) *
							(m_obj->xdiv + m_pos.x % m_obj->xdiv);

	//for(uint32_t k = 0; k < m_obj->blendframes; ++k){ // note: if it's anything except item, there won't be blendframes...
	{
		//int k = 0;
		for(uint32_t i = 0; i < m_obj->height; ++i){
			for(uint32_t j = 0; j < m_obj->width; ++j){

				//ASSERT(activeframe < m_obj->numsprites);

				m_gfx[activeframe]->Blit(x - j*32, y - i*32);
				activeframe++;
			}
		}
	}


}

void CreatureUI::loadOutfit() {
	m_obj = Objects::getInstance()->getOutfitType(12);

	for(uint32_t i = 0; i < m_obj->numsprites; i++){
		m_gfx.insert(m_gfx.end(), g_engine->createSprite("Tibia.spr", m_obj->imageData[i]));
	}
}
