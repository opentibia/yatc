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

#include <stdlib.h> // srand, rand
#include "../fassert.h"
#include "protocolgame.h"
#include "rsa.h"

#include "../notifications.h"

#include "../gamecontent/globalvars.h"
#include "../gamecontent/map.h"
#include "../gamecontent/item.h"
#include "../gamecontent/creature.h"
#include "../gamecontent/container.h"
#include "../gamecontent/inventory.h"
#include "../gamecontent/shop.h" // 8.2+

ProtocolGame::ProtocolGame(const std::string& accountname, const std::string& password, const std::string& name, bool isGM) :
m_outputMessage(NetworkMessage::CAN_WRITE)
{
	m_password = password;
	m_name = name;
	m_accountname = accountname;
	m_isGM = isGM;
	m_usesaccountname = false;
	m_account = atoi(m_accountname.c_str());
}

ProtocolGame::~ProtocolGame()
{
	//
}

void ProtocolGame::onConnect()
{
	printf("Connected!\n");
	ProtocolConfig& config = ProtocolConfig::getInstance();
	NetworkMessage output(NetworkMessage::CAN_WRITE);
	output.addU8(0x0A); //Game world Protocol
	output.addU16(config.getOS());
	output.addU16(config.getVersionOverride() ? config.getVersionOverride() : config.getClientVersion());

	int sizeBefore = output.getSize();

	//From here bytes are encrypted using RSA
	//generate XTEA key
	uint32_t k[4];
	srand(1); //TODO use a real seed
	k[0] = (rand() << 16) | rand(); k[1] = (rand() << 16) | rand();
	k[2] = (rand() << 16) | rand(); k[3] = (rand() << 16) | rand();

	output.addU8(0); // first byte have to be 0
	output.addU32(k[0]); //XTEA key
	output.addU32(k[1]);
	output.addU32(k[2]);
	output.addU32(k[3]);

	output.addU8(m_isGM);
	if(!m_usesaccountname)
		output.addU32(m_account);
	else {
		output.addString(m_accountname);
	}
	output.addString(m_name);
	output.addString(m_password);

	//RSA size has to be 128
	int rsaSize = output.getSize() - sizeBefore;
	output.addPaddingBytes(128 - rsaSize);

	char* rsaBuffer = output.getBuffer() + sizeBefore;
	RSA::getInstance()->encrypt(rsaBuffer, 128);

//	bool oldChecksumState = m_connection->getChecksumState();
//	m_connection->setChecksumState(false);

	m_connection->sendMessage(output);
	m_connection->setKey((char*)k, 4*sizeof(uint32_t));
	m_connection->setCryptoState(true);
//	m_connection->setChecksumState(oldChecksumState);

	m_account = 0;
	m_password = "";
	m_name = "";
	m_isGM = 0;
	checkVersion();
}



