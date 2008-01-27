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
#include "protocolgame.h"
#include "rsa.h"

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
}
