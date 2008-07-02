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
#include "protocollogin.h"
#include "networkmessage.h"
#include "rsa.h"
#include "../notifications.h"
#include "../debugprint.h"
ProtocolLogin::ProtocolLogin(uint32_t account, const std::string& password)
{
	m_account = account;
	m_password = password;
}

void ProtocolLogin::onConnect()
{
	ProtocolConfig& config = ProtocolConfig::getInstance();
	NetworkMessage output(NetworkMessage::CAN_WRITE);
	output.addU8(0x01); //Login Protocol
	output.addU16(config.getOS());
	output.addU16(config.getVersionOverride() ? config.getVersionOverride() : config.getClientVersion());
	output.addU32(config.getDatSignature());
	output.addU32(config.getSprSignature());
	output.addU32(config.getPicSignature());


	DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Logging on with version: %d dat: %08x spr: %08x pic: %08x\n", config.getVersionOverride() ? config.getVersionOverride() : config.getClientVersion(), config.getDatSignature(), config.getSprSignature(), config.getPicSignature());


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

	output.addU32(m_account);
	output.addString(m_password);

	//Rsa size has to be 128
	int rsaSize = output.getSize() - sizeBefore;
	output.addPaddingBytes(128 - rsaSize);

	char* rsaBuffer = output.getBuffer() + sizeBefore;
	RSA::getInstance()->encrypt(rsaBuffer, 128);

	m_connection->sendMessage(output);
	m_connection->setKey((char*)k, 4*sizeof(uint32_t));
	m_connection->setCryptoState(true);

	m_account = 0;
	m_password = "";
}

bool ProtocolLogin::onRecv(NetworkMessage& msg)
{
	m_currentMsg = &msg;
	m_currentMsgN++;
	while(msg.getReadSize() > 0){
		MSG_READ_U8(cmd);
		addServerCmd(cmd);
		DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "Received packet 0x%02x\n", cmd);
		switch(cmd){
		case 0x0A: //Error message
		{
			MSG_READ_STRING(errorMessage);
			Notifications::openMessageWindow(MESSAGE_ERROR, errorMessage);
			break;
		}
		case 0x0B: //For your information
		{
			MSG_READ_STRING(infoMessage);
			Notifications::openMessageWindow(MESSAGE_INFORMATION, infoMessage);
			break;
		}
		case 0x14: //MOTD
		{
			MSG_READ_STRING(motd);
			Notifications::openMessageWindow(MESSAGE_MOTD, motd);
			break;
		}
		case 0x1E: //Patching exe/dat/spr messages
		case 0x1F:
		case 0x20:
		{
			Notifications::openMessageWindow(MESSAGE_ERROR, "Patching is not supported.");
			RAISE_PROTOCOL_ERROR("Patching is not supported");
			break;
		}
		case 0x28: //Select other login server
		{
			Notifications::changeLoginServer();
			break;
		}
		case 0x64: //character list
		{
			std::list<CharacterList_t> charactersList;
			MSG_READ_U8(nchars);
			for(uint32_t i = 0; i < nchars; ++i){
				MSG_READ_STRING(charName);
				MSG_READ_STRING(world);
				MSG_READ_U32(ip);
				MSG_READ_U16(port);

				CharacterList_t tmp;
				tmp.name = charName;
				tmp.world = world;
				tmp.ip = ip;
				tmp.port = port;
				charactersList.push_back(tmp);
			}
			MSG_READ_U16(premiumDays);
			Notifications::openCharactersList(charactersList, premiumDays);
			break;
		}
		default:
			RAISE_PROTOCOL_ERROR("Unknown packet type");
			break;
		}
	}
	return true;
}
