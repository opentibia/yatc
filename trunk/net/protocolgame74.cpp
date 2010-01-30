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

#include "protocolgame74.h"

#include "../notifications.h"
#include "../gamecontent/globalvars.h"
#include "../gamecontent/creature.h"
#include "../gamecontent/item.h"
#include "../debugprint.h"

ProtocolGame74::ProtocolGame74(const std::string& accountname, const std::string& password, const std::string& name, bool isGM) :
    ProtocolGame(accountname, password, name, isGM)
{

}

void ProtocolGame74::checkVersion()
{
    ASSERT(ProtocolConfig::getInstance().getClientVersion() == CLIENT_VERSION_740);
}

bool ProtocolGame74::onRecv(NetworkMessage& msg)
{
	m_currentMsg = &msg;
	m_currentMsgN++;
	while(!msg.eof()){
		MSG_READ_U8(cmd);
		addServerCmd(cmd);
		DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Received packet 0x%02x\n", cmd);
		bool parsed = parsePacket(cmd,msg);
		if (!parsed) { // i wanted to avoid putting brackets around everything.
		    DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Did not handle 0x%02x in first step, retrying\n", cmd);
		}
		if (!parsed) {
			RAISE_PROTOCOL_ERROR("Unknown opcode");
		}

	}
	return true;
}

ProtocolGame74::~ProtocolGame74()
{
	//
}

void ProtocolGame74::onConnect()
{
    sendLogin();
}

void ProtocolGame74::sendLogin(NetworkMessage* msg)
{
	printf("Logging into gameworld server\n");
	ProtocolConfig& config = ProtocolConfig::getInstance();

	NetworkMessage output(NetworkMessage::CAN_WRITE);
	output.addU8(0x0A); //Game world Protocol
	output.addU16(config.getOS());
	output.addU16(config.getVersionOverride() ? config.getVersionOverride() : config.getClientVersion());

	output.addU8(m_isGM);
	if(!m_usesaccountname)
		output.addU32(m_account);
	else {
		output.addString(m_accountname);
	}
	output.addString(m_name);
	output.addString(m_password);

	if (msg)
	{
	    // Packet of form 0x1F
	    // A sort of timestamp and random byte(?): Server will disallow clients if it doesn't match.
 	    // Probably used to make sure that login packets cannot be spied from the network and replayed.
	    output.addU32(msg->getU32()); // timestamp
	    output.addU8(msg->getU8());   // random byte (?)
	}

//	bool oldChecksumState = m_connection->getChecksumState();
//	m_connection->setChecksumState(false);

	m_connection->sendMessage(output);
	m_connection->setCryptoState(false);
//	m_connection->setChecksumState(oldChecksumState);

	m_account = 0;
	m_password = "";
	m_name = "";
	m_isGM = 0;
	checkVersion();
}

