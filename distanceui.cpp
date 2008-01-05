//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
// Distanceui instance class
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

#include "distanceui.h"
#include "debugprint.h"
#include "fassert.h"
#include "math.h"

extern uint32_t g_frameTime;

DistanceUI::DistanceUI(const Position& from, const Position& to, uint32_t type) : ThingUI()
{
	m_obj = Objects::getInstance()->getDistanceType(type);

	m_startTime = g_frameTime;
	m_from = from;
	m_to = to;

	//Calculate distance effect direction
	float tan;
	int32_t dx = from.x - to.x;
	int32_t dy = from.y - to.y;
	if(dx != 0.f){
		tan = (float)dy/(float)dx;
	}
	else{
		tan = 10.f;
	}
	if(fabs(tan) < 0.4142f){
		if(dx > 0)
			m_direction = 3;//DIR_W;
		else
			m_direction = 5;//DIR_E;
	}
	else if(fabs(tan) < 2.4142f){
		if(tan > 0.f){
			if(dy > 0)
				m_direction = 0;//DIR_NW;
			else
				m_direction = 8;//DIR_SE;
		}
		else{ //tan < 0
			if(dx > 0)
				m_direction = 6;//DIR_SW;
			else
				m_direction = 2;//DIR_NE;
		}
	}
	else{
		if(dy > 0)
			m_direction = 1;//DIR_N;
		else
			m_direction = 7;//DIR_S;
	}
	//Distance effect speed = 10 tile/s
	m_flightTime = sqrt(dx*dx + dy*dy)/10.f;

	if(!m_obj){
		DEBUGPRINT(DEBUGPRINT_ERROR, DEBUGPRINT_LEVEL_OBLIGATORY, "[DistanceUI::DistanceUI] Invalid distance effect %d\n", type);
		return;
	}

	m_obj->loadGfx();
	m_obj->instancesOnMap++;
}

float DistanceUI::getFlightProgress()
{
	return (g_frameTime - m_startTime)/(1000.f*m_flightTime);
}

DistanceUI::~DistanceUI()
{
	if(m_obj){
		m_obj->instancesOnMap--;
		if (!m_obj->instancesOnMap) m_obj->unloadGfx();
	}
}

void DistanceUI::Blit(int x, int y, float scale, int map_x, int map_y) const
{
	if(!m_obj)
		return;

	uint32_t spriteSize = m_obj->width * m_obj->height;

	uint32_t activeframe = m_direction * spriteSize;

	for(uint32_t i = 0; i < m_obj->height; ++i){
		for(uint32_t j = 0; j < m_obj->width; ++j){

			ASSERT(activeframe < m_obj->numsprites);

			m_obj->getGfx()[activeframe]->Blit(x - j*32*scale, y - i*32*scale, 0, 0, 32, 32, 32*scale, 32*scale);
			activeframe++;
		}
	}
}
