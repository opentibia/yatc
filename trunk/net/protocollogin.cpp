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
#include <time.h> // time
#include <sstream>

#include "../fassert.h"
#include "protocollogin.h"
#include "networkmessage.h"
#include "rsa.h"
#include "../notifications.h"
#include "../debugprint.h"
#include "../util.h"
ProtocolLogin::ProtocolLogin(const std::string& accountname, const std::string& password)
{
	m_accountname = accountname;
	m_account = atoi(accountname.c_str());
	m_password = password;
	m_sendsysconf = false;
}

void ProtocolLogin::onConnect()
{
	printf("Connected!\n");
	ProtocolConfig& config = ProtocolConfig::getInstance();
	int version = config.getVersionOverride() ? config.getVersionOverride() : config.getClientVersion();
	NetworkMessage output(NetworkMessage::CAN_WRITE);
	output.addU8(0x01); //Login Protocol
	output.addU16(config.getOS());
	output.addU16(version);
	output.addU32(config.getDatSignature());
	output.addU32(config.getSprSignature());
	output.addU32(config.getPicSignature());


	DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Logging on with version: %d dat: %08x spr: %08x pic: %08x\n", config.getVersionOverride() ? config.getVersionOverride() : config.getClientVersion(), config.getDatSignature(), config.getSprSignature(), config.getPicSignature());


	int sizeBefore = output.getSize();

	//From here bytes are encrypted using RSA
	//generate XTEA key
    uint32_t k[4];
	if(version > CLIENT_VERSION_760){
        srand(time(NULL)); //TODO use a real seed
        k[0] = (rand() << 16) | rand(); k[1] = (rand() << 16) | rand();
        k[2] = (rand() << 16) | rand(); k[3] = (rand() << 16) | rand();

        output.addU8(0); // first byte have to be 0
        output.addU32(k[0]); //XTEA key
        output.addU32(k[1]);
        output.addU32(k[2]);
        output.addU32(k[3]);
	}

	if(!m_usesaccountname)
		output.addU32(m_account);
	else
		output.addString(m_accountname);
	output.addString(m_password);

    if (m_sendsysconf)
        sendSystemConfiguration(output);

	if(version > CLIENT_VERSION_760){
        //Rsa size has to be 128
        int rsaSize = output.getSize() - sizeBefore;
        output.addPaddingBytes(128 - rsaSize);

        char* rsaBuffer = output.getBuffer() + sizeBefore;
        RSA::getInstance()->encrypt(rsaBuffer, 128);
	}

	//bool oldChecksumState = m_connection->getChecksumState();
	//m_connection->setChecksumState(true);

	m_connection->sendMessage(output);
	if(version > CLIENT_VERSION_760){
        m_connection->setKey((char*)k, 4*sizeof(uint32_t));
        m_connection->setCryptoState(true);
	}
	else {
	    m_connection->setCryptoState(false);
	}
	//m_connection->setChecksumState(oldChecksumState);

	m_account = 0;
	m_accountname = "----------------------"; // fill with nonsense for protection of user
	m_password = "---------------------";
}