bool ProtocolGame74::parsePlayerStats(NetworkMessage& msg)
{
    // NOTE (nfries88): No percentages pre-7.6, soul was also added 7.6, stamina was added even later
	MSG_READ_U16(health);
	MSG_READ_U16(healthMax);
	MSG_READ_U16(capacity);
	MSG_READ_U32(experience);
	MSG_READ_U16(level);
	MSG_READ_U16(mana);
	MSG_READ_U16(manaMax);
	MSG_READ_U8(magicLevel);

	//some validations
	// NOTE (nfries88): sometimes, upon death, your mana will be greater than your maximum mana (due to level loss)
	if(health > healthMax){
		RAISE_PROTOCOL_ERROR("Player stats - values");
	}

	GlobalVariables::setPlayerStat(STAT_HEALTH, health);
	GlobalVariables::setPlayerStat(STAT_HEALTH_MAX, healthMax);
	GlobalVariables::setPlayerStat(STAT_CAPACITY, capacity);
	GlobalVariables::setPlayerStat(STAT_EXPERIENCE, experience);
	GlobalVariables::setPlayerSkill(SKILL_LEVEL, SKILL_ATTR_LEVEL, level);
	GlobalVariables::setPlayerStat(STAT_MANA, mana);
	GlobalVariables::setPlayerStat(STAT_MANA_MAX, manaMax);
	GlobalVariables::setPlayerSkill(SKILL_MAGIC, SKILL_ATTR_LEVEL, magicLevel);
	Notifications::onChangeStats();

	return true;
}
bool ProtocolGame74::parsePlayerSkills(NetworkMessage& msg)
{
    // NOTE (nfries88): No percentages pre-7.6
	MSG_READ_U8(fist);
	MSG_READ_U8(club);
	MSG_READ_U8(sword);
	MSG_READ_U8(axe);
	MSG_READ_U8(distance);
	MSG_READ_U8(shield);
	MSG_READ_U8(fish);

	GlobalVariables::setPlayerSkill(SKILL_FIST, SKILL_ATTR_LEVEL, fist);
	GlobalVariables::setPlayerSkill(SKILL_CLUB, SKILL_ATTR_LEVEL, club);
	GlobalVariables::setPlayerSkill(SKILL_SWORD, SKILL_ATTR_LEVEL, sword);
	GlobalVariables::setPlayerSkill(SKILL_AXE, SKILL_ATTR_LEVEL, axe);
	GlobalVariables::setPlayerSkill(SKILL_DISTANCE, SKILL_ATTR_LEVEL, distance);
	GlobalVariables::setPlayerSkill(SKILL_SHIELD, SKILL_ATTR_LEVEL, shield);
	GlobalVariables::setPlayerSkill(SKILL_FISH, SKILL_ATTR_LEVEL, fish);

	Notifications::onChangeSkills();

	return true;
}
bool ProtocolGame74::parseCreatureSpeak(NetworkMessage& msg)
{
	//MSG_READ_U32(unkSpeak);
	MSG_READ_STRING(name);
	MSG_READ_U8(type);
	SpeakClasses_t newtype = (SpeakClasses_t)translateSpeakClassToInternal((char)type);
	printf("Translated incoming %02x into %02x\n", type, newtype);
	switch(newtype){
		case SPEAK_SAY:
		case SPEAK_WHISPER:
		case SPEAK_YELL:
		case SPEAK_MONSTER_SAY:
		case SPEAK_MONSTER_YELL:
		case SPEAK_PRIVATE_NP:
		case SPEAK_PRIVATE_PN:
		{
			MSG_READ_POSITION(creaturePos);
			MSG_READ_STRING(text);
			Notifications::onCreatureSpeak((SpeakClasses_t)newtype, 0, name, 0, creaturePos, text);
			break;
		}
		case SPEAK_CHANNEL_R1:
		case SPEAK_CHANNEL_R2:
		case SPEAK_CHANNEL_O:
		case SPEAK_CHANNEL_Y:
		case SPEAK_CHANNEL_W:
		{
			MSG_READ_U16(channelID);
			MSG_READ_STRING(text);
			Notifications::onCreatureSpeak((SpeakClasses_t)newtype, 0, name, 0, channelID, text);
			break;
		}
		case SPEAK_PRIVATE:
		case SPEAK_BROADCAST:
		case SPEAK_PRIVATE_RED:
		{
			MSG_READ_STRING(text);
			Notifications::onCreatureSpeak((SpeakClasses_t)newtype, 0, name, 0, text);
			break;
		}
		case SPEAK_CHANNEL_UNK6:
		{
			//TODO. Rule violations
			MSG_READ_U32(number);
			MSG_READ_STRING(text);
			break;
		}
		case SPEAK_CHANNEL_UNK7:
		case SPEAK_CHANNEL_UNK8:
		{
			//TODO. Rule violations
			MSG_READ_STRING(text);
			break;
		}
		default:
			RAISE_PROTOCOL_ERROR("Creature speak - type");
			break;
	}
	return true;
}
bool ProtocolGame74::parseOutfitWindow(NetworkMessage& msg)
{
	Outfit_t outfit;
	if(!internalSetOutfit(msg, outfit)){
		RAISE_PROTOCOL_ERROR("Outfit window - outfit error");
	}
	MSG_READ_U8(nOutfits);
	if(nOutfits == 0 || nOutfits > 25){
		RAISE_PROTOCOL_ERROR("Outfit window - n = 0 || n > 25");
	}
	std::list<AvailOutfit_t> list;
	for(uint32_t i = 0; i < nOutfits; ++i){
	    // NOTE (nfries88): This is way different pre-7.8
		AvailOutfit_t avail;
		MSG_READ_U16(outfitID);
		if(!Objects::getInstance()->getOutfitType(outfitID)){
			RAISE_PROTOCOL_ERROR("Outfit window  - outfit list error");
		}
		MSG_READ_STRING(name);
		MSG_READ_U8(addons);
		avail.id = outfitID;
		avail.name = name;
		avail.addons = addons;
		list.push_back(avail);
	}
	printf("Current outfit: %d - %d %d %d %d - %d\n",  outfit.m_looktype, outfit.m_lookhead, outfit.m_lookbody, outfit.m_looklegs, outfit.m_lookfeet, outfit.m_lookitem);
	Notifications::openOutfitWindow(outfit, list);
	return true;
}

//******send functions***********
//public
void ProtocolGame74::sendFightModes(uint8_t attack, uint8_t chase, uint8_t secure)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xA0);
	m_outputMessage.addU8(attack);
	m_outputMessage.addU8(chase);
}

void ProtocolGame74::sendSetOutfit(uint16_t looktype, uint8_t head, uint8_t body,
								   uint8_t legs, uint8_t feet, uint8_t addons)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xD3);
	// NOTE (nfries88): Looktype was a U8 up until 7.7 or 7.8, forget which.
	// There were no addons until 7.8
	m_outputMessage.addU8(looktype);
	m_outputMessage.addU8(head);
	m_outputMessage.addU8(body);
	m_outputMessage.addU8(legs);
	m_outputMessage.addU8(feet);
}

