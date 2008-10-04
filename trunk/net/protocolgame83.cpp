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
#include "protocolgame83.h"

#include "../objects.h"
#include "../debugprint.h"

#include "../gamecontent/globalvars.h"
#include "../gamecontent/map.h"
#include "../gamecontent/item.h"
#include "../gamecontent/creature.h"
#include "../gamecontent/container.h"
#include "../gamecontent/inventory.h"

ProtocolGame83::ProtocolGame83(const std::string& accountname, const std::string& password, const std::string& name, bool isGM) :
ProtocolGame(accountname, password, name, isGM)
{
	m_skipTiles = 0;

}

void ProtocolGame83::checkVersion()
{
    ASSERT(ProtocolConfig::getInstance().getClientVersion() == CLIENT_VERSION_820);
}

ProtocolGame83::~ProtocolGame83()
{
	//
}

bool ProtocolGame83::parsePacket(uint8_t cmd, NetworkMessage& msg)
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

bool ProtocolGame83::onRecv(NetworkMessage& msg)
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
			RAISE_PROTOCOL_ERROR("Unknown opcode");
	}
	return true;
}

Thing* ProtocolGame83::internalGetThing(NetworkMessage& msg)
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

Item* ProtocolGame83::internalGetItem(NetworkMessage& msg, uint32_t itemid)
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

bool ProtocolGame83::internalSetOutfit(NetworkMessage& msg, Outfit_t& outfit)
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

///////////////////////////
char ProtocolGame83::translateSpeakClassFromInternal(SpeakClasses_t s){
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

SpeakClasses_t ProtocolGame83::translateSpeakClassToInternal(char s){
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

MessageType_t ProtocolGame83::translateTextMessageToInternal(uint8_t messageType)
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