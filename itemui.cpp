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

#include "itemui.h"
#include "engine.h"

ItemUI::ItemUI(uint16_t id, uint8_t count) : ThingUI() {
	m_id = id;

	if (id < 100) {
		printf("Error [ItemUI::ItemUI] Invalid item %d\n", id);
		m_obj = NULL;
		return;
	}

	m_obj = Objects::getInstance()->getItemType(id);
	printf("== Item %d ==\n" , id);
	for (int i = 0; i < m_obj->numsprites; i++) {
		printf("Loading sprite %d...\n", m_obj->imageData[i]);
		m_gfx.insert(m_gfx.end(), g_engine->createSprite("Tibia.spr", m_obj->imageData[i]));
	}
	printf("== End of item %d == \n", id);
}
ItemUI::~ItemUI() {
	if (!m_obj)
		return;
	for (std::vector<Sprite*>::iterator it = m_gfx.begin(); it != m_gfx.end();) {
		delete *it;
		m_gfx.erase(it);
	}
}

void ItemUI::Blit(int x, int y) const {
	int g_frame = 0;
	struct { int x, y; } m_pos = {0, 0};

	if (!m_obj)
		return;
	//m_gfx[0]->Blit(x, y);

	if (m_obj->stackable) {
		if (m_count < 1)
			m_gfx[0]->Blit(x,y);
		else if (m_count <= 4)
			m_gfx[m_count-1]->Blit(x,y);
		else if (m_count <= 9)
			m_gfx[4]->Blit(x,y);
		else if (m_count <= 24)
			m_gfx[5]->Blit(x,y);
		else if (m_count <= 49)
			m_gfx[6]->Blit(x,y);
		else if (m_count <= 100)
			m_gfx[7]->Blit(x,y);
		else
			m_gfx[0]->Blit(x,y);
	} else {
		unsigned int activeframe;
		for (int i = 0; i < m_obj->height; i++) {
			for (int j = 0; j < m_obj->width; j++) {
				for (int k = 0; k < m_obj->blendframes; k++) { // note: if it's anything except item, there won't be blendframes...

					activeframe =   (((((( // same amount of ('s as of *'s
									g_frame)
									* m_obj->ydiv + m_pos.y % m_obj->ydiv)
									* m_obj->xdiv + m_pos.x % m_obj->xdiv)
									* m_obj->blendframes + k)  // k == subblendframes  (stacked sprite)
									* m_obj->height + i)           // i == subheight       (y coordinate)
									* m_obj->width + j)        // j == subwidth        (x coordinate)

									;
					m_gfx[activeframe]->Blit(x-j*32,y-i*32);
					printf("Blitting to %d %d\n", x-j*32,y-i*32);
				}
			}
		}
	}
}