// implementations of default protocol parse functions
bool ProtocolGame::parsePacket(uint8_t cmd, NetworkMessage& msg)
{
    switch(cmd){
    case 0x0A:
        return parseSelfAppear(msg);
    case 0x0B:
        return parseGMActions(msg);
    case 0x14:
        return parseErrorMessage(msg);
    case 0x15:
        return parseFYIMessage(msg);
    case 0x16:
        return parseWaitingList(msg);
    case 0x1E:
        return parsePing(msg);
    case 0x28:
        return parseDeath(msg);
    case 0x32:
        return parseCanReportBugs(msg);
    case 0x64:
        return parseMapDescription(msg);
    case 0x65:
        return parseMoveNorth(msg);
    case 0x66:
        return parseMoveEast(msg);
    case 0x67:
        return parseMoveSouth(msg);
    case 0x68:
        return parseMoveWest(msg);
    case 0x69:
        return parseUpdateTile(msg);
    case 0x6A:
        return parseTileAddThing(msg);
    case 0x6B:
        return parseTileTransformThing(msg);
    case 0x6C:
        return parseTileRemoveThing(msg);
    case 0x6D:
        return parseCreatureMove(msg);
    case 0x6E:
        return parseOpenContainer(msg);
    case 0x6F:
        return parseCloseContainer(msg);
    case 0x70:
        return parseContainerAddItem(msg);
    case 0x71:
        return parseContainerUpdateItem(msg);
    case 0x72:
        return parseContainerRemoveItem(msg);
    case 0x78:
        return parseInventorySetSlot(msg);
    case 0x79:
        return parseInventoryResetSlot(msg);
    case 0x7D:
        return parseSafeTradeRequestAck(msg);
    case 0x7E:
        return parseSafeTradeRequestNoAck(msg);
    case 0x7F:
        return parseSafeTradeClose(msg);
    case 0x82:
        return parseWorldLight(msg);
    case 0x83:
        return parseMagicEffect(msg);
    case 0x84:
        return parseAnimatedText(msg);
    case 0x85:
        return parseDistanceShot(msg);
    case 0x86:
        return parseCreatureSquare(msg);
    case 0x8C:
        return parseCreatureHealth(msg);
    case 0x8D:
        return parseCreatureLight(msg);
    case 0x8E:
        return parseCreatureOutfit(msg);
    case 0x8F:
        return parseCreatureSpeed(msg);
    case 0x90:
        return parseCreatureSkulls(msg);
    case 0x91:
        return parseCreatureShields(msg);
	case 0x96:
		return parseItemTextWindow(msg);
	case 0x97:
		return parseHouseTextWindow(msg);
	case 0xA0:
		return parsePlayerStats(msg);
	case 0xA1:
		return parsePlayerSkills(msg);
	case 0xA2:
		return parsePlayerIcons(msg);
	case 0xA3:
		return parsePlayerCancelAttack(msg);
	case 0xAA:
		return parseCreatureSpeak(msg);
	case 0xAB:
		return parseChannelList(msg);
	case 0xAC:
		return parseOpenChannel(msg);
	case 0xAD:
		return parseOpenPrivatePlayerChat(msg);
	case 0xAE:
		return parseOpenRuleViolation(msg);
	case 0xAF:
		return parseRuleViolationAF(msg);
	case 0xB0:
		return parseRuleViolationB0(msg);
	case 0xB1:
		return parseRuleViolationB1(msg);
	case 0xB2:
		return parseCreatePrivateChannel(msg);
	case 0xB3:
		return parseClosePrivateChannel(msg);
	case 0xB4:
		return parseTextMessage(msg);
	case 0xB5:
		return parsePlayerCancelWalk(msg);
	case 0xBE:
		return parseFloorChangeUp(msg);
	case 0xBF:
		return parseFloorChangeDown(msg);
	case 0xC8:
		return parseOutfitWindow(msg);
	case 0xD2:
		return parseVipState(msg);
	case 0xD3:
		return parseVipLogin(msg);
	case 0xD4:
		return parseVipLogout(msg);
	case 0xF0:
		return parseQuestList(msg);
	case 0xF1:
		return parseQuestPartList(msg);


	// 8.2+
	case 0x7A:
		return parseOpenShopWindow(msg);
	case 0x7B:
		return parsePlayerCash(msg);
	case 0x7C:
		return parseCloseShopWindow(msg);
	case 0xDC:
		return parseShowTutorial(msg);
	case 0xDD:
		return parseAddMapMarker(msg);
    }
    return false;
}
bool ProtocolGame::parseSelfAppear(NetworkMessage& msg)
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
	printf("Entered game\n");
    return true;
}
bool ProtocolGame::parseGMActions(NetworkMessage& msg)
{
    for(uint32_t i = 0; i < 28; ++i){
        MSG_READ_U8(GMByte);
        GlobalVariables::setGMAction(i, GMByte);
    }
    return true;
}
bool ProtocolGame::parseErrorMessage(NetworkMessage& msg)
{
    MSG_READ_STRING(errorMessage);
    Notifications::openMessageWindow(MESSAGE_ERROR, errorMessage);
    return true;
}
bool ProtocolGame::parseFYIMessage(NetworkMessage& msg)
{
    MSG_READ_STRING(infoMessage);
    Notifications::openMessageWindow(MESSAGE_INFORMATION, infoMessage);
    return true;
}
bool ProtocolGame::parseWaitingList(NetworkMessage& msg)
{
    MSG_READ_STRING(waitMessage);
    MSG_READ_U8(waitTime);
    Notifications::openWaitingList(waitMessage, waitTime);
    return true;
}
bool ProtocolGame::parsePing(NetworkMessage& msg)
{
    //no data
    sendPing();
    return true;
}
bool ProtocolGame::parseDeath(NetworkMessage& msg)
{
    //no data
    Notifications::openRelogin();
    return true;
}
bool ProtocolGame::parseCanReportBugs(NetworkMessage& msg)
{
    MSG_READ_U8(reportBugs);
    return true;
}

