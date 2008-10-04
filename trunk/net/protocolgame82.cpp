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


// WARNING!
// Current implementation is not completely verified.
// It's based on this thread: http://otfans.net/showthread.php?t=119929
// This has not been thouroughly tested because, simply, there is no server
// yet that can truly handle 8.2.

#include "../fassert.h"
#include "protocolgame82.h"

#include "../objects.h"
#include "../debugprint.h"

#include "../gamecontent/globalvars.h"
#include "../gamecontent/map.h"
#include "../gamecontent/item.h"
#include "../gamecontent/creature.h"
#include "../gamecontent/container.h"
#include "../gamecontent/inventory.h"
#include "../gamecontent/shop.h"

ProtocolGame82::ProtocolGame82(int account, const std::string& password, const std::string& name, bool isGM) :
ProtocolGame(account, password, name, isGM)
{
	m_skipTiles = 0;

}

void ProtocolGame82::checkVersion()
{
    ASSERT(ProtocolConfig::getInstance().getClientVersion() == CLIENT_VERSION_820);
}

ProtocolGame82::~ProtocolGame82()
{
	//
}

bool ProtocolGame82::parsePacket(uint8_t cmd, NetworkMessage& msg)
{
    // example for overrides
    // this is a bad example since parseSelfAppear is already virtual
    // this should be used in case some function is not virtual
    switch(cmd){
    case 0x0A:
        return parseSelfAppear(msg);
    }
    return ProtocolGame::parsePacket(cmd,msg);
}

bool ProtocolGame82::onRecv(NetworkMessage& msg)
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
		case 0x7A: // npc inventory listing for 8.2+, thomac decoded: see http://otfans.net/showpost.php?p=1004578&postcount=59
		{
		    // FIXME STUB!!!!!
		    std::list<ShopItem> shopList;
		    MSG_READ_U8(size);
		    for(uint32_t i = 0; i < size; ++i){
				MSG_READ_U16(itemid);
				MSG_READ_U8(runecharges); // always present
				MSG_READ_STRING(itemname);
				MSG_READ_U32(buyprice);
				MSG_READ_U32(sellprice);
				shopList.push_back(ShopItem(itemname, itemid, runecharges, buyprice, sellprice));
			}
			Notifications::openShopWindow(shopList);
			break;
		}
		case 0x7B: // player's cash for 8.2+
        {
            MSG_READ_U32(cash);
            GlobalVariables::setPlayerCash(cash);
            Notifications::onUpdatePlayerCash(cash);
            break;
        }
        case 0x7C: // close shop opened with 0x7A for 8.2+
        {
        	Notifications::closeShopWindow();
            break;
        }

		case 0xDC: // show tutorial - 8.2+
		{
			MSG_READ_U8(tutorialID);
			Notifications::showTutorial(tutorialID);
			break;
		}
		case 0xDD: // minimap mark - 8.2+
		{
			MSG_READ_POSITION(myPos);
			MSG_READ_U8(icon);
			MSG_READ_STRING(desc);
			Notifications::addMapMark(icon, myPos, desc);
			break;
		}
		default:
			RAISE_PROTOCOL_ERROR("Unknown opcode");
			break;
		}
	}
	return true;
}


Thing* ProtocolGame82::internalGetThing(NetworkMessage& msg)
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

Item* ProtocolGame82::internalGetItem(NetworkMessage& msg, uint32_t itemid)
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

bool ProtocolGame82::internalSetOutfit(NetworkMessage& msg, Outfit_t& outfit)
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
void ProtocolGame82::sendLogout()
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x14);
}

void ProtocolGame82::sendAutoWalk(const std::list<Direction>& steps)
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

void ProtocolGame82::sendStopAutoWalk()
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x69);
}

void ProtocolGame82::sendMove(Direction dir)
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

void ProtocolGame82::sendTurn(Direction dir)
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

