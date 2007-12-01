//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
// Notifications to UI
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

#include <sstream>
#include "notifications.h"
#include "net/connection.h"
#include "gamemode.h"
extern Connection* g_connection;

std::list<std::string> g_recFiles;
std::list<std::string>::iterator g_recIt;

#include "gamecontent/container.h"
#include "gamecontent/creature.h"
#include "gamecontent/globalvars.h"
#include "gamecontent/inventory.h"
#include "gamecontent/map.h"
#include "debugprint.h"

void Notifications::openCharactersList(const std::list<CharacterList_t>& list, int premDays)
{
	g_game->openCharactersList(list, premDays);
}

void Notifications::onConnectionError(int message)
{
	if(message == Connection::ERROR_CLOSED_SOCKET){
		Containers::getInstance().clear();
		Creatures::getInstance().clear();
		GlobalVariables::clear();
		Inventory::getInstance().clear();
		Map::getInstance().clear();
	}
	else{
		g_game->onConnectionError(message, Connection::getErrorDesc(message));

	}
}

void Notifications::onProtocolError(bool fatal)
{
	Protocol* protocol = g_connection->getProtocol();
	std::string error = protocol->getErrorDesc();
	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "\n********************************************************\n");
	if(fatal){
		DEBUGPRINT(DEBUGPRINT_ERROR, DEBUGPRINT_LEVEL_OBLIGATORY, "PROTOCOL ERROR: %s\n", error.c_str());
	}
	else{
		DEBUGPRINT(DEBUGPRINT_WARNING, DEBUGPRINT_LEVEL_OBLIGATORY, "PROTOCOL WARNING: %s\n", error.c_str());
	}
	const std::list<uint8_t>& serverCmd = protocol->getLastServerCmd();
	std::list<uint8_t>::const_iterator it;
	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Last received cmd: ");
	for(it = serverCmd.begin(); it != serverCmd.end(); ++it){
		DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "%02x ", *it);
	}
	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "\n");
	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Total Received: %d\n", protocol->getCurrentMsgN());
	const NetworkMessage* msg = protocol->getCurrentMsg();
	if(msg){
		const unsigned char* buffer = (const unsigned char*)msg->getBuffer();
		if(buffer){
			int32_t msgRealSize = msg->getSize() - msg->getStart();
			DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Last received msg: (%x/%x)\n", (int)((long)msg->getReadBuffer() - (long)msg->getBuffer()), msgRealSize);
			#define LINE_SIZE 16
			for(int32_t i = 0; i < msgRealSize; i += LINE_SIZE){
				int32_t pos = i;
				DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "%04x : ", i);
				for(int32_t j = 0; j < LINE_SIZE; ++j, ++pos){
					if(pos < msgRealSize){
						DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "%02x ", buffer[pos]);
					}
					else{
						DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "   ");
					}
				}
				DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "; ");
				pos = i;
				for(uint32_t j = 0; j < LINE_SIZE; ++j, ++pos){
					if(pos < msgRealSize){
						if(buffer[pos] >= 32 && buffer[pos] < 127){
							DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "%c", buffer[pos]);
						}
						else{
							DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, ".");
						}
					}
					else{
						DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, " ");
					}
				}
				DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "\n");
			}
		}
	}
	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "********************************************************\n");
}



void Notifications::openMessageWindow(WindowMessage_t type, const std::string& message)
{
	if(type == MESSAGE_MOTD){
		std::string text;
		int motdnum;
		sscanf(message.c_str(), "%d", &motdnum);

		text = message.substr(message.find('\n')+1);
		g_game->openMOTD(motdnum, text);
	}
	else{
		g_game->openMessageWindow(type, message);
	}

}


void Notifications::openWaitingList(const std::string& message, int time) {
    // TODO (ivucica#3#) ugly stub, but i'm too lazy for a proper waiting list
    std::stringstream message2;
    message2 << message << std::endl << std::endl << "Retry in " << time << "seconds";
    printf("WAITING LIST!!!\n%s\n", message2.str().c_str());
    g_game->openMessageWindow(MESSAGE_ERROR, message2.str());

}

void Notifications::onTextMessage(MessageType_t type, const std::string& message)
{
	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "%s\n", message.c_str());
}

void Notifications::onEnterGame()
{
	g_game->onEnterGame();
}

void Notifications::onWalk()
{

}
