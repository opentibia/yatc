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

// TODO (ivucica#2#) review code added in 2739 which was written by mrauter, it's probably not 100% the way we want to do things, plus we need colorization

#include "creatureui.h"
#include "engine.h"
#include "fassert.h"
#include "gamecontent/creature.h"
CreatureUI::CreatureUI() : ThingUI() {
	//m_gfx.insert(m_gfx.end(), g_engine->createSprite("Tibia.spr", Objects::getInstance()->getOutfitType(1)->imageData[0]));
	m_obj = NULL;
}



void CreatureUI::Blit(int x,int y, float scale) const {
	int m_frame=0;
	struct { int x, y; } m_pos = {0, 0};

	if (!m_obj)
		return;



	/*uint32_t activeframe = m_frame *
							(m_obj->ydiv + m_pos.y % m_obj->ydiv) *
							(m_obj->xdiv + m_pos.x % m_obj->xdiv);*/


    Creature* n = (Creature*)this;

    uint32_t activeframe = 0;

    if (n->getOutfit().m_looktype != 0) {
        activeframe = n->getLookDir() * m_obj->height * m_obj->width * m_obj->blendframes;
    }



	//for(uint32_t k = 0; k < m_obj->blendframes; ++k){ // note: if it's anything except item, there won't be blendframes...
	{
	    uint32_t tframes = m_obj->height * m_obj->width * m_obj->blendframes * m_obj->xdiv;
		//int k = 0;
		for(uint32_t i = 0; i < m_obj->height; ++i){
			for(uint32_t j = 0; j < m_obj->width; ++j){

				//ASSERT(activeframe < m_obj->numsprites);

				//m_gfx[activeframe]->Blit(x - j*32, y - i*32);
                m_gfx[activeframe]->Blit(x -8 - j*32, y -8 - i*32);
                switch(n->getOutfit().m_addons){ // ivucica says: i'm not sure this is correct?
                case 1:
                    m_gfx[activeframe+(tframes)]->Blit(x -8 - j*32, y -8 -i*32);
                case 2:
                    m_gfx[activeframe+(tframes*2)]->Blit(x -8 - j*32, y -8 -i*32);
                case 3:
                    m_gfx[activeframe+(tframes)]->Blit(x -8 - j*32, y -8 -i*32);
                    m_gfx[activeframe+(tframes*2)]->Blit(x -8 - j*32, y -8 -i*32);
                }

				activeframe++;
			}
		}
	}


}

void CreatureUI::loadOutfit() {
    Creature* n = (Creature*)this;
    DEBUGPRINT(0,0,"Loading creature %d (itemlook %d)\n", n->getOutfit().m_looktype, n->getOutfit().m_lookitem);

	Outfit_t outfit = n->getOutfit();

	if (!outfit.m_looktype && !outfit.m_lookitem) {
		m_obj = Objects::getInstance()->getItemType(100); // FIXME (ivucica#1#) Completely wrong, but it will have to do for now since otherwise it crashes. it should be invisible
	} else if (outfit.m_lookitem != 0) {
        m_obj = Objects::getInstance()->getItemType(outfit.m_lookitem);
    } else {
       m_obj = Objects::getInstance()->getOutfitType(outfit.m_looktype);
    }

	for(uint32_t i = 0; i < m_obj->numsprites ; i++){
		Sprite *spr;

		if (m_obj->blendframes > 1) {
            if ((i / (m_obj->height * m_obj->width)) % 2 ) { // if it's a template, then let's just put a NULL in there
                m_gfx.insert(m_gfx.end(), NULL);
                continue;
            }
        }

		spr = g_engine->createSprite("Tibia.spr", m_obj->imageData[i]);
		spr->templatedColorize(&spr[i + m_obj->height * m_obj->width], outfit.m_lookhead, outfit.m_lookbody, outfit.m_looklegs, outfit.m_lookfeet);
		m_gfx.insert(m_gfx.end(), spr);
	}
}
