//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
//
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

#include "creature.h"
#include "globalvars.h"

extern uint32_t g_frameTime;
Creatures* Creatures::m_instance = NULL;

//*********** Creature **************
Creature::Creature()
{
	m_id = 0;
	m_health = 0;
	m_lookdir = DIRECTION_NORTH;
	m_outfit.m_looktype = 0;
	m_outfit.m_lookhead = 0;
	m_outfit.m_lookbody = 0;
	m_outfit.m_looklegs = 0;
	m_outfit.m_lookfeet = 0;
	m_outfit.m_addons = 0;

	m_lightLevel = 0;
	m_lightColor = 0;
	m_speed = 0;
	m_skull = 0;
	m_shield = 0;

	m_squareColor = 0;
	m_squareStartTime = 0;

	m_moveStartTime = 0;
}

void Creature::setSquare(uint32_t color)
{
	m_squareColor = color;
	m_squareStartTime = g_frameTime;
}

void Creature::setMoving(const Position& oldPos)
{
	m_moveStartTime = g_frameTime;
	m_moveOldPos = oldPos;
}

//*********** Creatures *************
// 133 outfit colors on this table
uint32_t Creatures::OutfitLookupTable[] = {
	0xFFFFFF, 0xFFD4BF, 0xFFE9BF, 0xFFFFBF, 0xE9FFBF, 0xD4FFBF,
	0xBFFFBF, 0xBFFFD4, 0xBFFFE9, 0xBFFFFF, 0xBFE9FF, 0xBFD4FF,
	0xBFBFFF, 0xD4BFFF, 0xE9BFFF, 0xFFBFFF, 0xFFBFE9, 0xFFBFD4,
	0xFFBFBF, 0xDADADA, 0xBF9F8F, 0xBFAF8F, 0xBFBF8F, 0xAFBF8F,
	0x9FBF8F, 0x8FBF8F, 0x8FBF9F, 0x8FBFAF, 0x8FBFBF, 0x8FAFBF,
	0x8F9FBF, 0x8F8FBF, 0x9F8FBF, 0xAF8FBF, 0xBF8FBF, 0xBF8FAF,
	0xBF8F9F, 0xBF8F8F, 0xB6B6B6, 0xBF7F5F, 0xBFAF8F, 0xBFBF5F,
	0x9FBF5F, 0x7FBF5F, 0x5FBF5F, 0x5FBF7F, 0x5FBF9F, 0x5FBFBF,
	0x5F9FBF, 0x5F7FBF, 0x5F5FBF, 0x7F5FBF, 0x9F5FBF, 0xBF5FBF,
	0xBF5F9F, 0xBF5F7F, 0xBF5F5F, 0x919191, 0xBF6A3F, 0xBF943F,
	0xBFBF3F, 0x94BF3F, 0x6ABF3F, 0x3FBF3F, 0x3FBF6A, 0x3FBF94,
	0x3FBFBF, 0x3F94BF, 0x3F6ABF, 0x3F3FBF, 0x6A3FBF, 0x943FBF,
	0xBF3FBF, 0xBF3F94, 0xBF3F6A, 0xBF3F3F, 0x6D6D6D, 0xFF5500,
	0xFFAA00, 0xFFFF00, 0xAAFF00, 0x54FF00, 0x00FF00, 0x00FF54,
	0x00FFAA, 0x00FFFF, 0x00A9FF, 0x0055FF, 0x0000FF, 0x5500FF,
	0xA900FF, 0xFE00FF, 0xFF00AA, 0xFF0055, 0xFF0000, 0x484848,
	0xBF3F00, 0xBF7F00, 0xBFBF00, 0x7FBF00, 0x3FBF00, 0x00BF00,
	0x00BF3F, 0x00BF7F, 0x00BFBF, 0x007FBF, 0x003FBF, 0x0000BF,
	0x3F00BF, 0x7F00BF, 0xBF00BF, 0xBF007F, 0xBF003F, 0xBF0000,
	0x242424, 0x7F2A00, 0x7F5500, 0x7F7F00, 0x557F00, 0x2A7F00,
	0x007F00, 0x007F2A, 0x007F55, 0x007F7F, 0x00547F, 0x002A7F,
	0x00007F, 0x2A007F, 0x54007F, 0x7F007F, 0x7F0055, 0x7F002A,
	0x7F0000,
};

Creature Creatures::m_creaturesArray[CREATURES_ARRAY];

int16_t Creatures::reserveCreature(uint32_t id)
{
	for(uint32_t i = 0; i < CREATURES_ARRAY; ++i){
		if(m_creaturesArray[i].m_id == 0){
			m_creaturesArray[i].m_id = id;
			return i;
		}
	}
	// TODO (mips_act#3#): Handle error, trying to create a creature but there isnt any free slot for it!
	return -1;
}

Creature* Creatures::getPlayer()
{
	uint32_t ourId = GlobalVariables::getPlayerID();
	return getCreature(ourId);
}

Creature* Creatures::getCreature(uint32_t id)
{
	CreatureMap::iterator it = m_creaturesId.find(id);
	if(it != m_creaturesId.end()){
		return &m_creaturesArray[it->second];
	}
	else{
		return NULL;
	}
}

Creature* Creatures::addCreature(uint32_t id)
{
	int16_t i = reserveCreature(id);
	if(i >= 0){
		m_creaturesId[id] = i;
		return &m_creaturesArray[i];
	}
	else{
		// TODO (mips_act#3#): Handle error...
		return NULL;
	}
}

void Creatures::removeCreature(uint32_t id)
{
	if(id == 0){
		return;
	}

	CreatureMap::iterator it = m_creaturesId.find(id);
	if(it != m_creaturesId.end()){
		m_creaturesArray[it->second].m_id = 0;
		m_creaturesId.erase(it);
	}
	else{
		// TODO (mips_act#3#): Handle error...
	}
}

void Creatures::clear()
{
	m_creaturesId.clear();
	for(uint32_t i = 0; i < CREATURES_ARRAY; ++i){
	    m_creaturesArray[i].unloadGfx();
		m_creaturesArray[i].m_id = 0;
	}


}
