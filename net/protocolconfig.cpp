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

#include "../debugprint.h"
#include "protocolconfig.h"
#include "../util.h"
#include "protocolgame.h"
#include "rsa.h"

#include "protocollogin.h"
#include "protocolgame74.h"
#include "protocolgame76.h"
#include "protocolgame78.h"
#include "protocolgame80.h"
#include "protocolgame81.h"
#include "protocolgame811.h"
#include "protocolgame82.h"
#include "protocolgame821.h"
#include "protocolgame822.h"
#include "protocolgame83.h"
#include "protocolgame831.h"
#include "protocolgame84.h"
#include "protocolgame841.h"
#include "protocolgame842.h"
#include "protocolgame85.h"
#include "protocolgame853.h"
#include "protocolgame854.h"

const char RSAKey_otserv[] = "109120132967399429278860960508995541528237502902798129123468757937266291492576446330739696001110603907230888610072655818825358503429057592827629436413108566029093628212635953836686562675849720620786279431090218017681061521755056710823876476444260558147179707119674283982419152118103759076030616683978566631413";
const char RSAKey_cip[]    = "124710459426827943004376449897985582167801707960697037164044904862948569380850421396904597686953877022394604239428185498284169068581802277612081027966724336319448537811441719076484340922854929273517308661370727105382899118999403808045846444647284499123164879035103627004668521005328367415259939915284902061793";

extern Connection* g_connection;

ProtocolConfig::ProtocolConfig()
{
	m_host = "localhost";
	m_port = 7171;
	setVersion(CLIENT_OS_WIN, CLIENT_VERSION_830);
	setServerType(SERVER_OTSERV);
	setVersionOverride(0); // no override
}

uint32_t ProtocolConfig::readSignature(const char* fn) {
	uint32_t sig;
	FILE*f=yatc_fopen(fn,"rb");
	ASSERTFRIENDLY(f,"Could not find a data file although we found it in the past.");
	yatc_fread(&sig, 4, 1, f);
	fclose(f);
	return sig;
}