bool ProtocolLogin::onRecv(NetworkMessage& msg)
{
	static FILE* patcherFile=NULL;
	static uint32_t patcherTotal=0;
	static uint32_t patcherSoFar=0;
	static std::string patcherFN="";
	m_currentMsg = &msg;
	m_currentMsgN++;
	while(msg.getReadSize() > 0){
		MSG_READ_U8(cmd);
		addServerCmd(cmd);
		DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Received packet 0x%02x\n", cmd);
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
		case 0x1E: // Patching part?
		{
			MSG_READ_U8(p_type); // type?
			MSG_READ_U32(p_size); // size?
			
			patcherSoFar = 0;
			patcherTotal = p_size;
			
			switch(p_type)
			{
				case 0:
					patcherFile = yatc_fopen((patcherFN="part.exe").c_str(),"w");
					break;
				case 1:
					patcherFile = yatc_fopen((patcherFN="part.dat").c_str(),"w");
					break;
				case 2:
					patcherFile = yatc_fopen((patcherFN="part.spr").c_str(),"w");
					break;
				case 3:
					patcherFile = yatc_fopen((patcherFN="part.pic").c_str(),"w");
					break;
				case 4:
					patcherFile = yatc_fopen((patcherFN="part.patch.exe").c_str(),"w");
					break;
				case 5:
					patcherFile = yatc_fopen((patcherFN="part.showerror.elf").c_str(),"w");
					break;
			}
			
			Notifications::openMessageWindow(MESSAGE_ERROR, std::string() + "Patching part: " + patcherFN);
			printf("Patching part: %02x %08x\n", p_type, p_size);

			
			
			break;
		}
		case 0x1F: // Patching full?
		{
			MSG_READ_U8(p_type);
			MSG_READ_U32(p_size);
			patcherSoFar = 0;
			patcherTotal = p_size;
			switch(p_type)
			{
				case 0:
					patcherFile = yatc_fopen((patcherFN="full.exe").c_str(),"w");
					break;
				case 1:
					patcherFile = yatc_fopen((patcherFN="full.dat").c_str(),"w");
					break;
				case 2:
					patcherFile = yatc_fopen((patcherFN="full.spr").c_str(),"w");
					break;
				case 3:
					patcherFile = yatc_fopen((patcherFN="full.pic").c_str(),"w");
					break;
				case 4:
					patcherFile = yatc_fopen((patcherFN="full.patch.exe").c_str(),"w");
					break;
				case 5:
					patcherFile = yatc_fopen((patcherFN="full.showerror.elf").c_str(),"w");
					break;
			}
			Notifications::openMessageWindow(MESSAGE_ERROR, std::string()+"Patching full: " + patcherFN);
			printf("Patching full: %02x %08x\n", p_type, p_size);
			break;
			
		}
		case 0x20: // Patching blob
		{
			int sz = msg.getU16();
			int from = 0, to = sz;
			const char* memory = msg.getReadBuffer();
			printf("Patch size %d (%08x)\n", sz, sz);
			
			//printf("Begin: %02x %02x %02x\n", memory[0], memory[1], memory[2]);

			if(patcherFile)
				fwrite(memory, sz, 1, patcherFile);
			else
				printf("Dropping bytes\n");
			msg.skipBytes(sz);
			
			patcherSoFar+=sz;
			if(patcherSoFar>=patcherTotal)
			{
				fclose(patcherFile);
				patcherFile=NULL;
			}
			
			//MSG_READ_STRING(p_unk);
			std::stringstream progress;
			progress << "Patching " << patcherFN << ": " << patcherSoFar << "/" << patcherTotal << "(" << (patcherSoFar*100/patcherTotal) << "%)";
			Notifications::openMessageWindow(MESSAGE_ERROR, progress.str());
			printf("Patching file - blob\n");

			
			break;
		}
		//case 0x1E: //Patching dat/pic/spr messages
		//case 0x1F:
		//case 0x20:
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
void ProtocolLogin::sendSystemConfiguration(NetworkMessage &output)
{
    // decoded by thomac, praise the Lord

    // FIXME (ivucica#1#): this sends thomac's configuration.
    // this can easily be detected.
    // we need to figure out remaining bytes and real detection
    char hardware_spec[] = {
        0x2C, // unknown
        'U', 'S', 'A', // USA - locale
        0xFE, 0x1F, // 8190 - RAM

        0x04, 0x5E, 0x08, 0x40, 0x10, 0x00,  // unknown

        '9', '6', '5', '0', '\0', 0x00, 0x00, 0x00, 0x00, // 9650 - processor, padded to 9 bytes

        0x11, 0x44, // unknown

        0x06, 0x09, // proc freq
        0x06, 0x09, // proc freq (?)
        0x22, 0x00, 0x00, 0x00, // unknown

        '4', '8', '0', '0', ' ', 'S', 'E', 'R', 'I', // 4800 SERI - gfx card padded to 9 bytes
        0x00, 0x04, // 1024 - vram?
        0x80, 0x07, // 1920 - horiz reso
        0x38, 0x04, // 1080 - vert reso,
        0x3C // refresh rate
    };
    output.addRaw(hardware_spec, sizeof(hardware_spec));
}
