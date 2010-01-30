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

#include "protocolgame76.h"

#include "../notifications.h"
#include "../gamecontent/globalvars.h"
#include "../gamecontent/creature.h"
#include "../gamecontent/item.h"

ProtocolGame76::ProtocolGame76(const std::string& accountname, const std::string& password, const std::string& name, bool isGM) :
    ProtocolGame74(accountname, password, name, isGM)
{

}
ProtocolGame76::~ProtocolGame76(){}

void ProtocolGame76::checkVersion()
{
    ASSERT(ProtocolConfig::getInstance().getClientVersion() == CLIENT_VERSION_760);
}

bool ProtocolGame76::parsePlayerStats(NetworkMessage& msg)
{
	MSG_READ_U16(health);
	MSG_READ_U16(healthMax);
	MSG_READ_U16(capacity);
	MSG_READ_U32(experience);
	MSG_READ_U16(level);
	MSG_READ_U8(levelPercent);
	MSG_READ_U16(mana);
	MSG_READ_U16(manaMax);
	MSG_READ_U8(magicLevel);
	MSG_READ_U8(magicLevelPercent);
	MSG_READ_U8(soul);
	MSG_READ_U16(stamina);

	//some validations
	// NOTE (nfries88): sometimes, upon death, your mana will be greater than your maximum mana (due to level loss)
	if(health > healthMax || levelPercent > 100 || magicLevelPercent > 100){
		RAISE_PROTOCOL_ERROR("Player stats - values");
	}

	GlobalVariables::setPlayerStat(STAT_HEALTH, health);
	GlobalVariables::setPlayerStat(STAT_HEALTH_MAX, healthMax);
	GlobalVariables::setPlayerStat(STAT_CAPACITY, capacity);
	GlobalVariables::setPlayerStat(STAT_EXPERIENCE, experience);
	GlobalVariables::setPlayerSkill(SKILL_LEVEL, SKILL_ATTR_LEVEL, level);
	GlobalVariables::setPlayerSkill(SKILL_LEVEL, SKILL_ATTR_PERCENT, levelPercent);
	GlobalVariables::setPlayerStat(STAT_MANA, mana);
	GlobalVariables::setPlayerStat(STAT_MANA_MAX, manaMax);
	GlobalVariables::setPlayerSkill(SKILL_MAGIC, SKILL_ATTR_LEVEL, magicLevel);
	GlobalVariables::setPlayerSkill(SKILL_MAGIC, SKILL_ATTR_PERCENT, magicLevelPercent);
	GlobalVariables::setPlayerStat(STAT_SOUL, soul);
	GlobalVariables::setPlayerStat(STAT_STAMINA, stamina);

	Notifications::onChangeStats();

	return true;
}
bool ProtocolGame76::parsePlayerSkills(NetworkMessage& msg)
{
	MSG_READ_U8(fist);
	MSG_READ_U8(fistPercent);
	MSG_READ_U8(club);
	MSG_READ_U8(clubPercent);
	MSG_READ_U8(sword);
	MSG_READ_U8(swordPercent);
	MSG_READ_U8(axe);
	MSG_READ_U8(axePercent);
	MSG_READ_U8(distance);
	MSG_READ_U8(distancePercent);
	MSG_READ_U8(shield);
	MSG_READ_U8(shieldPercent);
	MSG_READ_U8(fish);
	MSG_READ_U8(fishPercent);
	//some validations
	if(fistPercent > 100 || clubPercent > 100 || swordPercent > 100 ||
	   axePercent > 100 || distancePercent > 100 || shieldPercent > 100 ||
	   fishPercent > 100){
		RAISE_PROTOCOL_ERROR("Player skills - values");
	}

	GlobalVariables::setPlayerSkill(SKILL_FIST, SKILL_ATTR_LEVEL, fist);
	GlobalVariables::setPlayerSkill(SKILL_FIST, SKILL_ATTR_PERCENT, fistPercent);
	GlobalVariables::setPlayerSkill(SKILL_CLUB, SKILL_ATTR_LEVEL, club);
	GlobalVariables::setPlayerSkill(SKILL_CLUB, SKILL_ATTR_PERCENT, clubPercent);
	GlobalVariables::setPlayerSkill(SKILL_SWORD, SKILL_ATTR_LEVEL, sword);
	GlobalVariables::setPlayerSkill(SKILL_SWORD, SKILL_ATTR_PERCENT, swordPercent);
	GlobalVariables::setPlayerSkill(SKILL_AXE, SKILL_ATTR_LEVEL, axe);
	GlobalVariables::setPlayerSkill(SKILL_AXE, SKILL_ATTR_PERCENT, axePercent);
	GlobalVariables::setPlayerSkill(SKILL_DISTANCE, SKILL_ATTR_LEVEL, distance);
	GlobalVariables::setPlayerSkill(SKILL_DISTANCE, SKILL_ATTR_PERCENT, distancePercent);
	GlobalVariables::setPlayerSkill(SKILL_SHIELD, SKILL_ATTR_LEVEL, shield);
	GlobalVariables::setPlayerSkill(SKILL_SHIELD, SKILL_ATTR_PERCENT, shieldPercent);
	GlobalVariables::setPlayerSkill(SKILL_FISH, SKILL_ATTR_LEVEL, fish);
	GlobalVariables::setPlayerSkill(SKILL_FISH, SKILL_ATTR_PERCENT, fishPercent);

	Notifications::onChangeSkills();

	return true;
}