void ProtocolConfig::setVersion(ClientOS_t os, ClientVersion_t version)
{
	m_os = os;
	switch(version){
    case CLIENT_VERSION_740:
		NativeGUIError("Support for this client version is only experimental.", "Warning");
		m_clientVersion = CLIENT_VERSION_740;
        break;
    case CLIENT_VERSION_760:
		NativeGUIError("Support for this client version is only experimental.", "Warning");
		m_clientVersion = CLIENT_VERSION_760;
        break;
	case CLIENT_VERSION_780:
		NativeGUIError("Support for this client version is only experimental.", "Warning");
		m_clientVersion = CLIENT_VERSION_780;
		break;
	case CLIENT_VERSION_800:
		m_clientVersion = CLIENT_VERSION_800;
		break;
	case CLIENT_VERSION_810:
		m_clientVersion = CLIENT_VERSION_810;
		break;
	case CLIENT_VERSION_811:
		m_clientVersion = CLIENT_VERSION_811;
		break;
	case CLIENT_VERSION_820:
		m_clientVersion = CLIENT_VERSION_820;
		break;
	case CLIENT_VERSION_821:
		m_clientVersion = CLIENT_VERSION_821;
		break;
	case CLIENT_VERSION_822:
		m_clientVersion = CLIENT_VERSION_822;
		break;
	case CLIENT_VERSION_830:
		m_clientVersion = CLIENT_VERSION_830;
		break;
	case CLIENT_VERSION_831:
		m_clientVersion = CLIENT_VERSION_831;
		break;
	case CLIENT_VERSION_840:
		m_clientVersion = CLIENT_VERSION_840;
		break;
	case CLIENT_VERSION_841:
		m_clientVersion = CLIENT_VERSION_841;
		break;
	case CLIENT_VERSION_842:
		m_clientVersion = CLIENT_VERSION_842;
		break;
	case CLIENT_VERSION_850:
		m_clientVersion = CLIENT_VERSION_850;
		break;
	case CLIENT_VERSION_853:
		m_clientVersion = CLIENT_VERSION_853;
		break;
	case CLIENT_VERSION_854:
		m_clientVersion = CLIENT_VERSION_854;
		break;
	case CLIENT_VERSION_AUTO:
		m_clientVersion = detectVersion();
		break;
	default:
		//ASSERT(0);
		NativeGUIError("This client version is not currently supported.", "Sorry");
		return;
	}
	m_datSignature = this->readSignature("Tibia.dat");
	m_sprSignature = this->readSignature("Tibia.spr");
	m_picSignature = this->readSignature("Tibia.pic");
}
ClientVersion_t ProtocolConfig::detectVersion()
{
	uint32_t datSignature = ProtocolConfig::readSignature("Tibia.dat");
	uint32_t sprSignature = ProtocolConfig::readSignature("Tibia.spr");
	uint32_t picSignature = ProtocolConfig::readSignature("Tibia.pic");

	printf("Data file signatures: %08x %08x %08x\n", datSignature, sprSignature, picSignature);
	//todo (nfries88): Client version 74 autodetect
	//todo (nfries88): Client version 76 and 77 series autodetect
	//todo (nfries88): Client version 78 series autodetect
	if (datSignature == 0x439D5A33 &&
		sprSignature == 0x439852BE &&
		picSignature == 0x4450C8D8)
			return CLIENT_VERSION_790;
	//todo (nfries88): Client version 791 autodetect
	if (datSignature == 0x459E7B73 &&
		sprSignature == 0x45880FE8 &&
		picSignature == 0x45670923)
			return CLIENT_VERSION_792;

	if (datSignature == 0x467FD7E6 &&
		sprSignature == 0x467F9E74 &&
		picSignature == 0x45670923)
			return CLIENT_VERSION_800;

	if (datSignature == 0x475D3747 &&
		sprSignature == 0x475D0B01 &&
		picSignature == 0x4742FCD7)
	        return CLIENT_VERSION_810;

	if (datSignature == 0x47F60E37 &&
		sprSignature == 0x47EBB9B2 &&
		picSignature == 0x4742FCD7)
	        return CLIENT_VERSION_811;

	if (datSignature == 0x486905AA &&  // 8.20 minipatch uses datafiles from 8.21
		sprSignature == 0x4868ECC9 &&
		picSignature == 0x48562106)
		return CLIENT_VERSION_820;

	if (datSignature == 0x486CCA2B &&
		sprSignature == 0x4868ECC9 &&
		picSignature == 0x48562106)
		return CLIENT_VERSION_821;

    if (datSignature == 0x489980a1 &&
        sprSignature == 0x489980a5 &&
        picSignature == 0x48562106)
        return CLIENT_VERSION_822;

	// 8.30 == 8.31; prefer 8.31
    if (datSignature == 0x48da1fb6 &&
        sprSignature == 0x48c8e712 &&
        picSignature == 0x48562106)
        return CLIENT_VERSION_831;

    if (datSignature == 0x493d607a &&
        sprSignature == 0x493d4e7c &&
        picSignature == 0x49144178)
        return CLIENT_VERSION_840;

    if (datSignature == 0x49b7cc19 &&
        sprSignature == 0x49b140ea &&
        picSignature == 0x49144178)
        return CLIENT_VERSION_841;

    if (datSignature == 0x49c233c9 &&
        sprSignature == 0x49b140ea &&
        picSignature == 0x49144178)
        return CLIENT_VERSION_842;

    if ((datSignature == 0x4A49C5EB || datSignature == 0x4a4cc0dc) && // verbatim release, and the minipatch
        sprSignature == 0x4A44FD4E &&
        picSignature == 0x4A2FA8D6)
        return CLIENT_VERSION_850;


    if (datSignature == 0x4AE97492  &&
        sprSignature == 0x4ACB5230  &&
        picSignature == 0x4AE5C3D3)
        return CLIENT_VERSION_853;

    if ((datSignature == 0x4b28b89e || datSignature == 0x4b1e2caa)  &&
        sprSignature == 0x4B1E2C87  &&
        picSignature == 0x4AE5C3D3)
        return CLIENT_VERSION_854;

	return CLIENT_VERSION_AUTO; // failure
}
void ProtocolConfig::setVersionOverride(uint16_t version) {
    // this means that we'll just send a different version to the server
    // dat, spr and pic are read from file anyways
    m_overrideVersion = version;
}

void ProtocolConfig::setServerType(ServerType_t type)
{
	switch(type){
	case SERVER_OTSERV:
		RSA::getInstance()->setPublicKey(RSAKey_otserv, "65537");
		break;
	case SERVER_CIP:
		RSA::getInstance()->setPublicKey(RSAKey_cip, "65537");
		break;
	default:
		break;
	}
}

