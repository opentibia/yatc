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

#include "../fassert.h"
#include "protocolgame80.h"

#include "../objects.h"
#include "../debugprint.h"

#include "../gamecontent/globalvars.h"
#include "../gamecontent/map.h"
#include "../gamecontent/item.h"
#include "../gamecontent/creature.h"
#include "../gamecontent/container.h"
#include "../gamecontent/inventory.h"

ProtocolGame80::ProtocolGame80(int account, const std::string& password, const std::string& name, bool isGM) :
ProtocolGame(account, password, name, isGM)
{
	m_skipTiles = 0;

}

void ProtocolGame80::checkVersion()
{
    ASSERT(ProtocolConfig::getInstance().getClientVersion() == CLIENT_VERSION_800);
}

ProtocolGame80::~ProtocolGame80()
{
	//
}


bool ProtocolGame80::onRecv(NetworkMessage& msg)
{
	m_currentMsg = &msg;
	m_currentMsgN++;
	while(!msg.eof()){
		MSG_READ_U8(cmd);
		addServerCmd(cmd);
		DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Received packet 0x%02x\n", cmd);
		// TODO (nfries88): move switch statement to a virtual function for simpler expansion
		bool parsed = parsePacket(cmd,msg);
		if (!parsed) { // i wanted to avoid putting brackets around everything.
		    DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Did not handle 0x%02x in first step, retrying\n", cmd);
		}
		if (!parsed)
		switch(cmd){
			// protocolgame contains 8.0 basecode
		default:
			RAISE_PROTOCOL_ERROR("Unknown opcode");
			break;
		}
	}
	return true;
}


Thing* ProtocolGame80::internalGetThing(NetworkMessage& msg)
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
			//add a new creature
			uint32_t creatureID = msg.getU32();
			creature = Creatures::getInstance().addCreature(creatureID);
			if(!creature){
				return NULL;
			}

			creature->setName(msg.getString());
		}
		if(!creature){
			return NULL;
		}

		//read creature properties
		creature->setHealth(msg.getU8());
		if(creature->getHealth() > 100){
			return NULL;
		}
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

Item* ProtocolGame80::internalGetItem(NetworkMessage& msg, uint32_t itemid)
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
	if(it->stackable || it->splash || it->fluidContainer || it->rune){
		if(!msg.getU8(count)){
			return NULL;
		}
	}

	return Item::CreateItem(itemid, count);
}

bool ProtocolGame80::internalSetOutfit(NetworkMessage& msg, Outfit_t& outfit)
{
	MSG_READ_U16(looktype);
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

//******send functions***********
//public
void ProtocolGame80::sendLogout()
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x14);
}

void ProtocolGame80::sendAutoWalk(const std::list<Direction>& steps)
{
	PROTOCOLGAME_SEND_FUNCTION;
	ASSERT(steps.size() <= 255);

	m_outputMessage.addMessageType(0x64);
	m_outputMessage.addU8(steps.size());
	std::list<Direction>::const_iterator it;
	for(it = steps.begin(); it != steps.end(); ++it){
		m_outputMessage.addU8(*it);
	}
}

void ProtocolGame80::sendStopAutoWalk()
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x69);
}

void ProtocolGame80::sendMove(Direction dir)
{
	PROTOCOLGAME_SEND_FUNCTION;
	switch(dir){
	case DIRECTION_NORTH:
		m_outputMessage.addMessageType(0x65);
		break;
	case DIRECTION_EAST:
		m_outputMessage.addMessageType(0x66);
		break;
	case DIRECTION_SOUTH:
		m_outputMessage.addMessageType(0x67);
		break;
	case DIRECTION_WEST:
		m_outputMessage.addMessageType(0x68);
		break;
	case DIRECTION_NE:
		m_outputMessage.addMessageType(0x6A);
		break;
	case DIRECTION_SE:
		m_outputMessage.addMessageType(0x6B);
		break;
	case DIRECTION_SW:
		m_outputMessage.addMessageType(0x6C);
		break;
	case DIRECTION_NW:
		m_outputMessage.addMessageType(0x6D);
		break;
	default:
		ASSERT(0);
		break;
	}
}

