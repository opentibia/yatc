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
		DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Received packet 0x%02x\n", cmd);
		// TODO (nfries88): move switch statement to a virtual function for simpler expansion
		switch(cmd){
		case 0x0A: //Self appear
		{
			MSG_READ_U32(ourID);
			MSG_READ_U8(unk32);
			MSG_READ_U8(unk);
			MSG_READ_U8(canReportBugs);

			//Reset everything. Todo notify gui?
			Containers::getInstance().clear();
			Creatures::getInstance().clear();
			GlobalVariables::clear();
			Inventory::getInstance().clear();
			Map::getInstance().clear();
			//TODO. reset chats

			GlobalVariables::setPlayerID(ourID);
			GlobalVariables::setCanReportBugs(canReportBugs);
			Notifications::onEnterGame();
			break;
		}
		case 0x0B: //GM actions
		{
			for(uint32_t i = 0; i < 32; ++i){
				MSG_READ_U8(GMByte);
				GlobalVariables::setGMAction(i, GMByte);
			}
			break;
		}
		case 0x14: //Error message
		{
			MSG_READ_STRING(errorMessage);
			Notifications::openMessageWindow(MESSAGE_ERROR, errorMessage);
			break;
		}
		case 0x15: //For your information message
		{
			MSG_READ_STRING(infoMessage);
			Notifications::openMessageWindow(MESSAGE_INFORMATION, infoMessage);
			break;
		}
            case 0x16: //Waiting list
		{
			MSG_READ_STRING(waitMessage);
			MSG_READ_U8(waitTime);
			Notifications::openWaitingList(waitMessage, waitTime);
			break;
		}
		case 0x1E: //ping
		{
			//no data
			sendPing();
			break;
		}
		case 0x28: //Relogin window
		{
			//no data
			Notifications::openRelogin();
			break;
		}
		case 0x64: //Map description
		{
			MSG_READ_POSITION(currentPos);
			GlobalVariables::getPlayerPosition() = currentPos;
			if(!setMapDescription(msg, currentPos.x - 8, currentPos.y - 6, currentPos.z, 18, 14)){
				RAISE_PROTOCOL_ERROR("Map description 0x64");
			}
			break;
		}
		case 0x65: //move north
		{
			Position& myPos = GlobalVariables::getPlayerPosition();
			myPos.y--;
			if(!setMapDescription(msg, myPos.x - 8, myPos.y - 6, myPos.z, 18, 1)){
				RAISE_PROTOCOL_ERROR("Map description 0x65");
			}
			Notifications::onWalk();
			break;
		}
		case 0x66: //move east
		{
			Position& myPos = GlobalVariables::getPlayerPosition();
			myPos.x++;
			if(!setMapDescription(msg, myPos.x + 9, myPos.y - 6, myPos.z, 1, 14)){
				RAISE_PROTOCOL_ERROR("Map description 0x66");
			}
			Notifications::onWalk();
			break;
		}
		case 0x67: //move south
		{
			Position& myPos = GlobalVariables::getPlayerPosition();
			myPos.y++;
			if(!setMapDescription(msg, myPos.x - 8, myPos.y + 7, myPos.z, 18, 1)){
				RAISE_PROTOCOL_ERROR("Map description 0x67");
			}
			Notifications::onWalk();
			break;
		}
		case 0x68: //move west
		{
			Position& myPos = GlobalVariables::getPlayerPosition();
			myPos.x--;
			if(!setMapDescription(msg, myPos.x - 8, myPos.y - 6, myPos.z, 1, 14)){
				RAISE_PROTOCOL_ERROR("Map description 0x68");
			}
			Notifications::onWalk();
			break;
		}
		case 0x69: //update tile
		{
			MSG_READ_POSITION(tilePos);

			MSG_INSPECT_U16(thingId);

			if(thingId == 0xFF01){
				MSG_READ_U16(FF01);
				Tile* tile = Map::getInstance().getTile(tilePos);
				if(!tile){
					RAISE_PROTOCOL_ERROR("Tile Update - !tile");
				}
				tile->clear();
			}
			else{
				if(!setTileDescription(msg, tilePos)){
					RAISE_PROTOCOL_ERROR("Tile Update - SetTileDescription");
				}
				MSG_READ_U16(FF00);
			}
			break;
		}
		case 0x6A: //tile - add thing
		{
			MSG_READ_POSITION(tilePos);
			Thing* thing = internalGetThing(msg);
			if(!thing){
				RAISE_PROTOCOL_ERROR("Tile Add - getThing");
			}

			Tile* tile = Map::getInstance().getTile(tilePos);
			if(!tile){
				RAISE_PROTOCOL_ERROR("Tile Add - !tile");
			}

			if(!tile->addThing(thing, true)){
				RAISE_PROTOCOL_ERROR("Tile Add - addThing");
			}

			break;
		}
		case 0x6B: //tile transform thing or creature turn
		{
			MSG_READ_POSITION(tilePos);
			MSG_READ_U8(stackPos);

			MSG_READ_U16(thingId);

			if(thingId == 0x0061 || thingId == 0x0062 || thingId == 0x0063){
				//creature turn
				MSG_READ_U32(creatureID);
				MSG_READ_U8(direction);

				Creature* creature = Creatures::getInstance().getCreature(creatureID);
				if(!creature){
					RAISE_PROTOCOL_ERROR("Tile Transform - !creature");
				}
				if(direction > 3){
					RAISE_PROTOCOL_ERROR("Tile Transform - direction > 3");
				}
				creature->setTurnDir((Direction)direction);
			}
			else{
				//get tile
				Tile* tile = Map::getInstance().getTile(tilePos);
				if(!tile){
					RAISE_PROTOCOL_ERROR("Tile Transform - !tile");
				}
				//removes the old item
				Thing* thing = tile->getThingByStackPos(stackPos);
				if(!thing){
					RAISE_PROTOCOL_ERROR("Tile Transform - !thing");
				}
				if(!tile->removeThing(thing)){
					RAISE_PROTOCOL_ERROR("Tile Transform - removeThing");
				}
				Thing::deleteThing(thing);

				//adds a new item
				Item* item = internalGetItem(msg, thingId);
				if(!tile->insertThing(item, (int32_t)stackPos)){
					RAISE_PROTOCOL_ERROR("Tile Transform - insertThing");
				}
			}
			break;
		}
		case 0x6C: //tile - remove thing
		{
			MSG_READ_POSITION(tilePos);
			MSG_READ_U8(stackPos);

			//get tile
			Tile* tile = Map::getInstance().getTile(tilePos);
			if(!tile){
				RAISE_PROTOCOL_ERROR("Tile Remove - !tile");
			}

			//remove thing
			Thing* thing = tile->getThingByStackPos(stackPos);
			if(!thing){
				//RAISE_PROTOCOL_WARNING("Tile Remove - !thing");
				//TODO. send update tile
				return true;
			}
			if(!tile->removeThing(thing)){
				RAISE_PROTOCOL_ERROR("Tile Remove - removeThing");
			}
			Thing::deleteThing(thing);
			break;
		}
		case 0x6D: //creature move
		{
			MSG_READ_POSITION(oldPos);
			MSG_READ_U8(oldStackPos);
			MSG_READ_POSITION(newPos);

			if(oldStackPos > 9){
				RAISE_PROTOCOL_ERROR("Creature move - oldStackpos");
			}
			Tile* tile = Map::getInstance().getTile(oldPos);
			if(!tile){
				RAISE_PROTOCOL_ERROR("Creature move - !tile old");
			}

			Thing* thing = tile->getThingByStackPos(oldStackPos);
			if(!thing){
				//RAISE_PROTOCOL_WARNING("Creature move - !thing");
				//TODO. Notify GUI
				//TODO. send update tile
				return true;
			}
			Creature* creature = thing->getCreature();
			if(!creature){
				RAISE_PROTOCOL_ERROR("Creature move - !creature");
			}

			if(!tile->removeThing(creature)){
				RAISE_PROTOCOL_ERROR("Creature move - removeThing");
			}

			tile = Map::getInstance().getTile(newPos);
			if(!tile){
				RAISE_PROTOCOL_ERROR("Creature move - !tile new");
			}
			if(!tile->addThing(creature, true)){
				RAISE_PROTOCOL_ERROR("Creature move - addThing");
			}

			Notifications::onCreatureMove(creature->getId());

			creature->setMoving(oldPos);

			//update creature direction
			if(oldPos.x > newPos.x){
				creature->setLookDir(DIRECTION_WEST); creature->setTurnDir(DIRECTION_WEST);
			}
			else if(oldPos.x < newPos.x){
				creature->setLookDir(DIRECTION_EAST); creature->setTurnDir(DIRECTION_EAST);
			}
			else if(oldPos.y > newPos.y){
				creature->setLookDir(DIRECTION_NORTH); creature->setTurnDir(DIRECTION_NORTH);
			}
			else if(oldPos.y < newPos.y){
				creature->setLookDir(DIRECTION_SOUTH); creature->setTurnDir(DIRECTION_SOUTH);
			}

			break;
		}
		case 0x6E: //open container
		{
			MSG_READ_U8(cid);
			MSG_READ_U16(itemid);
			MSG_READ_STRING(name);
			MSG_READ_U8(capacity);
			MSG_READ_U8(hasParent);
			MSG_READ_U8(size);

			if(size > capacity){
				RAISE_PROTOCOL_ERROR("Open container - size > cap");
			}

			Container* container = Containers::getInstance().createContainer(cid);
			if(!container){
				RAISE_PROTOCOL_ERROR("Open container - !container");
			}
			container->setName(name);
			container->setItemId(itemid);
			container->setCapacity(capacity);
			container->setHasParent(hasParent == 1);

			for(uint32_t i = 0; i < size; ++i){
				Item* item = internalGetItem(msg, (uint32_t)-1);
				if(!item){
					RAISE_PROTOCOL_ERROR("Container Open - !item");
				}
				if(!container->addItem(item)){
					RAISE_PROTOCOL_ERROR("Container Open - addItem");
				}
			}

			Notifications::openContainer(cid);
			break;
		}
		case 0x6F: //close container
		{
			MSG_READ_U8(cid);
			Containers::getInstance().deleteContainer(cid);
			Notifications::closeContainer(cid);
			break;
		}
		case 0x70: //container - add item
		{
			MSG_READ_U8(cid);
			Item* item = internalGetItem(msg, (uint32_t)-1);
			if(!item){
				RAISE_PROTOCOL_ERROR("Container add - !item");
			}

			Container* container = Containers::getInstance().getContainer(cid);
			if(!container){
				//RAISE_PROTOCOL_ERROR("Container add - !container");
				break;
			}
			if(!container->addItem(item)){
				RAISE_PROTOCOL_ERROR("Container add - addItem");
			}
			break;
		}
		case 0x71: //container - update item
		{
			MSG_READ_U8(cid);
			MSG_READ_U8(slot);
			Item* item = internalGetItem(msg, (uint32_t)-1);
			if(!item){
				RAISE_PROTOCOL_ERROR("Container update - !item");
			}

			Container* container = Containers::getInstance().getContainer(cid);
			if(!container){
				//RAISE_PROTOCOL_ERROR("Container update - !container");
				break;
			}
			if(!container->updateItem(slot, item)){
				RAISE_PROTOCOL_ERROR("Container update - updateItem");
			}
			break;
		}
		case 0x72: //container - remove item
		{
			MSG_READ_U8(cid);
			MSG_READ_U8(slot);

			Container* container = Containers::getInstance().getContainer(cid);
			if(!container){
				//RAISE_PROTOCOL_ERROR("Container remove - !container");
				break;
			}
			if(!container->removeItem(slot)){
				RAISE_PROTOCOL_ERROR("Container remove - removeItem");
			}
			//TODO. send update container
			break;
		}
		case 0x78: //inventory set slot item
		{
			MSG_READ_U8(slot);
			Item* item = internalGetItem(msg, (uint32_t)-1);
			if(!item){
				RAISE_PROTOCOL_ERROR("Inventory set - !item");
			}
			if(!Inventory::getInstance().addItem(slot, item)){
				RAISE_PROTOCOL_ERROR("Inventory set - addItem");
			}
			break;
		}
		case 0x79: //inventory reset slot
		{
			MSG_READ_U8(slot);
			if(!Inventory::getInstance().removeItem(slot)){
				RAISE_PROTOCOL_ERROR("Inventory reset - removeItem");
			}
			break;
		}
		case 0x7D: //safe-trade request - ack
		case 0x7E: //safe-trade request - no-ack
		{
			MSG_READ_STRING(name);
			MSG_READ_U8(count);

			Container* container = Containers::getInstance().newTradeContainer();
			if(!container){
				RAISE_PROTOCOL_ERROR("Trade open - !container");
			}
			container->setName(name);
			container->setCapacity(count);

			for(uint32_t i = 0; i < count; ++i) {

				Item* item = internalGetItem(msg, (uint32_t)-1);
				if(!item){
					RAISE_PROTOCOL_ERROR("Trade open - !item");
				}

				if(!container->addItem(item)){
					RAISE_PROTOCOL_ERROR("Container add - addItem");
				}
			}

			Notifications::openTradeWindow(cmd == 0x7D);
			break;
		}
		case 0x7F: //close trade
		{
			//no data
			Containers::getInstance().closeTradeContainer();
			Notifications::closeTradeWindow();
			break;
		}

		case 0x82: //gameworld light-settings
		{
			MSG_READ_U8(level);
			MSG_READ_U8(color);
			GlobalVariables::setWorldLightLevel(level);
			GlobalVariables::setWorldLightColor(color);
			break;
		}
		case 0x83: //magic effect
		{
			MSG_READ_POSITION(effectPos);
			MSG_READ_U8(effect);
			if(effect == 0 || effect > 35){
				RAISE_PROTOCOL_ERROR("Magic effect - out of range");
			}
			Tile* tile = Map::getInstance().getTile(effectPos);
			if(!tile){
				RAISE_PROTOCOL_ERROR("Magic effect - !tile");
			}
			tile->addEffect(effect);
			break;
		}
		case 0x84: //animated text
		{
			MSG_READ_POSITION(textPos);
			MSG_READ_U8(color);
			MSG_READ_STRING(text);
			Map::getInstance().addAnimatedText(textPos, color, text);
			break;
		}
		case 0x85: //distance shoot
		{
			MSG_READ_POSITION(fromPos);
			MSG_READ_POSITION(toPos);
			MSG_READ_U8(effect);
			if(effect == 0 || effect > 28){
				RAISE_PROTOCOL_ERROR("Distance shoot - out of range");
			}
			Map::getInstance().addDistanceEffect(fromPos, toPos, effect);
			break;
		}
		case 0x86: //creature - square
		{
			MSG_READ_U32(creatureID);
			MSG_READ_U8(color);
			Creature* creature = Creatures::getInstance().getCreature(creatureID);
			if(creature){
				creature->setSquare(color);
			}
			break;
		}
		case 0x8C: //creature - health
		{
			MSG_READ_U32(creatureID);
			MSG_READ_U8(percent);
			Creature* creature = Creatures::getInstance().getCreature(creatureID);
			if(creature){
				if(percent > 100){
					RAISE_PROTOCOL_ERROR("Creature health - percent > 100");
				}
				creature->setHealth(percent);
			}
			break;
		}
		case 0x8D: //creature - light level
		{
			MSG_READ_U32(creatureID);
			MSG_READ_U8(level);
			MSG_READ_U8(color);
			Creature* creature = Creatures::getInstance().getCreature(creatureID);
			if(creature){
				creature->setLightLevel(level);
				creature->setLightColor(color);
			}
			break;
		}
		case 0x8E: //creature - outfit
		{
			MSG_READ_U32(creatureID);
			Creature* creature = Creatures::getInstance().getCreature(creatureID);
			if(creature){
				if(!internalSetOutfit(msg, creature->getOutfit())){
					RAISE_PROTOCOL_ERROR("Creature outfit - outfit error");
				}
			}
			else{
				Outfit_t dummy;
				if(!internalSetOutfit(msg, dummy)){
					RAISE_PROTOCOL_ERROR("Creature outfit - outfit error2");
				}
			}
			break;
		}
		case 0x8F: //creature - change speed
		{
			MSG_READ_U32(creatureID);
			MSG_READ_U16(speed);
			Creature* creature = Creatures::getInstance().getCreature(creatureID);
			if(creature){
				creature->setSpeed(speed);
			}
			break;
		}
		case 0x90: //creature - skulls
		{
			MSG_READ_U32(creatureID);
			MSG_READ_U8(skull);
			Creature* creature = Creatures::getInstance().getCreature(creatureID);
			if(creature){
				creature->setSkull(skull);
			}
			break;
		}
		case 0x91: //creature - shields
		{
			MSG_READ_U32(creatureID);
			MSG_READ_U8(shield);
			Creature* creature = Creatures::getInstance().getCreature(creatureID);
			if(creature){
				creature->setShield(shield);
			}
			break;
		}
		case 0x96: //text window
		{
			MSG_READ_U32(windowID);
			MSG_READ_U16(itemID);
			MSG_READ_U16(maxlen);
			MSG_READ_STRING(text);
			MSG_READ_STRING(writter);
			MSG_READ_STRING(date);
			Notifications::openItemText(windowID, itemID, maxlen, text, writter, date);
			break;
		}
		case 0x97: //house text window
		{
			MSG_READ_U8(unk);
			MSG_READ_U32(windowID);
			MSG_READ_STRING(text);
			Notifications::openHouseText(windowID, unk, text);
			break;
		}
		case 0xA0: //player - stats
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
			if(health > healthMax || levelPercent > 100 || mana > manaMax ||
				magicLevelPercent > 100){
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
			break;
		}
		case 0xA1: //player - skills
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
			GlobalVariables::setPlayerSkill(SKILL_FISH, SKILL_ATTR_LEVEL, fish);
			GlobalVariables::setPlayerSkill(SKILL_FISH, SKILL_ATTR_PERCENT, fishPercent);

			Notifications::onChangeSkills();
			break;
		}
		case 0xA2: //player - icons
		{
			MSG_READ_U16(icons);
			GlobalVariables::setPlayerIcons(icons);
			Notifications::onChangeIcons();
			break;
		}
		case 0xA3: //player cancel attack
		{
			//no data
			Notifications::onCancelAttack();
			break;
		}
		case 0xAA: //creature speak
		{
			MSG_READ_U32(unkSpeak);
			MSG_READ_STRING(name);
			MSG_READ_U16(level);
			MSG_READ_U8(type);
			switch(type){
			case SPEAK_SAY:
			case SPEAK_WHISPER:
			case SPEAK_YELL:
			case SPEAK_MONSTER_SAY:
			case SPEAK_MONSTER_YELL:
			{
				MSG_READ_POSITION(creaturePos);
				MSG_READ_STRING(text);
				Notifications::onCreatureSpeak((SpeakClasses_t)type, unkSpeak, name, level, creaturePos, text);
				break;
			}
			case SPEAK_CHANNEL_Y:
			case SPEAK_CHANNEL_R1:
			case SPEAK_CHANNEL_R2:
			case SPEAK_CHANNEL_O:
			{
				MSG_READ_U16(channelID);
				MSG_READ_STRING(text);
				Notifications::onCreatureSpeak((SpeakClasses_t)type, unkSpeak, name, level, channelID, text);
				break;
			}
			case SPEAK_PRIVATE:
			case SPEAK_BROADCAST:
			case SPEAK_PRIVATE_RED:
			{
				MSG_READ_STRING(text);
				Notifications::onCreatureSpeak((SpeakClasses_t)type, unkSpeak, name, level, text);
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
			break;
		}
		case 0xAB: //list channels
		{
			MSG_READ_U8(count);
			std::list<ChannelList_t> list;
			for(uint32_t i = 0; i < count; ++i){
				ChannelList_t channel;
				MSG_READ_U16(channelID);
				MSG_READ_STRING(name);
				channel.id = channelID;
				channel.name = name;
				list.push_back(channel);
			}
			Notifications::openChannelList(list);
			break;
		}
		case 0xAC: //open channel
		{
			MSG_READ_U16(channelID);
			MSG_READ_STRING(name);
			Notifications::openChannel(channelID, name);
			break;
		}
		case 0xAD: //open private player chat
		{
			MSG_READ_STRING(name);
			Notifications::openPrivatePlayerChat(name);
			break;
		}
		case 0xAE: //rule violation - open
		{
			//TODO
			MSG_READ_U16(a);
			break;
		}
		case 0xAF: //rule violation - ?
		{
			//TODO
			MSG_READ_STRING(a);
			break;
		}
		case 0xB0: //rule violation - ?
		{
			//TODO
			MSG_READ_STRING(a);
			break;
		}
		case 0xB1: //rule violation - ?
		{
			//TODO
			break;
		}
		case 0xB2: //create private chat channel
		{
			MSG_READ_U16(channelID);
			MSG_READ_STRING(name);
			Notifications::openPrivateChannel(channelID, name);
			break;
		}
		case 0xB3: //close private channel
		{
			MSG_READ_U16(channelID);
			Notifications::closePrivateChannel(channelID);
			break;
		}
		case 0xB4: //text message
		{
			MSG_READ_U8(messageType);
			MSG_READ_STRING(text);
			if(messageType < 0x11 || messageType > 0x19){
				RAISE_PROTOCOL_ERROR("text message - type");
			}
			Notifications::onTextMessage((MessageType_t)messageType, text);
			break;
		}
		case 0xB5: //player cancel walk
		{
			MSG_READ_U8(direction);
			if(direction > 3){
				RAISE_PROTOCOL_ERROR("cancel walk - direction > 3");
			}
			Notifications::onCancelWalk((Direction)direction);
			break;
		}
		case 0xBE: //floor change up
		{
			Position& myPos = GlobalVariables::getPlayerPosition();
			myPos.z--;
			//going to surface
			if(myPos.z == 7){
				//floor 7 and 6 already set
				for(int32_t i = 5; i >= 0; i--){
					if(!setFloorDescription(msg, myPos.x - 8, myPos.y - 6, i, 18, 14, 8 - i)){
						RAISE_PROTOCOL_ERROR("Set Floor Desc z = 7 0xBE");
					}
				}
			}
			//underground, going one floor up (still underground)
			else if(myPos.z > 7){
				if(!setFloorDescription(msg, myPos.x - 8, myPos.y - 6, myPos.z - 2, 18, 14, 3)){
					RAISE_PROTOCOL_ERROR("Set Floor Desc  z > 7 0xBE");
				}
			}
			myPos.x++;
			myPos.y++;
			break;
		}
		case 0xBF: //floor change down
		{
			Position& myPos = GlobalVariables::getPlayerPosition();
			myPos.z++;
			//going from surface to underground
			if(myPos.z == 8){
				int32_t j, i;
				for(i = myPos.z, j = -1; i < (int32_t)myPos.z + 3; ++i, --j){
					if(!setFloorDescription(msg, myPos.x - 8, myPos.y - 6, i, 18, 14, j)){
						RAISE_PROTOCOL_ERROR("Set Floor Desc  z = 8 0xBF");
					}
				}
			}
			//going further down
			else if(myPos.z > 8 && myPos.z < 14) {
				if(!setFloorDescription(msg, myPos.x - 8, myPos.y - 6, myPos.z + 2, 18, 14, -3)){
					RAISE_PROTOCOL_ERROR("Set Floor Desc  z > 8 && z < 14 0xBF");
				}
			}
			myPos.x--;
			myPos.y--;
			break;
		}
		case 0xC8: //outfit window
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
			Notifications::openOutfit(outfit, list);
			break;
		}
		case 0xD2: //vip - state
		{
			MSG_READ_U32(creatureID);
			MSG_READ_STRING(name);
			MSG_READ_U8(online);
			Notifications::onVipState(creatureID, name, online);
			break;
		}
		case 0xD3: //vip - login
		{
			MSG_READ_U32(creatureID);
			Notifications::onVipLogin(creatureID);
			break;
		}
		case 0xD4: //vip - logout
		{
			MSG_READ_U32(creatureID);
			Notifications::onVipLogout(creatureID);
			break;
		}
		case 0xF0: //quest list
		{
			//TODO.GUI
			MSG_READ_U16(nQuests);
			for(uint32_t i = 0; i < nQuests; ++i){
				MSG_READ_U16(questsID);
				MSG_READ_STRING(questsName);
				MSG_READ_U8(questsState);
			}
			break;
		}
		case 0xF1: //quest list
		{
			//TODO.GUI
			MSG_READ_U16(questsID);
			MSG_READ_U8(nMission);
			for(uint32_t i = 0; i < nMission; ++i){
				MSG_READ_STRING(questsName);
				MSG_READ_STRING(questsDesc);
			}
			break;
		}
		default:
			RAISE_PROTOCOL_ERROR("Unknown opcode");
			break;
		}
	}
	return true;
}

