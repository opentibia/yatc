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
#include "protocolgame853.h"
#include "../gamecontent/position.h"
#include "../gamecontent/map.h"
#include "../defines.h"

ProtocolGame853::ProtocolGame853(const std::string& accountname, const std::string& password, const std::string& name, bool isGM) :
ProtocolGame85(accountname, password, name, isGM)
{
	//
}

ProtocolGame853::~ProtocolGame853()
{
	//
}

void ProtocolGame853::checkVersion()
{
	ASSERT(ProtocolConfig::getInstance().getClientVersion() == CLIENT_VERSION_853);
}



//////


bool ProtocolGame853::parsePacket(uint8_t cmd, NetworkMessage& msg)
{
    // example for overrides
    printf("%s\n", __PRETTY_FUNCTION__);
    printf("Packet %02x\n", cmd);
    switch(cmd){
    case 0x1F:
        //m_connection->setChecksumState(false);
        sendLogin(&msg);
        return true;
    }
    return ProtocolGame::parsePacket(cmd,msg);
}

void ProtocolGame853::onConnect()
{
    // we do logging in by handling 0x1F
    // here we only tell "skip checksum in unencypted packet"
    printf("%s\n", __PRETTY_FUNCTION__);
    m_connection->setChecksumState(true);
}

bool ProtocolGame853::parseTileAddThing(NetworkMessage& msg)
{
    MSG_READ_POSITION(tilePos);
    MSG_READ_U8(stackPos);
    Thing* thing = internalGetThing(msg);
    if(!thing){
        RAISE_PROTOCOL_ERROR("Tile Add - getThing");
    }

    Tile* tile = Map::getInstance().getTile(tilePos);
    if(!tile){
        RAISE_PROTOCOL_ERROR("Tile Add - !tile");
    }

    if(!tile->insertThing(thing, stackPos)){
        RAISE_PROTOCOL_ERROR("Tile Add - addThing");
    }
    Notifications::onTileUpdate(tilePos);
    return true;
}