bool ProtocolGame::parseMapDescription(NetworkMessage& msg)
{
    MSG_READ_POSITION(currentPos);
    GlobalVariables::getPlayerPosition() = currentPos;
    if(!setMapDescription(msg, currentPos.x - 8, currentPos.y - 6, currentPos.z, 18, 14)){
        RAISE_PROTOCOL_ERROR("Map description 0x64");
    }
    return true;
}
bool ProtocolGame::parseMoveNorth(NetworkMessage& msg)
{
    Position& myPos = GlobalVariables::getPlayerPosition();
    myPos.y--;
    if(!setMapDescription(msg, myPos.x - 8, myPos.y - 6, myPos.z, 18, 1)){
        RAISE_PROTOCOL_ERROR("Map description 0x65");
    }
    Notifications::onWalk();
    return true;
}
bool ProtocolGame::parseMoveEast(NetworkMessage& msg)
{
    Position& myPos = GlobalVariables::getPlayerPosition();
    myPos.x++;
    if(!setMapDescription(msg, myPos.x + 9, myPos.y - 6, myPos.z, 1, 14)){
        RAISE_PROTOCOL_ERROR("Map description 0x66");
    }
    Notifications::onWalk();
    return true;
}
bool ProtocolGame::parseMoveSouth(NetworkMessage& msg)
{
    Position& myPos = GlobalVariables::getPlayerPosition();
    myPos.y++;
    if(!setMapDescription(msg, myPos.x - 8, myPos.y + 7, myPos.z, 18, 1)){
        RAISE_PROTOCOL_ERROR("Map description 0x67");
    }
    Notifications::onWalk();
    return true;
}
bool ProtocolGame::parseMoveWest(NetworkMessage& msg)
{
    Position& myPos = GlobalVariables::getPlayerPosition();
    myPos.x--;
    if(!setMapDescription(msg, myPos.x - 8, myPos.y - 6, myPos.z, 1, 14)){
        RAISE_PROTOCOL_ERROR("Map description 0x68");
    }
    Notifications::onWalk();
    return true;
}

bool ProtocolGame::parseUpdateTile(NetworkMessage& msg)
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

    return true;
}
bool ProtocolGame::parseTileAddThing(NetworkMessage& msg)
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

    return true;
}
bool ProtocolGame::parseTileTransformThing(NetworkMessage& msg)
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

    return true;
}
bool ProtocolGame::parseTileRemoveThing(NetworkMessage& msg)
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
    return true;
}
bool ProtocolGame::parseCreatureMove(NetworkMessage& msg)
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
    return true;
}

