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
	ObjectType* obj = Objects::getInstance()->getItemType(id);

	if(id < 100 || !obj){
		DEBUGPRINT(DEBUGPRINT_ERROR, DEBUGPRINT_LEVEL_OBLIGATORY, "[ItemUI::ItemUI] Invalid item %d\n", id);
		return;
	}

    obj->loadGfx();
    obj->instancesOnMap++;
}

ItemUI::~ItemUI()
{
    // FIXME (ivucica#2#) mips, you've already removed m_id once, but atm i have no idea how to do this cleanly ;)
    // thing is, gfx should be unloaded as soon as it's not needed anymore (when all instances of item are removed from map)

    // and because i'd like to see this optimized for PocketPC, please see what you can do about emptying tiles
    // a bit more often (the way it currently is, it's untolerable for a machine whose process __CAN'T__ take more than 32mb
    // of RAM, and realistically should NEVER take more than 8mb of ram ;)

    ObjectType* obj = Objects::getInstance()->getItemType(m_id);
    if(obj){
    	obj->instancesOnMap--;
    	if (!obj->instancesOnMap) obj->unloadGfx();
    }
}

void ItemUI::BlitItem(int x, int y, uint8_t count, const ObjectType* obj, float scale, int map_x, int map_y) const
{
	if(!obj)
		return;

	x = x - obj->xOffset;
	y = y - obj->xOffset;

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
		uint32_t activeframe;
		uint32_t spriteSize = obj->width * obj->height * obj->blendframes;
		uint32_t animationTime = g_frameTime/500;

		if(obj->splash || obj->fluidContainer){
			if(count < sizeof(fluidColorMap)/sizeof(int)){
				activeframe = fluidColorMap[count];
			}
			else{
				activeframe = animationTime % obj->numsprites;
			}
		}
		else if(obj->isHangable){
			//TODO
			activeframe = 0;
		}
		else{
			activeframe = (map_x % obj->xdiv + (map_y % obj->ydiv)*obj->xdiv +
							(animationTime % obj->animcount)*obj->xdiv*obj->ydiv)*
							spriteSize;
		}

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
