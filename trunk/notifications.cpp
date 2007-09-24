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

#include "notifications.h"
#include "net/connection.h"

extern Connection* g_connection;

#include "gamecontent/container.h"
#include "gamecontent/creature.h"
#include "gamecontent/globalvars.h"
#include "gamecontent/inventory.h"
#include "gamecontent/map.h"

void Notifications::openCharactersList(const std::list<CharacterList_t>& list, int premDays)
{
	//
}

void Notifications::onConnectionError(int message)
{
	printf("********************************************************\n");
	printf("CONNECTION ERROR: %d - %s\n", message, Connection::getErrorDesc(message));
	printf("********************************************************\n");
}

void Notifications::onProtocolError(bool fatal)
{
	Protocol* protocol = g_connection->getProtocol();
	std::string error = protocol->getErrorDesc();
	printf("\n********************************************************\n");
	if(fatal){
		printf("PROTOCOL ERROR: %s\n", error.c_str());
	}
	else{
		printf("PROTOCOL WARNING: %s\n", error.c_str());
	}
	const std::list<uint8_t>& serverCmd = protocol->getLastServerCmd();
	std::list<uint8_t>::const_iterator it;
	printf("Last received cmd: ");
	for(it = serverCmd.begin(); it != serverCmd.end(); ++it){
		printf("%02x ", *it);
	}
	printf("\n");
	printf("Total Received: %d\n", protocol->getCurrentMsgN());
	const NetworkMessage* msg = protocol->getCurrentMsg();
	if(msg){
		const unsigned char* buffer = (const unsigned char*)msg->getBuffer();
		if(buffer){
			int32_t msgRealSize = msg->getSize() - msg->getStart();
			printf("Last received msg: (%x/%x)\n", (int)((long)msg->getReadBuffer() - (long)msg->getBuffer()), msgRealSize);
			#define LINE_SIZE 16
			for(int32_t i = 0; i < msgRealSize; i += LINE_SIZE){
				int32_t pos = i;
				printf("%04x : ", i);
				for(int32_t j = 0; j < LINE_SIZE; ++j, ++pos){
					if(pos < msgRealSize){
						printf("%02x ", buffer[pos]);
					}
					else{
						printf("   ");
					}
				}
				printf("; ");
				pos = i;
				for(uint32_t j = 0; j < LINE_SIZE; ++j, ++pos){
					if(pos < msgRealSize){
						if(buffer[pos] >= 32 && buffer[pos] < 127){
							printf("%c", buffer[pos]);
						}
						else{
							printf(".");
						}
					}
					else{
						printf(" ");
					}
				}
				printf("\n");
			}
		}
	}
	printf("********************************************************\n");
}