void ProtocolGame80::sendTurn(Direction dir)
{
	PROTOCOLGAME_SEND_FUNCTION;
	switch(dir){
	case DIRECTION_NORTH:
		m_outputMessage.addMessageType(0x6F);
		break;
	case DIRECTION_EAST:
		m_outputMessage.addMessageType(0x70);
		break;
	case DIRECTION_SOUTH:
		m_outputMessage.addMessageType(0x71);
		break;
	case DIRECTION_WEST:
		m_outputMessage.addMessageType(0x72);
		break;
	default:
		ASSERT(0);
		break;
	}
}

void ProtocolGame80::sendThrow(const Position& fromPos, uint16_t itemid,
				uint8_t fromStackpos, const Position& toPos, uint8_t count)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x78);
	m_outputMessage.addPosition(fromPos);
	m_outputMessage.addU16(itemid);
	m_outputMessage.addU8(fromStackpos);
	m_outputMessage.addPosition(toPos);
	m_outputMessage.addU8(count);
}

void ProtocolGame80::sendRequestTrade(const Position& pos, uint16_t itemid,
			uint8_t stackpos, uint32_t playerid)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x7D);
	m_outputMessage.addPosition(pos);
	m_outputMessage.addU16(itemid);
	m_outputMessage.addU8(stackpos);
	m_outputMessage.addU32(playerid);
}

void ProtocolGame80::sendLookInTrade(bool inMyOffer, uint8_t index)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x7E);
	m_outputMessage.addU8(inMyOffer ? 0 : 1);
	m_outputMessage.addU8(index);
}

void ProtocolGame80::sendAcceptTrade()
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x7F);
}

void ProtocolGame80::sendRejectTrade()
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x80);
}

void ProtocolGame80::sendUseItem(const Position& pos, uint16_t itemid,
			uint8_t stackpos)
{
	PROTOCOLGAME_SEND_FUNCTION;
	int8_t index = Containers::getInstance().getFreeContainerSlot();
	m_outputMessage.addMessageType(0x82);
	m_outputMessage.addPosition(pos);
	m_outputMessage.addU16(itemid);
	m_outputMessage.addU8(stackpos);
	m_outputMessage.addU8(index);
}

void ProtocolGame80::sendUseItemWith(const Position& fromPos, uint16_t fromItemid,
			uint8_t fromStackpos, const Position& toPos, uint16_t toItemid,
			uint8_t toStackpos)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x83);
	m_outputMessage.addPosition(fromPos);
	m_outputMessage.addU16(fromItemid);
	m_outputMessage.addU8(fromStackpos);
	m_outputMessage.addPosition(toPos);
	m_outputMessage.addU16(toItemid);
	m_outputMessage.addU8(toStackpos);
}

void ProtocolGame80::sendUseBattleWindow(const Position& pos, uint16_t itemid,
			uint8_t stackpos, uint32_t creatureid)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x84);
	m_outputMessage.addPosition(pos);
	m_outputMessage.addU16(itemid);
	m_outputMessage.addU8(stackpos);
	m_outputMessage.addU32(creatureid);
}

void ProtocolGame80::sendRotateItem(const Position& pos, uint16_t itemid,
			uint8_t stackpos)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x85);
	m_outputMessage.addPosition(pos);
	m_outputMessage.addU16(itemid);
	m_outputMessage.addU8(stackpos);
}

void ProtocolGame80::sendCloseContainer(uint8_t containerIndex)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x87);
	m_outputMessage.addU8(containerIndex);
}

void ProtocolGame80::sendUpContainer(uint8_t containerIndex)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x88);
	m_outputMessage.addU8(containerIndex);
}

void ProtocolGame80::sendTextWindow(uint32_t windowId, const std::string& text)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x89);
	m_outputMessage.addU32(windowId);
	m_outputMessage.addString(text);
}

void ProtocolGame80::sendHouseWindow(uint32_t windowId, int unk,
			const std::string& text)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x8A);
	m_outputMessage.addU8(unk);
	m_outputMessage.addU32(windowId);
	m_outputMessage.addString(text);
}

void ProtocolGame80::sendLookItem(const Position& pos, uint16_t itemid,
			uint8_t stackpos)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x8C);
	m_outputMessage.addPosition(pos);
	m_outputMessage.addU16(itemid);
	m_outputMessage.addU8(stackpos);
}

void ProtocolGame80::sendSay(SpeakClasses_t type, const std::string text)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x96);
	m_outputMessage.addU8(type);
	m_outputMessage.addString(text);
}

