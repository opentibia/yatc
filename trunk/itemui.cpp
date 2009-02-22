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

#include "util.h"
#include "gamecontent/enums.h"


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
    if (!Objects::getInstance()->isLoaded()) {
        printf("Destroying item %d after dat was unloaded\n", m_id);
        return;
    }
    ObjectType* obj = Objects::getInstance()->getItemType(m_id);
    //printf("Destroying an item of type %d\n", m_id);
    if (obj) {
		if(obj->instancesOnMap){
			obj->instancesOnMap--;
			//printf("Remaining: %d\n", obj->instancesOnMap);
			if (!obj->instancesOnMap) obj->unloadGfx();
		} else {
		    //printf("Already unloaded\n");
		}
    }
}

void ItemUI::BlitItem(int x, int y, uint8_t count, ObjectType* obj, float scale, int map_x, int map_y) const
{
	if(!obj)
		return;

	x = x - obj->xOffset;
	y = y - obj->xOffset;

    if (!obj->isGfxLoaded()) {
        obj->loadGfx();
    }

	if(obj->stackable){

		if(obj->numsprites < 8){
			obj->getGfx()[0]->Blit(x, y, 0, 0, 32, 32, 32*scale, 32*scale);
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
				if (activeframe >= obj->numsprites) // for items like serverid 2014 which are fluid containers but with no visible variation
                    activeframe = 0;
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
                    if (activeframe >= obj->numsprites) {
                        printf("warning: active: %d out of %d (itemid %d)\n", activeframe, obj->numsprites, m_id);
                        printf("generated from: \n"
                            "(map_x[%d] %% obj->xdiv[%d] + (map_y[%d] %% obj->ydiv[%d])[%d]*obj->xdiv[%d]   [%d] + \n"
							"(animationTime[%d] %% obj->animcount[%d])[%d]*obj->xdiv*obj->ydiv[%d])  [%d]* \n"
							"spriteSize[%d];\n",
							map_x,
							obj->xdiv,
							map_y,
                            obj->ydiv,
							(map_y % obj->ydiv),
							obj->xdiv,
							(map_y % obj->ydiv)*obj->xdiv,

							animationTime,
							obj->animcount,
							animationTime % obj->animcount,
							(animationTime % obj->animcount)*obj->xdiv*obj->ydiv,

							(map_x % obj->xdiv + (map_y % obj->ydiv)*obj->xdiv +
							(animationTime % obj->animcount)*obj->xdiv*obj->ydiv),
							spriteSize
							 );
                        printf("Sizeof fluidcolormap: %ld\n", sizeof(fluidColorMap));
                        printf("Count: %d\n", count);
                        continue;
                    }
					ASSERT(activeframe < obj->numsprites);
					ASSERT(obj->getGfx().size() > activeframe);
					ASSERT(obj->getGfx()[activeframe]);

					obj->getGfx()[activeframe]->Blit(x - j*32*scale, y - i*32*scale, 0, 0, 32, 32, 32*scale, 32*scale);
					activeframe++;
				}
			}
		}
	}

	if(map_x == 0 && map_y == 0 && (obj->rune || obj->stackable) && count > 1){
		BlitCount(x, y, count, scale);
	}
}

void ItemUI::BlitCount(int x, int y, uint8_t count, float scale) const
{
	std::string cntText = yatc_itoa(count);

	// calculate position for count
	x += int(32.f * scale) - int(g_engine->sizeText(cntText.c_str(), "gamefont"));
	y += int(32.f * scale) - 12;

	g_engine->drawText(cntText.c_str(), "gamefont", x, y, TEXTCOLOR_WHITE);
}