void ProtocolGame74::sendGMReport(const std::string& targetplayer, uint8_t reason, const std::string& comment, const std::string& statement, uint8_t action, bool ipban)
{
    // NOTE (nfries88): At least will work with 7.4 Avesta...
    PROTOCOLGAME_SEND_FUNCTION;
    m_outputMessage.addMessageType(0xE7);
    m_outputMessage.addString(targetplayer);
    m_outputMessage.addU8(reason);
    m_outputMessage.addString(comment);
    m_outputMessage.addU8(action);
    m_outputMessage.addU8(ipban ? 1 : 0);
}

bool ProtocolGame74::internalSetOutfit(NetworkMessage& msg, Outfit_t& outfit)
{
	MSG_READ_U8(looktype);
	outfit.m_looktype = looktype;
	if(outfit.m_looktype != 0){
		if(!Objects::getInstance()->getOutfitType(outfit.m_looktype)){
			return false;
		}
		MSG_READ_U8(head);
		MSG_READ_U8(body);
		MSG_READ_U8(legs);
		MSG_READ_U8(feet);
		MSG_READ_U8(addons);
		outfit.m_lookhead = head;
		outfit.m_lookbody = body;
		outfit.m_looklegs = legs;
		outfit.m_lookfeet = feet;
		outfit.m_addons = addons;
	}
	else{
		uint16_t  itemid;
		if(!msg.getU16(itemid)){
			return false;
		}
		if(itemid != 0 && !Objects::getInstance()->getItemType(itemid)){
			return false;
		}
		outfit.m_lookitem = itemid;
	}
	return true;
}

Thing* ProtocolGame74::internalGetThing(NetworkMessage& msg)
{
	//get thing type
	uint16_t thingId = msg.getU16();

	if(thingId == 0x0061 || thingId == 0x0062) {
		//creatures
		Creature* creature = NULL;
		if(thingId == 0x0062){ //creature is known
			uint32_t creatureID = msg.getU32();
			creature = Creatures::getInstance().getCreature(creatureID);
		}
		else if(thingId == 0x0061){ //creature is not known
			//perhaps we have to remove a known creature
			uint32_t removeID = msg.getU32();
			Creatures::getInstance().removeCreature(removeID);
			Notifications::onRemoveCreature(removeID);
			//add a new creature
			uint32_t creatureID = msg.getU32();
			creature = Creatures::getInstance().addCreature(creatureID);
			if(!creature){
				return NULL;
			}
			Notifications::onAddCreature(creatureID);

			creature->setName(msg.getString());
		}
		if(!creature){
			return NULL;
		}

		//read creature properties
		uint8_t hp = msg.getU8();
		creature->setHealth(hp);
		if(creature->getHealth() > 100){
			return NULL;
		}
		Notifications::onCreatureChangeHealth(creature->getID(), hp);
		//
		uint8_t direction;
		if(!msg.getU8(direction) || direction > 3){
			return NULL;
		}
		creature->setLookDir((Direction)direction);
		creature->setTurnDir((Direction)direction);
		//

		if(!internalSetOutfit(msg, creature->getOutfit())){
			return NULL;
		}
		creature->loadOutfit();

		//check if we can read 6 bytes
		if(!msg.canRead(6)){
			return NULL;
		}

		creature->setLightLevel(msg.getU8());
		creature->setLightColor(msg.getU8());

		creature->setSpeed(msg.getU16());

		creature->setSkull(msg.getU8());
		creature->setShield(msg.getU8());

		return creature;
	}
	else if(thingId == 0x0063){
		//creature turn
		uint32_t creatureID = msg.getU32();
		Creature* creature = Creatures::getInstance().getCreature(creatureID);
		if(!creature){
			return NULL;
		}
		//check if we can read 1 byte
		uint8_t direction;
		if(!msg.getU8(direction) || direction > 3){
			return NULL;
		}
		creature->setTurnDir((Direction)direction);

		return creature;
	}
	else{
		//item
		return internalGetItem(msg, thingId);
	}
}

Item* ProtocolGame74::internalGetItem(NetworkMessage& msg, uint32_t itemid)
{
	if(itemid == (uint32_t)-1){
		uint16_t u16;
		if(!msg.getU16(u16)){
			return NULL;
		}
		itemid = u16;
	}

	ObjectType* it = Objects::getInstance()->getItemType(itemid);
	if(!it){
		return NULL;
	}

	uint8_t count = 0;
	if(it->stackable || it->splash || it->fluidContainer){
		if(!msg.getU8(count)){
			return NULL;
		}
	}

	return Item::CreateItem(itemid, count);
}