bool ProtocolGame::parseOpenContainer(NetworkMessage& msg)
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

    // NOTE (nfries88)
    // The server sends a message to open a container when it is moved client-side
    // In the event of it already being opened, don't remake it
    // but allow updates.
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
        // When the server sends a message to open a container that's already opened
        if(container->getItem(i)) {
            if(!container->updateItem(i, item)) {
                RAISE_PROTOCOL_ERROR("Container Open - updateItem");
            }
        }
        else if(!container->addItemInitial(item)){
            RAISE_PROTOCOL_ERROR("Container Open - addItem");
        }
    }
    Notifications::openContainer(cid);
    return true;
}
bool ProtocolGame::parseCloseContainer(NetworkMessage& msg)
{
    MSG_READ_U8(cid);
    Containers::getInstance().deleteContainer(cid);
    Notifications::closeContainer(cid);
    return true;
}
bool ProtocolGame::parseContainerAddItem(NetworkMessage& msg)
{
    MSG_READ_U8(cid);
    Item* item = internalGetItem(msg, (uint32_t)-1);
    if(!item){
        RAISE_PROTOCOL_ERROR("Container add - !item");
    }

    Container* container = Containers::getInstance().getContainer(cid);
    if(!container){
        //RAISE_PROTOCOL_ERROR("Container add - !container");
        return true;
    }
    if(!container->addItem(item)){
        RAISE_PROTOCOL_ERROR("Container add - addItem");
    }
    return true;
}
bool ProtocolGame::parseContainerUpdateItem(NetworkMessage& msg)
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
        return true;
    }
    if(!container->updateItem(slot, item)){
        RAISE_PROTOCOL_ERROR("Container update - updateItem");
    }
    return true;
}
bool ProtocolGame::parseContainerRemoveItem(NetworkMessage& msg)
{
    MSG_READ_U8(cid);
    MSG_READ_U8(slot);

    Container* container = Containers::getInstance().getContainer(cid);
    if(!container){
        //RAISE_PROTOCOL_ERROR("Container remove - !container");
        return true;
    }
    if(!container->removeItem(slot)){
        RAISE_PROTOCOL_ERROR("Container remove - removeItem");
    }
    //TODO. send update container

    return true;
}
bool ProtocolGame::parseInventorySetSlot(NetworkMessage& msg)
{
    MSG_READ_U8(slot);
    Item* item = internalGetItem(msg, (uint32_t)-1);
    if(!item){
        RAISE_PROTOCOL_ERROR("Inventory set - !item");
    }
    if(!Inventory::getInstance().addItem(slot, item)){
        RAISE_PROTOCOL_ERROR("Inventory set - addItem");
    }

    return true;
}
bool ProtocolGame::parseInventoryResetSlot(NetworkMessage& msg)
{
    MSG_READ_U8(slot);
    if(!Inventory::getInstance().removeItem(slot)){
        RAISE_PROTOCOL_ERROR("Inventory reset - removeItem");
    }

    return true;
}
bool ProtocolGame::parseSafeTradeRequestAck(NetworkMessage& msg)
{
    return parseSafeTradeRequest(msg, true);
}
bool ProtocolGame::parseSafeTradeRequestNoAck(NetworkMessage& msg)
{
    return parseSafeTradeRequest(msg, false);
}
bool ProtocolGame::parseSafeTradeRequest(NetworkMessage& msg, bool ack)
{
    MSG_READ_STRING(name);
    MSG_READ_U8(count);

    Container* container = NULL;
    if(ack){
        Containers::getInstance().newTradeContainerAck();
    }
    else{
        Containers::getInstance().newTradeContainer();
    }

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

    Notifications::openTradeWindow(ack);
    return true;
}
bool ProtocolGame::parseSafeTradeClose(NetworkMessage& msg)
{
    //no data
    Containers::getInstance().closeTradeContainer();
    Notifications::closeTradeWindow();
    return true;
}
bool ProtocolGame::parseWorldLight(NetworkMessage& msg)
{
    MSG_READ_U8(level);
    MSG_READ_U8(color);
    GlobalVariables::setWorldLightLevel(level);
    GlobalVariables::setWorldLightColor(color);
    return true;
}
bool ProtocolGame::parseMagicEffect(NetworkMessage& msg)
{
    MSG_READ_POSITION(effectPos);
    MSG_READ_U8(effect);
    // FIXME (ivucica#5#) should ask Objects class if its in range and not conclude on its own
/*			if(effect == 0 || effect > 35){
        RAISE_PROTOCOL_ERROR("Magic effect - out of range");
    }*/
    Tile* tile = Map::getInstance().getTile(effectPos);
    if(!tile){
        RAISE_PROTOCOL_ERROR("Magic effect - !tile");
    }
    tile->addEffect(effect);
    return true;
}
bool ProtocolGame::parseAnimatedText(NetworkMessage& msg)
{
    MSG_READ_POSITION(textPos);
    MSG_READ_U8(color);
    MSG_READ_STRING(text);
    Map::getInstance().addAnimatedText(textPos, color, text);
    return true;
}
bool ProtocolGame::parseDistanceShot(NetworkMessage& msg)
{
    MSG_READ_POSITION(fromPos);
    MSG_READ_POSITION(toPos);
    MSG_READ_U8(effect);
    // FIXME (ivucica#5#) should ask Objects class if it's out of range and not conclude on its own
    /*			if(effect == 0 || effect > 28){
        RAISE_PROTOCOL_ERROR("Distance shoot - out of range");
    }*/
    Map::getInstance().addDistanceEffect(fromPos, toPos, effect);
    return true;
}