void ProtocolGame82::sendThrow(const Position& fromPos, uint16_t itemid,
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

void ProtocolGame82::sendRequestTrade(const Position& pos, uint16_t itemid,
			uint8_t stackpos, uint32_t playerid)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x7D);
	m_outputMessage.addPosition(pos);
	m_outputMessage.addU16(itemid);
	m_outputMessage.addU8(stackpos);
	m_outputMessage.addU32(playerid);
}

void ProtocolGame82::sendLookInTrade(bool inMyOffer, uint8_t index)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x7E);
	m_outputMessage.addU8(inMyOffer ? 0 : 1);
	m_outputMessage.addU8(index);
}

void ProtocolGame82::sendAcceptTrade()
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x7F);
}

void ProtocolGame82::sendRejectTrade()
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x80);
}

void ProtocolGame82::sendUseItem(const Position& pos, uint16_t itemid,
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

void ProtocolGame82::sendUseItemWith(const Position& fromPos, uint16_t fromItemid,
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

void ProtocolGame82::sendUseBattleWindow(const Position& pos, uint16_t itemid,
			uint8_t stackpos, uint32_t creatureid)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x84);
	m_outputMessage.addPosition(pos);
	m_outputMessage.addU16(itemid);
	m_outputMessage.addU8(stackpos);
	m_outputMessage.addU32(creatureid);
}

void ProtocolGame82::sendRotateItem(const Position& pos, uint16_t itemid,
			uint8_t stackpos)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x85);
	m_outputMessage.addPosition(pos);
	m_outputMessage.addU16(itemid);
	m_outputMessage.addU8(stackpos);
}

void ProtocolGame82::sendCloseContainer(uint8_t containerIndex)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x87);
	m_outputMessage.addU8(containerIndex);
}

void ProtocolGame82::sendUpContainer(uint8_t containerIndex)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x88);
	m_outputMessage.addU8(containerIndex);
}

void ProtocolGame82::sendTextWindow(uint32_t windowId, const std::string& text)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x89);
	m_outputMessage.addU32(windowId);
	m_outputMessage.addString(text);
}

void ProtocolGame82::sendHouseWindow(uint32_t windowId, int unk,
			const std::string& text)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x8A);
	m_outputMessage.addU8(unk);
	m_outputMessage.addU32(windowId);
	m_outputMessage.addString(text);
}

void ProtocolGame82::sendLookItem(const Position& pos, uint16_t itemid,
			uint8_t stackpos)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x8C);
	m_outputMessage.addPosition(pos);
	m_outputMessage.addU16(itemid);
	m_outputMessage.addU8(stackpos);
}

void ProtocolGame82::sendSay(SpeakClasses_t type, const std::string text)
{
	PROTOCOLGAME_SEND_FUNCTION;
	char newtype = translateSpeakClassFromInternal(type);
	m_outputMessage.addMessageType(0x96);
	m_outputMessage.addU8(newtype);
	m_outputMessage.addString(text);
}

void ProtocolGame82::sendSay(SpeakClasses_t type, uint16_t channel,
			const std::string text)
{
	PROTOCOLGAME_SEND_FUNCTION;
	char newtype = translateSpeakClassFromInternal(type);
	m_outputMessage.addMessageType(0x96);
	m_outputMessage.addU8(newtype);
	m_outputMessage.addU16(channel);
	m_outputMessage.addString(text);
}

void ProtocolGame82::sendSay(SpeakClasses_t type, const std::string& sendToplayer,
			const std::string text)
{
	PROTOCOLGAME_SEND_FUNCTION;
	char newtype = translateSpeakClassFromInternal(type);
	m_outputMessage.addMessageType(0x96);
	m_outputMessage.addU8(newtype);
	m_outputMessage.addString(sendToplayer);
	m_outputMessage.addString(text);
}

void ProtocolGame82::sendRequestChannels()
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x97);
}

void ProtocolGame82::sendOpenChannel(uint16_t channelid)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x98);
	m_outputMessage.addU16(channelid);
}

void ProtocolGame82::sendCloseChannel(uint16_t channelid)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x99);
	m_outputMessage.addU16(channelid);
}

void ProtocolGame82::sendOpenPrivatePlayerChat(const std::string& playerName)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x9A);
	m_outputMessage.addString(playerName);
}

