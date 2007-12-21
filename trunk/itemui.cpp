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
		DEBUGPRINT(DEBUGPRINT_ERROR, DEBUGPRINT_LEVEL_OBLIGATORY, "[ItemUI::ItemUI] Invalid item %d\n", id);
		return;
	}
    ObjectType* obj = Objects::getInstance()->getItemType(id);

    obj->loadGfx();
    obj->instancesOnMap++;
    m_id = id;
}

ItemUI::~ItemUI()
{
    // FIXME (ivucica#2#) mips, you've already removed m_id once, but atm i have no idea how to do this cleanly ;)
    // thing is, gfx should be unloaded as soon as it's not needed anymore (when all instances of item are removed from map)

    // and because i'd like to see this optimized for PocketPC, please see what you can do about emptying tiles
    // a bit more often (the way it currently is, it's untolerable for a machine whose process __CAN'T__ take more than 32mb
    // of RAM, and realistically should NEVER take more than 8mb of ram ;)

    ObjectType* obj = Objects::getInstance()->getItemType(m_id);

    obj->instancesOnMap--;
    if (!obj->instancesOnMap) obj->unloadGfx();
}

void ItemUI::BlitItem(int x, int y, uint8_t count, const ObjectType* obj, float scale) const
{
	uint32_t g_frame = 0;
	struct { int x, y; } m_pos = {0, 0};

	if(!obj)
		return;

	if(obj->stackable){

		if(obj->numsprites < 8){
			obj->getGfx()[0]->Blit(x, y, 0, 0, 32, 32, 32*scale, 32*scale);
			DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Item stackable - m_obj->numsprites < 8");
			return;
		}

		if(count < 1){
			obj->getGfx()[0]->Blit(x, y, 0, 0, 32, 32, 32*scale, 32*scale);
		}
		else if(count < 5){
			obj->getGfx()[count - 1]->Blit(x,y, 0, 0, 32, 32, 32*scale, 32*scale);
		}
		else if(count < 10){
			obj->getGfx()[4]->Blit(x, y, 0, 0, 32, 32, 32*scale, 32*scale);
		}
		else if(count < 25){
			obj->getGfx()[5]->Blit(x, y, 0, 0, 32, 32, 32*scale, 32*scale);
		}
		else if(count < 50){
			obj->getGfx()[6]->Blit(x, y, 0, 0, 32, 32, 32*scale, 32*scale);
		}
		else if(count <= 100){
			obj->getGfx()[7]->Blit(x, y, 0, 0, 32, 32, 32*scale, 32*scale);
		}
		else{
			obj->getGfx()[0]->Blit(x, y, 0, 0, 32, 32, 32*scale, 32*scale);
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

					obj->getGfx()[activeframe]->Blit(x - j*32*scale, y - i*32*scale, 0, 0, 32, 32, 32*scale, 32*scale);
					activeframe++;
				}
			}
		}
	}
}