bool ProtocolGame80::setMapDescription(NetworkMessage& msg, int x, int y, int z, int width, int height)
{
	int startz, endz, zstep;
	//calculate map limits
	if(z > 7){
		startz = z - 2;
		endz = std::min(MAP_LAYER - 1, z + 2);
		zstep = 1;
	}
	else{
		startz = 7;
		endz = 0;
		zstep = -1;
	}

	for(int nz = startz; nz != endz + zstep; nz += zstep){
		//pare each floor
		if(!setFloorDescription(msg, x, y, nz, width, height, z - nz)){
			return false;
		}
	}
	return true;
}

bool ProtocolGame80::setFloorDescription(NetworkMessage& msg, int x, int y, int z, int width, int height, int offset)
{
	for(int nx = 0; nx < width; nx++){
		for(int ny = 0; ny < height; ny++){
			if(m_skipTiles == 0){
				MSG_INSPECT_U16(tileOpt);
				//Decide if we have to skip tiles
				// or if it is a real tile
				if(tileOpt >= 0xFF00){
					MSG_READ_U16(skipTiles);
					m_skipTiles = (skipTiles & 0xFF);
				}
				else{
					//real tile so read tile
					Position pos(x + nx + offset, y + ny + offset, z);
					if(!setTileDescription(msg, pos)){
						return false;
					}
					//read skip tiles info
					MSG_READ_U16(skipTiles);
					m_skipTiles = (skipTiles & 0xFF);
				}
			}
			//skipping tiles...
			else{
				m_skipTiles--;
			}
		}
	}
	return true;
}

bool ProtocolGame80::setTileDescription(NetworkMessage& msg, const Position& pos)
{
	//set the tile in the map
	Tile* tile = Map::getInstance().setTile(pos);
	if(!tile){
		return false;
	}
	//and clear it
	tile->clear();

	int n = 0;
	while(1){
		//avoid infinite loop
		n++;

		MSG_INSPECT_U16(inspectTileId);
		if(inspectTileId >= 0xFF00){
			//end of the tile
			return true;
		}
		else{
			if(n > 10){
				return false;
			}
			//read tile things: items and creatures
			Thing* thing = internalGetThing(msg);
			if(!thing){
				return false;
			}
			//and add to the tile
			if(!tile->addThing(thing)) {
				return false;
			}
		}
	}
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

void ProtocolGame80::sendPassParyLeader(uint32_t playerid)
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
