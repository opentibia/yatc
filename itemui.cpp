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

ItemUI::ItemUI(int id) : ThingUI() {
	m_id = id;
	m_obj = Objects::getInstance()->getItemType(id);
	for (int i = 0; i < m_obj->numsprites; i++)
		m_gfx.insert(m_gfx.end(), g_engine->createSprite("Tibia.spr", m_obj->imageData[i]));
}
ItemUI::~ItemUI() {
	for (std::vector<Sprite*>::iterator it = m_gfx.begin(); it != m_gfx.end();) {
		delete *it;
		m_gfx.erase(it);
	}
}

void ItemUI::Blit(int x, int y) {
	m_gfx[0]->Blit(x, y);
}