void ProtocolGame82::sendOpenRuleViolation(const std::string& text)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x9B);
	m_outputMessage.addString(text);
}

void ProtocolGame82::sendCloseRuleViolation(const std::string& text)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x9C);
	m_outputMessage.addString(text);
}

void ProtocolGame82::sendCancelRuleViolation()
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x9D);
}

void ProtocolGame82::sendFightModes(uint8_t attack, uint8_t chase, uint8_t secure)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xA0);
	m_outputMessage.addU8(attack);
	m_outputMessage.addU8(chase);
	m_outputMessage.addU8(secure);
}

void ProtocolGame82::sendAttackCreature(uint32_t creatureid)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xA1);
	m_outputMessage.addU32(creatureid);
	GlobalVariables::setAttackID(creatureid);
}

void ProtocolGame82::sendFollowCreature(uint32_t creatureid)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xA2);
	m_outputMessage.addU32(creatureid);
	GlobalVariables::setFollowID(creatureid);
}

void ProtocolGame82::sendCreatePrivateChatChannel()
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xAA);
}

void ProtocolGame82::sendInvitePrivateChatChannel(const std::string& name)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xAB);
	m_outputMessage.addString(name);
}

void ProtocolGame82::sendExcludePrivateChatChannel(const std::string& name)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xAC);
	m_outputMessage.addString(name);
}

void ProtocolGame82::sendInviteParty(uint32_t playerid)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xA3);
	m_outputMessage.addU32(playerid);
}

void ProtocolGame82::sendJoinParty(uint32_t playerid)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xA4);
	m_outputMessage.addU32(playerid);
}

void ProtocolGame82::sendCancelInviteParty(uint32_t playerid)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xA5);
	m_outputMessage.addU32(playerid);
}

void ProtocolGame82::sendPassPartyLeader(uint32_t playerid)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xA6);
	m_outputMessage.addU32(playerid);
}

void ProtocolGame82::sendLeaveParty()
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xA7);
}

void ProtocolGame82::sendCancelMove()
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xBE);
	GlobalVariables::setAttackID(0);
	GlobalVariables::setFollowID(0);
}

void ProtocolGame82::sendRequestOutfit()
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xD2);
}

void ProtocolGame82::sendSetOutfit(uint16_t looktype, uint8_t head, uint8_t body,
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

void ProtocolGame82::sendAddVip(const std::string& name)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xDC);
	m_outputMessage.addString(name);
}

void ProtocolGame82::sendRemVip(uint32_t playerid)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xDD);
	m_outputMessage.addU32(playerid);
}

void ProtocolGame82::sendBugReport(const std::string& text)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xE6);
	m_outputMessage.addString(text);
}

void ProtocolGame82::sendRequestQuestLog()
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xF0);
}

void ProtocolGame82::sendRequestQuest(uint16_t questid)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xF1);
	m_outputMessage.addU16(questid);
}

//protected
void ProtocolGame82::sendPing()
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x1E);
}

void ProtocolGame82::sendRequestUpdateTile(const Position& pos)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xC9);
	m_outputMessage.addPosition(pos);
}

void ProtocolGame82::sendRequestUpdateContainer(uint8_t containerid)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xCA);
	m_outputMessage.addU8(containerid);
}