bool ProtocolGame::parseCreatureSquare(NetworkMessage& msg)
{
    MSG_READ_U32(creatureID);
    MSG_READ_U8(color);
    Creature* creature = Creatures::getInstance().getCreature(creatureID);
    if(creature){
        creature->setSquare(color);
    }
    return true;
}
bool ProtocolGame::parseCreatureHealth(NetworkMessage& msg)
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
    return true;
}
bool ProtocolGame::parseCreatureLight(NetworkMessage& msg)
{
    MSG_READ_U32(creatureID);
    MSG_READ_U8(level);
    MSG_READ_U8(color);
    Creature* creature = Creatures::getInstance().getCreature(creatureID);
    if(creature){
        creature->setLightLevel(level);
        creature->setLightColor(color);
    }
    return true;
}
bool ProtocolGame::parseCreatureOutfit(NetworkMessage& msg)
{
    MSG_READ_U32(creatureID);
    Creature* creature = Creatures::getInstance().getCreature(creatureID);
    if(creature){
        if(!internalSetOutfit(msg, creature->getOutfit())){
            RAISE_PROTOCOL_ERROR("Creature outfit - outfit error");
        } else {
            printf("===> Updating creature outfit\n");
            creature->unloadGfx(); // FIXME (ivucica#2#): move this to notifications
            creature->loadOutfit();
            printf("===> Updated creature outfit\n");

        }
    }
    else{
        Outfit_t dummy;
        if(!internalSetOutfit(msg, dummy)){
            RAISE_PROTOCOL_ERROR("Creature outfit - outfit error2");
        }

    }
    return true;
}
bool ProtocolGame::parseCreatureSpeed(NetworkMessage& msg)
{
    MSG_READ_U32(creatureID);
    MSG_READ_U16(speed);
    Creature* creature = Creatures::getInstance().getCreature(creatureID);
    if(creature){
        creature->setSpeed(speed);
    }
    return true;
}
bool ProtocolGame::parseCreatureSkulls(NetworkMessage& msg)
{
    MSG_READ_U32(creatureID);
    MSG_READ_U8(skull);
    Creature* creature = Creatures::getInstance().getCreature(creatureID);
    if(creature){
        creature->setSkull(skull);
    }
    return true;
}
bool ProtocolGame::parseCreatureShields(NetworkMessage& msg)
{
    MSG_READ_U32(creatureID);
    MSG_READ_U8(shield);
    Creature* creature = Creatures::getInstance().getCreature(creatureID);
    if(creature){
        creature->setShield(shield);
    }
    return true;
}
bool ProtocolGame::parseItemTextWindow(NetworkMessage& msg)
{
	MSG_READ_U32(windowID);
	MSG_READ_U16(itemID);
	MSG_READ_U16(maxlen);
	MSG_READ_STRING(text);
	MSG_READ_STRING(writter);
	MSG_READ_STRING(date);
	Notifications::openItemText(windowID, itemID, maxlen, text, writter, date);
    return true;
}
bool ProtocolGame::parseHouseTextWindow(NetworkMessage& msg)
{
	MSG_READ_U8(unk);
	MSG_READ_U32(windowID);
	MSG_READ_STRING(text);
	Notifications::openHouseText(windowID, unk, text);
	return true;
}
bool ProtocolGame::parsePlayerStats(NetworkMessage& msg)
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

	return true;
}
bool ProtocolGame::parsePlayerSkills(NetworkMessage& msg)
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

	return true;
}
bool ProtocolGame::parsePlayerIcons(NetworkMessage& msg)
{
	MSG_READ_U16(icons);
	GlobalVariables::setPlayerIcons(icons);
	Notifications::onChangeIcons();
	return true;
}
bool ProtocolGame::parsePlayerCancelAttack(NetworkMessage& msg)
{
	//no data
	Notifications::onCancelAttack();
	return true;
}
bool ProtocolGame::parseCreatureSpeak(NetworkMessage& msg)
{
	MSG_READ_U32(unkSpeak);
	MSG_READ_STRING(name);
	MSG_READ_U16(level);
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
			Notifications::onCreatureSpeak((SpeakClasses_t)newtype, unkSpeak, name, level, creaturePos, text);
			break;
		}
		case SPEAK_CHANNEL_R1:
		case SPEAK_CHANNEL_R2:
		case SPEAK_CHANNEL_O:
		case SPEAK_CHANNEL_Y:
		{
			MSG_READ_U16(channelID);
			MSG_READ_STRING(text);
			Notifications::onCreatureSpeak((SpeakClasses_t)newtype, unkSpeak, name, level, channelID, text);
			break;
		}
		case SPEAK_PRIVATE:
		case SPEAK_BROADCAST:
		case SPEAK_PRIVATE_RED:
		{
			MSG_READ_STRING(text);
			Notifications::onCreatureSpeak((SpeakClasses_t)newtype, unkSpeak, name, level, text);
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
bool ProtocolGame::parseChannelList(NetworkMessage& msg)
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
	return true;
}
bool ProtocolGame::parseOpenChannel(NetworkMessage& msg)
{
	MSG_READ_U16(channelID);
	MSG_READ_STRING(name);
	Notifications::openChannel(channelID, name);
	return true;
}
bool ProtocolGame::parseOpenPrivatePlayerChat(NetworkMessage& msg)
{
	MSG_READ_STRING(name);
	Notifications::openPrivatePlayerChat(name);
	return true;
}
bool ProtocolGame::parseOpenRuleViolation(NetworkMessage& msg)
{
	//TODO
	MSG_READ_U16(a);
	return true;
}
bool ProtocolGame::parseRuleViolationAF(NetworkMessage& msg)
{
	//TODO
	MSG_READ_U16(a);
	return true;
}
bool ProtocolGame::parseRuleViolationB0(NetworkMessage& msg)
{
	//TODO
	MSG_READ_U16(a);
	return true;
}
bool ProtocolGame::parseRuleViolationB1(NetworkMessage& msg)
{
	//TODO
	MSG_READ_U16(a);
	return true;
}
bool ProtocolGame::parseCreatePrivateChannel(NetworkMessage& msg)
{
	MSG_READ_U16(channelID);
	MSG_READ_STRING(name);
	Notifications::openPrivateChannel(channelID, name);
	return true;
}
bool ProtocolGame::parseClosePrivateChannel(NetworkMessage& msg)
{
	MSG_READ_U16(channelID);
	Notifications::closePrivateChannel(channelID);
	return true;
}
bool ProtocolGame::parseTextMessage(NetworkMessage& msg)
{
	MSG_READ_U8(messageType);
	MSG_READ_STRING(text);
	if(messageType < 0x11 || messageType > 0x26){
		printf("text msg type 0x%02x\n", messageType);
		RAISE_PROTOCOL_ERROR("text message - type");
	}
	messageType = translateTextMessageToInternal(messageType);

	Notifications::onTextMessage((MessageType_t)messageType, text);
	return true;
}
bool ProtocolGame::parsePlayerCancelWalk(NetworkMessage& msg)
{
	MSG_READ_U8(direction);
	if(direction > 3){
		RAISE_PROTOCOL_ERROR("cancel walk - direction > 3");
	}
	Notifications::onCancelWalk((Direction)direction);
	return true;
}
bool ProtocolGame::parseFloorChangeUp(NetworkMessage& msg)
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
	return true;
}
bool ProtocolGame::parseFloorChangeDown(NetworkMessage& msg)
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

	return true;
}
bool ProtocolGame::parseOutfitWindow(NetworkMessage& msg)
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
	return true;
}
bool ProtocolGame::parseVipState(NetworkMessage& msg)
{
	MSG_READ_U32(creatureID);
	MSG_READ_STRING(name);
	MSG_READ_U8(online);
	Notifications::onVipState(creatureID, name, online);
	return true;
}
bool ProtocolGame::parseVipLogin(NetworkMessage& msg)
{
	MSG_READ_U32(creatureID);
	Notifications::onVipLogin(creatureID);
	return true;
}
bool ProtocolGame::parseVipLogout(NetworkMessage& msg)
{
	MSG_READ_U32(creatureID);
	Notifications::onVipLogout(creatureID);
	return true;
}
bool ProtocolGame::parseQuestList(NetworkMessage& msg)
{
	//TODO.GUI
	MSG_READ_U16(nQuests);
	for(uint32_t i = 0; i < nQuests; ++i){
		MSG_READ_U16(questsID);
		MSG_READ_STRING(questsName);
		MSG_READ_U8(questsState);
	}
	return true;
}
bool ProtocolGame::parseQuestPartList(NetworkMessage& msg)
{
	//TODO.GUI
	MSG_READ_U16(questsID);
	MSG_READ_U8(nMission);
	for(uint32_t i = 0; i < nMission; ++i){
		MSG_READ_STRING(questsName);
		MSG_READ_STRING(questsDesc);
	}
	return true;
}