void ProtocolGame80::sendSay(SpeakClasses_t type, uint16_t channel,
			const std::string text)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x96);
	m_outputMessage.addU8(type);
	m_outputMessage.addU16(channel);
	m_outputMessage.addString(text);
}

void ProtocolGame80::sendSay(SpeakClasses_t type, const std::string& sendToplayer,
			const std::string text)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x96);
	m_outputMessage.addU8(type);
	m_outputMessage.addString(sendToplayer);
	m_outputMessage.addString(text);
}

void ProtocolGame80::sendRequestChannels()
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x97);
}

void ProtocolGame80::sendOpenChannel(uint16_t channelid)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x98);
	m_outputMessage.addU16(channelid);
}

void ProtocolGame80::sendCloseChannel(uint16_t channelid)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x99);
	m_outputMessage.addU16(channelid);
}

void ProtocolGame80::sendOpenPrivatePlayerChat(const std::string& playerName)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x9A);
	m_outputMessage.addString(playerName);
}

void ProtocolGame80::sendOpenRuleViolation(const std::string& text)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x9B);
	m_outputMessage.addString(text);
}

void ProtocolGame80::sendCloseRuleViolation(const std::string& text)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x9C);
	m_outputMessage.addString(text);
}

void ProtocolGame80::sendCancelRuleViolation()
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x9D);
}

void ProtocolGame80::sendFightModes(uint8_t attack, uint8_t chase, uint8_t secure)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xA0);
	m_outputMessage.addU8(attack);
	m_outputMessage.addU8(chase);
	m_outputMessage.addU8(secure);
}

void ProtocolGame80::sendAttackCreature(uint32_t creatureid)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xA1);
	m_outputMessage.addU32(creatureid);
	GlobalVariables::setAttackID(creatureid);
}

void ProtocolGame80::sendFollowCreature(uint32_t creatureid)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xA2);
	m_outputMessage.addU32(creatureid);
	GlobalVariables::setFollowID(creatureid);
}

void ProtocolGame80::sendCreatePrivateChatChannel()
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xAA);
}

void ProtocolGame80::sendInvitePrivateChatChannel(const std::string& name)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xAB);
	m_outputMessage.addString(name);
}

void ProtocolGame80::sendExcludePrivateChatChannel(const std::string& name)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xAC);
	m_outputMessage.addString(name);
}

void ProtocolGame80::sendInviteParty(uint32_t playerid)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xA3);
	m_outputMessage.addU32(playerid);
}

void ProtocolGame80::sendJoinParty(uint32_t playerid)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xA4);
	m_outputMessage.addU32(playerid);
}

void ProtocolGame80::sendCancelInviteParty(uint32_t playerid)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xA5);
	m_outputMessage.addU32(playerid);
}

void ProtocolGame80::sendPassPartyLeader(uint32_t playerid)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xA6);
	m_outputMessage.addU32(playerid);
}

void ProtocolGame80::sendLeaveParty()
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xA7);
}

void ProtocolGame80::sendCancelMove()
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xBE);
	GlobalVariables::setAttackID(0);
	GlobalVariables::setFollowID(0);
}

void ProtocolGame80::sendRequestOutfit()
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xD2);
}

void ProtocolGame80::sendSetOutfit(uint16_t looktype, uint8_t head, uint8_t body,
			uint8_t legs, uint8_t feet)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xD3);
	m_outputMessage.addU16(looktype);
	m_outputMessage.addU8(head);
	m_outputMessage.addU8(body);
	m_outputMessage.addU8(legs);
	m_outputMessage.addU8(feet);
}

void ProtocolGame80::sendAddVip(const std::string& name)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xDC);
	m_outputMessage.addString(name);
}

void ProtocolGame80::sendRemVip(uint32_t playerid)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xDD);
	m_outputMessage.addU32(playerid);
}

void ProtocolGame80::sendBugReport(const std::string& text)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xE6);
	m_outputMessage.addString(text);
}

void ProtocolGame80::sendRequestQuestLog()
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xF0);
}

void ProtocolGame80::sendRequestQuest(uint16_t questid)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xF1);
	m_outputMessage.addU16(questid);
}

//protected
void ProtocolGame80::sendPing()
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x1E);
}

void ProtocolGame80::sendRequestUpdateTile(const Position& pos)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xC9);
	m_outputMessage.addPosition(pos);
}

void ProtocolGame80::sendRequestUpdateContainer(uint8_t containerid)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xCA);
	m_outputMessage.addU8(containerid);
}
