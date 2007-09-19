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
#include "protocollogin.h"
#include "networkmessage.h"
#include "rsa.h"

ProtocolLogin::ProtocolLogin(int account, const std::string& password)
{
	m_account = account;
	m_password = password;
}

void ProtocolLogin::onConnect()
{
	ProtocolConfig* config = ProtocolConfig::getInstance();
	NetworkMessage output(NetworkMessage::CAN_WRITE);
	output.addU8(0x01); //Login Protocol
	output.addU16(config->os); // os; 1 = linux, 2 = windows
	output.addU16(config->clientVersion); //Client version
	output.addU32(config->datSignature); //tibia.dat signature
	output.addU32(config->sprSignature); //tibia.spr signature
	output.addU32(config->picSignature); //tibia.pic signature

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
	ASSERT(RSA::getInstance()->encrypt(rsaBuffer, 128));

	m_connection->sendMessage(output);
	m_connection->setKey((char*)k, 4*sizeof(uint32_t));
	m_connection->setCryptoState(true);
}

bool ProtocolLogin::onRecv(NetworkMessage& msg)
{
	while(msg.getReadSize() > 0){
		uint8_t cmd = msg.getU8();
		switch(cmd){
		case 0x0A: //message
			printf("Message: %s\n", msg.getString().c_str());
			break;
		case 0x14: //MOTD
			printf("MOTD: %s\n", msg.getString().c_str());
			break;
		case 0x64: //character list
		{
			uint8_t chars = msg.getU8();
			printf("Chars: %d\n", chars);
			for(uint32_t i = 0; i < chars; ++i){
				std::string name = msg.getString();
				std::string world = msg.getString();
				uint32_t world_ip = msg.getU32();
				uint16_t port = msg.getU16();
				printf("Char %d: %s(%s) %d:%d\n",
					i, name.c_str(), world.c_str(), world_ip, port);
			}
			uint16_t prem = msg.getU16();
			printf("Premium: %d\n", prem);
			break;
		}
		default:
			printf("ERROR: Unk login packet type\n");
			return false;
			break;
		}
	}
	return true;
}
