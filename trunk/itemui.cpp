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
#include "fassert.h"

ItemUI::ItemUI(uint16_t id) : ThingUI()
{
	if(id < 100){
		printf("Error [ItemUI::ItemUI] Invalid item %d\n", id);
		return;
	}

	const ObjectType* obj = Objects::getInstance()->getItemType(id);
//	printf("== Item %d ==\n" , id);
	for(uint32_t i = 0; i < obj->numsprites; i++){
//		printf("Loading sprite %d...\n", obj->imageData[i]);
		m_gfx.insert(m_gfx.end(), g_engine->createSprite("Tibia.spr", obj->imageData[i]));
	}

//	printf("== End of item %d == \n", id);
}

ItemUI::~ItemUI()
{
	std::vector<Sprite*>::iterator it;
	for(it = m_gfx.begin(); it != m_gfx.end(); ++it){
		delete *it;
	}
	m_gfx.clear();
}

void ItemUI::BlitItem(int x, int y, uint8_t count, const ObjectType* obj, float scale) const
{
	uint32_t g_frame = 0;
	struct { int x, y; } m_pos = {0, 0};

	if(!obj)
		return;

	if(obj->stackable){

		if(obj->numsprites < 8){
			m_gfx[0]->Blit(x, y);
			printf("Item stackable - m_obj->numsprites < 8");
			return;
		}

		if(count < 1){
			m_gfx[0]->Blit(x, y);
		}
		else if(count < 5){
			m_gfx[count - 1]->Blit(x,y);
		}
		else if(count < 10){
			m_gfx[4]->Blit(x, y);
		}
		else if(count < 25){
			m_gfx[5]->Blit(x, y);
		}
		else if(count < 50){
			m_gfx[6]->Blit(x, y);
		}
		else if(count <= 100){
			m_gfx[7]->Blit(x, y);
		}
		else{
			m_gfx[0]->Blit(x, y);
		}
	}
	else{
		uint32_t activeframe = g_frame *
							(obj->ydiv + m_pos.y % obj->ydiv) *
							(obj->xdiv + m_pos.x % obj->xdiv);

		for(uint32_t k = 0; k < obj->blendframes; ++k){ // note: if it's anything except item, there won't be blendframes...
			for(uint32_t i = 0; i < obj->height; ++i){
				for(uint32_t j = 0; j < obj->width; ++j){

					ASSERT(activeframe < obj->numsprites);

					m_gfx[activeframe]->Blit(x - j*32, y - i*32);
					activeframe++;
				}
			}
		}
	}
}