// 8.2+
bool ProtocolGame::parseOpenShopWindow(NetworkMessage& msg)
{
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
	return true;
}
bool ProtocolGame::parsePlayerCash(NetworkMessage& msg)
{
	MSG_READ_U32(cash);
	GlobalVariables::setPlayerCash(cash);
	Notifications::onUpdatePlayerCash(cash);
	return true;
}
bool ProtocolGame::parseCloseShopWindow(NetworkMessage& msg)
{
	Notifications::closeShopWindow();
	return true;
}
bool ProtocolGame::parseShowTutorial(NetworkMessage& msg)
{
	MSG_READ_U8(tutorialID);
	Notifications::showTutorial(tutorialID);
	return true;
}
bool ProtocolGame::parseAddMapMarker(NetworkMessage& msg)
{
	MSG_READ_POSITION(myPos);
	MSG_READ_U8(icon);
	MSG_READ_STRING(desc);
	Notifications::addMapMark(icon, myPos, desc);
	return true;
}

//******send functions***********
//public
void ProtocolGame::sendLogout()
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x14);
}

void ProtocolGame::sendAutoWalk(const std::list<Direction>& steps)
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

void ProtocolGame::sendStopAutoWalk()
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x69);
}

void ProtocolGame::sendMove(Direction dir)
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

void ProtocolGame::sendTurn(Direction dir)
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

void ProtocolGame::sendThrow(const Position& fromPos, uint16_t itemid,
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

void ProtocolGame::sendRequestTrade(const Position& pos, uint16_t itemid,
									uint8_t stackpos, uint32_t playerid)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x7D);
	m_outputMessage.addPosition(pos);
	m_outputMessage.addU16(itemid);
	m_outputMessage.addU8(stackpos);
	m_outputMessage.addU32(playerid);
}

