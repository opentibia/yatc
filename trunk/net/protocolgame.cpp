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

#include "notifications.h"

#include "../gamecontent/globalvars.h"
#include "../gamecontent/map.h"
#include "../gamecontent/item.h"
#include "../gamecontent/creature.h"
#include "../gamecontent/container.h"
#include "../gamecontent/inventory.h"

ProtocolGame::ProtocolGame(int account, const std::string& password, const std::string& name, bool isGM) :
m_outputMessage(NetworkMessage::CAN_WRITE)
{
	m_password = password;
	m_name = name;
	m_account = account;
	m_isGM = isGM;
}

ProtocolGame::~ProtocolGame()
{
	//
}

void ProtocolGame::onConnect()
{
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
	output.addU32(m_account);
	output.addString(m_name);
	output.addString(m_password);

	//RSA size has to be 128
	int rsaSize = output.getSize() - sizeBefore;
	output.addPaddingBytes(128 - rsaSize);

	char* rsaBuffer = output.getBuffer() + sizeBefore;
	RSA::getInstance()->encrypt(rsaBuffer, 128);

	m_connection->sendMessage(output);
	m_connection->setKey((char*)k, 4*sizeof(uint32_t));
	m_connection->setCryptoState(true);

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
    return true;
}
bool ProtocolGame::parseGMActions(NetworkMessage& msg)
{
    for(uint32_t i = 0; i < 32; ++i){
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