void ProtocolConfig::createLoginConnection(const std::string& accountname, const std::string& password)
{
	ASSERT(g_connection == NULL);

	EncXTEA* crypto = new EncXTEA;
	ProtocolLogin* protocol = new ProtocolLogin(accountname, password);
	g_connection = new Connection(getInstance().m_host, getInstance().m_port, crypto, protocol);
	if(getInstance().m_clientVersion<CLIENT_VERSION_830){
		g_connection->setChecksumState(false);
		protocol->usesAccountName(false);
	} else {
		g_connection->setChecksumState(true);
		protocol->usesAccountName(true);

		if (getInstance().m_clientVersion>=CLIENT_VERSION_850)
		{
            protocol->setSendSystemConfiguration(true);
		}
	}
}

ProtocolGame* ProtocolConfig::createGameProtocol(int version, const std::string&accountname, const std::string&password, const std::string&name, bool isGM)
{
#if !LOGIN_ONLY
    ProtocolGame* protocol;
    switch(version){
    case CLIENT_VERSION_740:
        protocol = new ProtocolGame74(accountname, password, name, isGM);
        break;
    case CLIENT_VERSION_760:
        protocol = new ProtocolGame76(accountname, password, name, isGM);
        break;
    case CLIENT_VERSION_780:
		protocol = new ProtocolGame78(accountname, password, name, isGM);
		break;
	// todo (nfries88): more client version protocols.
	case CLIENT_VERSION_800:
		protocol = new ProtocolGame80(accountname, password, name, isGM);
		break;
	case CLIENT_VERSION_810:
		protocol = new ProtocolGame81(accountname, password, name, isGM);
		break;
	case CLIENT_VERSION_811:
		protocol = new ProtocolGame811(accountname, password, name, isGM);
		break;
	case CLIENT_VERSION_820:
		protocol = new ProtocolGame82(accountname, password, name, isGM);
		break;
	case CLIENT_VERSION_821:
		protocol = new ProtocolGame821(accountname, password, name, isGM);
		break;
	case CLIENT_VERSION_822:
 		protocol = new ProtocolGame822(accountname, password, name, isGM);
		break;
	case CLIENT_VERSION_830:
		protocol = new ProtocolGame83(accountname, password, name, isGM);
		break;
	case CLIENT_VERSION_831:
		protocol = new ProtocolGame831(accountname, password, name, isGM);
		break;
	case CLIENT_VERSION_840:
		protocol = new ProtocolGame84(accountname, password, name, isGM);
		break;
	case CLIENT_VERSION_841:
		protocol = new ProtocolGame841(accountname, password, name, isGM);
		break;
	case CLIENT_VERSION_842:
		protocol = new ProtocolGame842(accountname, password, name, isGM);
		break;
	case CLIENT_VERSION_850:
		protocol = new ProtocolGame85(accountname, password, name, isGM);
		break;
	case CLIENT_VERSION_853:
		protocol = new ProtocolGame853(accountname, password, name, isGM);
		break;
	case CLIENT_VERSION_854:
		protocol = new ProtocolGame854(accountname, password, name, isGM);
		break;
	default:
                DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_ERROR, "createGameProtocol: Unknown protocol version passed\n");
		return NULL;
		break;
	}
	return protocol;
#else
        DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_WARNING, "createGameProtocol: cannot do this, LOGIN_ONLY has been #defined\n");

	return NULL;
#endif
}

ProtocolGame* ProtocolConfig::createGameConnection(const std::string& accountname, const std::string& password, const std::string& name, bool isGM)
{
	ASSERT(g_connection == NULL);

        DEBUGPRINT(DEBUGPRINT_LEVEL_USEFUL, DEBUGPRINT_NORMAL, "createGameConnection: creating a game protocol\n");

	ProtocolGame* protocol = createGameProtocol(getInstance().m_clientVersion,accountname,password,name,isGM);
	if (protocol == NULL) {
                DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_WARNING, "createGameConnection: protocol returned from createGameProtocol is null\n");
                return NULL;
        }

        DEBUGPRINT(DEBUGPRINT_LEVEL_USEFUL, DEBUGPRINT_NORMAL, "createGameConnection: creating crypto object\n");

	EncXTEA* crypto = new EncXTEA;

        DEBUGPRINT(DEBUGPRINT_LEVEL_USEFUL, DEBUGPRINT_NORMAL, "createGameConnection: creating a game connection\n");

	g_connection = new Connection(getInstance().m_host, getInstance().m_port, crypto, protocol);

	if(getInstance().m_clientVersion >= CLIENT_VERSION_830){
		g_connection->setChecksumState(true);
		protocol->usesAccountName(true);
	}

	return protocol;
}