void ProtocolGame::sendLookInTrade(bool inMyOffer, uint8_t index)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x7E);
	m_outputMessage.addU8(inMyOffer ? 0 : 1);
	m_outputMessage.addU8(index);
}

void ProtocolGame::sendAcceptTrade()
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x7F);
}

void ProtocolGame::sendRejectTrade()
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x80);
}

void ProtocolGame::sendUseItem(const Position& pos, uint16_t itemid,
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

void ProtocolGame::sendUseItemWith(const Position& fromPos, uint16_t fromItemid,
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

void ProtocolGame::sendUseBattleWindow(const Position& pos, uint16_t itemid,
									   uint8_t stackpos, uint32_t creatureid)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x84);
	m_outputMessage.addPosition(pos);
	m_outputMessage.addU16(itemid);
	m_outputMessage.addU8(stackpos);
	m_outputMessage.addU32(creatureid);
}

void ProtocolGame::sendRotateItem(const Position& pos, uint16_t itemid,
								  uint8_t stackpos)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x85);
	m_outputMessage.addPosition(pos);
	m_outputMessage.addU16(itemid);
	m_outputMessage.addU8(stackpos);
}

void ProtocolGame::sendCloseContainer(uint8_t containerIndex)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x87);
	m_outputMessage.addU8(containerIndex);
}

void ProtocolGame::sendUpContainer(uint8_t containerIndex)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x88);
	m_outputMessage.addU8(containerIndex);
}

void ProtocolGame::sendTextWindow(uint32_t windowId, const std::string& text)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x89);
	m_outputMessage.addU32(windowId);
	m_outputMessage.addString(text);
}

void ProtocolGame::sendHouseWindow(uint32_t windowId, int unk,
								   const std::string& text)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x8A);
	m_outputMessage.addU8(unk);
	m_outputMessage.addU32(windowId);
	m_outputMessage.addString(text);
}

void ProtocolGame::sendLookItem(const Position& pos, uint16_t itemid,
								uint8_t stackpos)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x8C);
	m_outputMessage.addPosition(pos);
	m_outputMessage.addU16(itemid);
	m_outputMessage.addU8(stackpos);
}

void ProtocolGame::sendSay(SpeakClasses_t type, const std::string text)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x96);
	m_outputMessage.addU8(type);
	m_outputMessage.addString(text);
}

void ProtocolGame::sendSay(SpeakClasses_t type, uint16_t channel,
							 const std::string text)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x96);
	m_outputMessage.addU8(type);
	m_outputMessage.addU16(channel);
	m_outputMessage.addString(text);
}

void ProtocolGame::sendSay(SpeakClasses_t type, const std::string& sendToplayer,
							 const std::string text)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x96);
	m_outputMessage.addU8(type);
	m_outputMessage.addString(sendToplayer);
	m_outputMessage.addString(text);
}

void ProtocolGame::sendRequestChannels()
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x97);
}

void ProtocolGame::sendOpenChannel(uint16_t channelid)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x98);
	m_outputMessage.addU16(channelid);
}

void ProtocolGame::sendCloseChannel(uint16_t channelid)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x99);
	m_outputMessage.addU16(channelid);
}

void ProtocolGame::sendOpenPrivatePlayerChat(const std::string& playerName)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x9A);
	m_outputMessage.addString(playerName);
}

void ProtocolGame::sendOpenRuleViolation(const std::string& text)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x9B);
	m_outputMessage.addString(text);
}

void ProtocolGame::sendCloseRuleViolation(const std::string& text)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x9C);
	m_outputMessage.addString(text);
}

void ProtocolGame::sendCancelRuleViolation()
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x9D);
}

void ProtocolGame::sendFightModes(uint8_t attack, uint8_t chase, uint8_t secure)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xA0);
	m_outputMessage.addU8(attack);
	m_outputMessage.addU8(chase);
	m_outputMessage.addU8(secure);
}

void ProtocolGame::sendAttackCreature(uint32_t creatureid)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xA1);
	m_outputMessage.addU32(creatureid);
	GlobalVariables::setAttackID(creatureid);
}

void ProtocolGame::sendFollowCreature(uint32_t creatureid)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xA2);
	m_outputMessage.addU32(creatureid);
	GlobalVariables::setFollowID(creatureid);
}

void ProtocolGame::sendCreatePrivateChatChannel()
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xAA);
}

void ProtocolGame::sendInvitePrivateChatChannel(const std::string& name)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xAB);
	m_outputMessage.addString(name);
}

void ProtocolGame::sendExcludePrivateChatChannel(const std::string& name)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xAC);
	m_outputMessage.addString(name);
}

