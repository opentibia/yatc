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

CreatureUI::CreatureUI() : ThingUI() {

	//m_gfx.insert(m_gfx.end(), g_engine->createSprite("Tibia.spr", Objects::getInstance()->getOutfitType(1)->imageData[0]));

}



void CreatureUI::Blit(int x,int y, float scale) const {
	int frame=0;
	struct { int x, y; } m_pos = {0, 0};

	if (!m_obj)
		return;

	unsigned int activeframe;
	for (int i = 0; i < m_obj->height; i++) {
		for (int j = 0; j < m_obj->width; j++) {
			for (int k = 0; k < m_obj->blendframes; k++) { // note: if it's anything except item, there won't be blendframes...

				activeframe =   (((((( // same amount of ('s as of *'s
								frame)
								* m_obj->ydiv + m_pos.y % m_obj->ydiv)
								* m_obj->xdiv + m_pos.x % m_obj->xdiv)
								* m_obj->blendframes + k)  // k == subblendframes  (stacked sprite)
								* m_obj->height + i)           // i == subheight       (y coordinate)
								* m_obj->width + j)        // j == subwidth        (x coordinate)

								;
				if (scale != 1.)
					m_gfx[activeframe]->Blit(x-j*32*scale,y-i*32*scale,0,0,32*scale,32*scale,32*scale,32*scale);
				else
					m_gfx[activeframe]->Blit(x-j*32,y-i*32);
			}
		}
	}

}

void CreatureUI::loadOutfit() {
	m_obj = Objects::getInstance()->getOutfitType(1);
	m_gfx.insert(m_gfx.end(), g_engine->createSprite("Tibia.spr", m_obj->imageData[0]));
}