void ProtocolGame82::sendCloseShop()
{
    PROTOCOLGAME_SEND_FUNCTION;
    m_outputMessage.addMessageType(0x7C);
}
void ProtocolGame82::sendCloseNPCChannel()
{
    PROTOCOLGAME_SEND_FUNCTION;
    m_outputMessage.addMessageType(0x9E);
}
///////////////////////////
char ProtocolGame82::translateSpeakClassFromInternal(SpeakClasses_t s){
    switch(s){
        case SPEAK_SAY:
            return 0x01;
        case SPEAK_WHISPER:
            return 0x02;
        case SPEAK_YELL:
            return 0x03;
        case SPEAK_PRIVATE_PN:
            return 0x04;
        case SPEAK_PRIVATE_NP:
            return 0x05;
        case SPEAK_PRIVATE:
            return 0x06;
        case SPEAK_CHANNEL_Y:
            return 0x07;
        /*
        case SPEAK_RVR_CHANNEL:
            return 0x08; // reporting rule violation - ctrl+r
        case SPEAK_RVR_ANSWER:
            return 0x09; // answering report
        case SPEAK_RVR_CONTINUE:
            return 0x0A; // answering the answer of report
        */
        case SPEAK_BROADCAST:
            return 0x0B;
        case SPEAK_CHANNEL_R1:
            return 0x0C;
        case SPEAK_PRIVATE_RED:
            return 0x0D;
        case SPEAK_CHANNEL_O:
            return 0x0E;
        case SPEAK_CHANNEL_R2:
            return 0x10;
        // 0x11
        case SPEAK_MONSTER_SAY:
            return 0x12;
        case SPEAK_MONSTER_YELL:
            return 0x13;

        default: // UNK6, UNK7, UNK8 -- perhaps these are reports? those that we commented out above?
            RAISE_PROTOCOL_WARNING("speakclass translatefrominternal failed");
    }
    //RAISE_PROTOCOL_ERROR("speakclass translatetoint error");
}

SpeakClasses_t ProtocolGame82::translateSpeakClassToInternal(char s){
    switch(s){
        case 0x01:
            return SPEAK_SAY;
        case 0x02:
            return SPEAK_WHISPER;
        case 0x03:
            return SPEAK_YELL;
        case 0x04:
            return SPEAK_PRIVATE_PN;
        case 0x05:
            return SPEAK_PRIVATE_NP;
        case 0x06:
            return SPEAK_PRIVATE;
        case 0x07:
            return SPEAK_CHANNEL_Y;
        /*
        case SPEAK_RVR_CHANNEL:
            return 0x08; // reporting rule violation - ctrl+r
        case SPEAK_RVR_ANSWER:
            return 0x09; // answering report
        case SPEAK_RVR_CONTINUE:
            return 0x0A; // answering the answer of report
        */
        case 0x0B:
            return SPEAK_BROADCAST;
        case 0x0C:
            return SPEAK_CHANNEL_R1;
        case 0x0D:
            return SPEAK_PRIVATE_RED;
        case 0x0E:
            return SPEAK_CHANNEL_O;
        case 0x10:
            return SPEAK_CHANNEL_R2;
        // 0x11
        case 0x12:
            return SPEAK_MONSTER_SAY;
        case 0x13:
            return SPEAK_MONSTER_YELL;
        default:
            printf("speakclass translatetointernal failed\n");
            return SPEAK_SAY;

    }
    //RAISE_PROTOCOL_ERROR("speakclass translatefromint error");
}

MessageType_t ProtocolGame82::translateTextMessageToInternal(uint8_t messageType)
{
	MessageType_t nmessageType;
	switch(messageType){
		case 0x11:
			nmessageType = MSG_STATUS_CONSOLE_RED;
			break;
		case 0x12:
			nmessageType = MSG_EVENT_DEFAULT; // broadcast from raid script
			break;
		case 0x13:
			nmessageType = MSG_STATUS_CONSOLE_ORNG;
			break;
		case 0x14:
			nmessageType = MSG_STATUS_WARNING;
			break;
		case 0x15:
			nmessageType = MSG_EVENT_ADVANCE;
			break;
		case 0x16:
			nmessageType = MSG_EVENT_DEFAULT;
			break;
		case 0x17:
			nmessageType = MSG_STATUS_DEFAULT;
			break;
		case 0x18:
			nmessageType = MSG_INFO_DESCR;
			break;
		case 0x19:
			nmessageType = MSG_STATUS_SMALL;
			break;
		case 0x1A:
			nmessageType = MSG_STATUS_CONSOLE_BLUE;
			break;
			
		default:
			/*RAISE_PROTOCOL_ERROR(*/
			printf("text message - 8.2 translation problem\n");
			nmessageType = MSG_INFO_DESCR;
	}
	return nmessageType;
}