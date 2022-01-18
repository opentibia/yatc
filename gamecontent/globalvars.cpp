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

#include "globalvars.h"

uint32_t GlobalVariables::m_playerId = 0;
uint32_t GlobalVariables::m_attackId = 0;
uint32_t GlobalVariables::m_followId = 0;
Position GlobalVariables::m_playerPos(0,0,0);
bool GlobalVariables::m_canReportBugs = 0;
uint8_t GlobalVariables::m_worldLightColor = 0;
uint8_t GlobalVariables::m_worldLightLevel = 0;
uint8_t GlobalVariables::m_GMActions[32] = {0};
uint32_t GlobalVariables::m_playerIcons = 0;
uint32_t GlobalVariables::m_stats[STAT_LAST] = {0};
uint32_t GlobalVariables::m_skills[SKILL_LAST][2] = {{0}};
uint32_t GlobalVariables::m_cash = 0;

float GlobalVariables::m_scale = 1.F;

void GlobalVariables::clear()
{
	m_playerId = 0;
	m_playerPos.x = 0;
	m_playerPos.y = 0;
	m_playerPos.z = 0;
	m_canReportBugs = false;
	m_worldLightColor = 0;
	m_worldLightLevel = 0;
	m_playerIcons = 0;
	for(uint32_t i = 0; i < sizeof(m_GMActions); ++i){
		m_GMActions[i] = 0;
	}
	for(uint32_t i = 0; i < STAT_LAST; ++i){
		m_stats[i] = 0;
	}
	for(uint32_t i = 0; i < SKILL_LAST; ++i){
		for(uint32_t j = 0; j < SKILL_ATTR_LAST; ++j){
			m_skills[i][j] = 0;
		}
	}
}

uint32_t GlobalVariables::getPlayerStat(Stat_t s)
{
	if(s >=0 && s < STAT_LAST){
		return m_stats[s];
	}
	else{
		//TODO. error
		return 0;
	}
}

void GlobalVariables::setPlayerStat(Stat_t s, uint32_t v)
{
	if(s >=0 && s < STAT_LAST){
		m_stats[s] = v;
	}
	else{
		//TODO. error
	}
}

uint32_t GlobalVariables::getPlayerSkill(SkillList_t skill, SkillAttr_t attr )
{
	if(skill >= 0 && skill < SKILL_LAST && attr >= 0 && attr < SKILL_ATTR_LAST){
		return m_skills[skill][attr];
	}
	else{
		//TODO. error
		return 0;
	}
}

void GlobalVariables::setPlayerSkill(SkillList_t skill, SkillAttr_t attr, uint32_t v)
{
	if(skill >= 0 && skill < SKILL_LAST && attr >= 0 && attr < SKILL_ATTR_LAST){
		m_skills[skill][attr] = v;
	}
	else{
		//TODO. error
	}
}