void ProtocolGame::sendInviteParty(uint32_t playerid)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xA3);
	m_outputMessage.addU32(playerid);
}

void ProtocolGame::sendJoinParty(uint32_t playerid)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xA4);
	m_outputMessage.addU32(playerid);
}

void ProtocolGame::sendCancelInviteParty(uint32_t playerid)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xA5);
	m_outputMessage.addU32(playerid);
}

void ProtocolGame::sendPassPartyLeader(uint32_t playerid)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xA6);
	m_outputMessage.addU32(playerid);
}

void ProtocolGame::sendLeaveParty()
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xA7);
}

void ProtocolGame::sendCancelMove()
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xBE);
	GlobalVariables::setAttackID(0);
	GlobalVariables::setFollowID(0);
}

void ProtocolGame::sendRequestOutfit()
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xD2);
}

void ProtocolGame::sendSetOutfit(uint16_t looktype, uint8_t head, uint8_t body,
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

void ProtocolGame::sendAddVip(const std::string& name)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xDC);
	m_outputMessage.addString(name);
}

void ProtocolGame::sendRemVip(uint32_t playerid)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xDD);
	m_outputMessage.addU32(playerid);
}

void ProtocolGame::sendBugReport(const std::string& text)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xE6);
	m_outputMessage.addString(text);
}

void ProtocolGame::sendGMReport(const std::string& targetplayer, uint8_t reason, const std::string& comment, const std::string& statement, uint8_t action, bool ipban)
{
    // 8.3 code; perhaps needs updating
    PROTOCOLGAME_SEND_FUNCTION;
    m_outputMessage.addMessageType(0xE7);
    m_outputMessage.addString(targetplayer);
    m_outputMessage.addU8(reason);
    m_outputMessage.addU8(action);
    m_outputMessage.addString(comment);
    m_outputMessage.addString(statement);
    m_outputMessage.addU16(0); // unknown!
    m_outputMessage.addU8(ipban ? 1 : 0);
}

void ProtocolGame::sendRequestQuestLog()
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xF0);
}

void ProtocolGame::sendRequestQuest(uint16_t questid)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xF1);
	m_outputMessage.addU16(questid);
}
//8.2+
void ProtocolGame::sendCloseNPCChannel()
{
    PROTOCOLGAME_SEND_FUNCTION;
    m_outputMessage.addMessageType(0x9E);
}
void ProtocolGame::sendShopPurchase(uint16_t itemid, uint8_t count, uint8_t amount, bool ignoreCap, bool withBackpack)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x7A);
	m_outputMessage.addU8(itemid);
	m_outputMessage.addU8(count);
	m_outputMessage.addU8(amount);
	if (getVersion() >= CLIENT_VERSION_830){ // FIXME (ivucica#5#): make a replacement ProtocolGame83:: function
		m_outputMessage.addU8(ignoreCap ? 1 : 0);
		m_outputMessage.addU8(withBackpack ? 1 : 0);
	}
}
void ProtocolGame::sendShopSale(uint16_t itemid, uint8_t count, uint8_t amount)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x7B);
	m_outputMessage.addU8(itemid);
	m_outputMessage.addU8(count);
	m_outputMessage.addU8(amount);
}
void ProtocolGame::sendShopClose()
{
    PROTOCOLGAME_SEND_FUNCTION;
    m_outputMessage.addMessageType(0x7C);
}

//protected
void ProtocolGame::sendPing()
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0x1E);
}

void ProtocolGame::sendRequestUpdateTile(const Position& pos)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xC9);
	m_outputMessage.addPosition(pos);
}

void ProtocolGame::sendRequestUpdateContainer(uint8_t containerid)
{
	PROTOCOLGAME_SEND_FUNCTION;
	m_outputMessage.addMessageType(0xCA);
	m_outputMessage.addU8(containerid);
}



// generic structure parsing functions
bool ProtocolGame::setMapDescription(NetworkMessage& msg, int x, int y, int z, int width, int height)
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
		    printf("Failed to set floor desc\n");
			return false;
		}
	}
	return true;
}



bool ProtocolGame::setFloorDescription(NetworkMessage& msg, int x, int y, int z, int width, int height, int offset)
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
					    printf("Failed to set tile descr\n");
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

bool ProtocolGame::setTileDescription(NetworkMessage& msg, const Position& pos)
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
			    printf("Too many things\n");
				return false;
			}
			//read tile things: items and creatures
			Thing* thing = internalGetThing(msg);
			if(!thing){
			    printf("Failed to get thing\n");
				return false;
			}
			//and add to the tile
			if(!tile->addThing(thing)) {
			    printf("Failed to add thing\n");
				return false;
			}
		}
	}
}

