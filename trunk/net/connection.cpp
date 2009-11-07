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

#include <SDL/SDL.h>

#ifdef WINCE
#include <windows.h>
#include "../util.h"
#endif

#include "connection.h"
#include "encryption.h"
#include "../notifications.h"

#include "rsa.h"
#include "protocollogin.h"
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
#include "../debugprint.h"
#include "../util.h" // yatc_fopen

#ifndef min
#include <algorithm> // std::min
#define min std::min
#endif

const char RSAKey_otserv[] = "109120132967399429278860960508995541528237502902798129123468757937266291492576446330739696001110603907230888610072655818825358503429057592827629436413108566029093628212635953836686562675849720620786279431090218017681061521755056710823876476444260558147179707119674283982419152118103759076030616683978566631413";
const char RSAKey_cip[]    = "124710459426827943004376449897985582167801707960697037164044904862948569380850421396904597686953877022394604239428185498284169068581802277612081027966724336319448537811441719076484340922854929273517308661370727105382899118999403808045846444647284499123164879035103627004668521005328367415259939915284902061793";

extern Connection* g_connection;
extern std::string g_recordfilename;

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
	case CLIENT_VERSION_AUTO:
		m_clientVersion = detectVersion();
		break;
	default:
		//ASSERT(0);
		NativeGUIError("This client version is not currently supported.", "Sorry");
		return;
		break;
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

    if (datSignature == 0x4A49C5EB || datSignature == 0x4a4cc0dc && // verbatim release, and the minipatch
        sprSignature == 0x4A44FD4E &&
        picSignature == 0x4A2FA8D6)
        return CLIENT_VERSION_850;


    if (datSignature == 0x4AE97492  &&
        sprSignature == 0x4ACB5230  &&
        picSignature == 0x4AE5C3D3)
        return CLIENT_VERSION_853;


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
    ProtocolGame* protocol;
    switch(version){
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
	default:
		return NULL;
		break;
	}
	return protocol;
}

ProtocolGame* ProtocolConfig::createGameConnection(const std::string& accountname, const std::string& password, const std::string& name, bool isGM)
{
	ASSERT(g_connection == NULL);

	ProtocolGame* protocol = createGameProtocol(getInstance().m_clientVersion,accountname,password,name,isGM);
	if (protocol == NULL)
        return NULL;

	EncXTEA* crypto = new EncXTEA;

	g_connection = new Connection(getInstance().m_host, getInstance().m_port, crypto, protocol);

	if(getInstance().m_clientVersion >= CLIENT_VERSION_830){
		g_connection->setChecksumState(true);
		protocol->usesAccountName(true);
	}

	return protocol;
}

//****************************************************

void Protocol::addServerCmd(uint8_t type)
{
	if(m_lastServerCmd.size() >= 10){
		m_lastServerCmd.pop_back();
	}
	m_lastServerCmd.push_front(type);
}

//*****************************************************

const char* Connection::getErrorDesc(int message)
{
	switch(message){
	case ERROR_CANNOT_RESOLVE_HOST:
		return "Cannot resolve host.";
	case ERROR_WRONG_HOST_ADDR_TYPE:
		return "Host uses an unsupported address type.";
	case ERROR_CANNOT_CREATE_SOCKET:
		return "Could not create socket.";
	case ERROR_CANNOT_SET_NOBLOCKING_SOCKET:
		return "Could not create a non-blocking socket.";
	case ERROR_CANNOT_CONNECT:
		return "Cannot connect.";
	case ERROR_CONNECT_TIMEOUT:
		return "Connection timed out.";
	case ERROR_SELECT_FAIL_CONNECTED:
		return "Failed call to select() while connected.";
	case ERROR_SELECT_FAIL_CONNECTING:
		return "Failed call to select() while connecting.";
	case ERROR_UNSUCCESSFUL_CONNECTION:
		return "Could not connect to server. Is the server running?";
	case ERROR_GETSOCKTOPT_FAIL:
		return "Failed call to getsockopt().";
	case ERROR_UNEXPECTED_SELECT_RETURN_VALUE:
		return "Unexpected return value from select()";
	case ERROR_CANNOT_GET_PENDING_SIZE:
		return "Could not get pending size.";
	case ERROR_RECV_FAIL:
		return "Failed call to recv()";
	case ERROR_UNEXPECTED_RECV_ERROR:
		return "Unexpected recv() error.";
	case ERROR_DECRYPT_FAIL:
		return "Decrypting of message failed.\nIn Network options, try changing option \"OT Key\".";
	case ERROR_WRONG_MSG_SIZE:
		return "Message size is wrong.";
	case ERROR_SEND_FAIL:
		return "Failed call to send()";
	case ERROR_PROTOCOL_ONRECV:
		return "Failed to parse received message.";
	case ERROR_CLOSED_SOCKET:
		return "Socket has been closed.";
	case ERROR_TOO_BIG_MESSAGE:
		return "Message is too big.";
	default:
		return "Error without description.";
	}
}

Connection::Connection(const std::string& host, uint16_t port, Encryption* crypto, Protocol* protocol) :
m_inputMessage(NetworkMessage::CAN_READ)
{
	m_host = host;
	m_ip = 0;
	m_port = port;

	m_crypto = crypto;
	m_protocol = protocol;
	m_protocol->setConnection(this);

	m_state = STATE_INIT;
	m_readState = READING_SIZE;
	m_msgSize = 0;
	m_cryptoEnable = false;
	m_checksumEnable = false;

	m_socket = INVALID_SOCKET;
	m_ticks = 0;

	m_sentBytes = 0;
	m_recvBytes = 0;

	m_recordstart=0;
	m_recordfile=NULL;

}

Connection::~Connection()
{
	closeConnection();
	delete m_protocol;
	delete m_crypto;
}

void Connection::callCallback(int message)
{
	Notifications::onConnectionError(message);
}

int Connection::getSocketError()
{
	#ifdef WIN32
	return WSAGetLastError();
	#else
	return errno;
	#endif
}

void Connection::closeConnection()
{
	if(m_socket != INVALID_SOCKET){
		shutdown(m_socket, SD_BOTH);
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	}
	m_state = STATE_CLOSED;
	if(m_recordfile)
	{
	    fclose(m_recordfile);
	    m_recordstart=0;
	    m_recordfile=NULL;
	}
}

void Connection::executeNetwork()
{
	switch(m_state){
	case STATE_INIT:
	{
		//Resolve host
		m_ip = inet_addr(m_host.c_str());
		if(m_ip == INADDR_NONE){
			struct hostent* hp = gethostbyname(m_host.c_str());
			if(hp != NULL){
				if(hp->h_addrtype == AF_INET){
					//only are supported ipv4 addr
					m_ip = *(uint32_t*)hp->h_addr_list[0];
				}
				else{
					closeConnectionError(ERROR_WRONG_HOST_ADDR_TYPE);
					return;
				}
			}
			else{
				closeConnectionError(ERROR_CANNOT_RESOLVE_HOST);
				return;
			}
		}
		//Create a TCP socket
		m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		#ifdef WIN32
		if(m_socket == INVALID_SOCKET){
            closeConnectionError(ERROR_CANNOT_CREATE_SOCKET);
			return;
		}
		#else
		if(m_socket <= 0){
			m_socket = INVALID_SOCKET;
            closeConnectionError(ERROR_CANNOT_CREATE_SOCKET);
			return;
		}
		#endif


		//Set non-blocking socket
		#ifdef WIN32
		unsigned long mode = 1;
		if(ioctlsocket(m_socket, FIONBIO, &mode) == SOCKET_ERROR){
			closeConnectionError(ERROR_CANNOT_SET_NOBLOCKING_SOCKET);
			return;
		}
		#else
		if(fcntl(m_socket, F_SETFL, O_NONBLOCK) == -1){
			closeConnectionError(ERROR_CANNOT_SET_NOBLOCKING_SOCKET);
			return;
		}
		#endif

		//Reset traffic counter
		m_sentBytes = 0;
		m_recvBytes = 0;

		//And connect
		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = m_ip;
		addr.sin_port = htons(m_port);

		int ret = connect(m_socket, (sockaddr*)&addr, sizeof(addr));
		if(ret == SOCKET_ERROR && getSocketError() != CONNECT_WOULD_BLOCK){
			closeConnectionError(ERROR_CANNOT_CONNECT);
			return;
		}
		else if(ret == 0){
			//connection succeeds
			m_state = STATE_CONNECTED;
		}
		else{
			//waiting non blocking connect
			m_state = STATE_CONNECTING;
			m_ticks = SDL_GetTicks();
		}

		break;
	}
	case STATE_CONNECTING:
	{
		//Check socket state
		timeval tv =  {0, 0}; //non-blocking select
		fd_set write_set;
		FD_ZERO(&write_set);
		FD_SET(m_socket, &write_set);

		int ret = select(m_socket + 1, NULL, &write_set, NULL, &tv);
		if(ret == 0){
			//time expired, socket not connected yet
			if(SDL_GetTicks() - m_ticks > 20*1000){
				//waiting 20 seconds? -> timeout
				closeConnectionError(ERROR_CONNECT_TIMEOUT);
			}
		}
		else if(ret == 1 && FD_ISSET(m_socket, &write_set)){
#ifndef WINCE
			//Check if it was a successful connection
			int optError;
			optlen_t optErrorLen = sizeof(optError);
			int ret = getsockopt(m_socket, SOL_SOCKET, SO_ERROR, (opt_t*)&optError, &optErrorLen);
			if(ret != SOCKET_ERROR && optError == 0){
				//connection succeeded
				m_state = STATE_CONNECTED;

				// record starts now
				if(g_recordfilename.size())
				{
                    m_recordstart=SDL_GetTicks();
                    m_recordfile=yatc_fopen(g_recordfilename.c_str(),"wb");
				}

				//raise onConnect event
                m_protocol->onConnect();
			}
			else if(ret != SOCKET_ERROR && optError != 0){
				//connection failed
				closeConnectionError(ERROR_UNSUCCESSFUL_CONNECTION);
			}
			else{
				//call to getsockopt failed
				closeConnectionError(ERROR_GETSOCKTOPT_FAIL);
			}
#else
			// Since Windows Mobile 2003 (WINCE4.20) seems to set 120 in WSAGetLastError() which stands for something
			// like CALLNOTIMPLEMENTED we'll just stub this to successful connection and pray it works ;)
			DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "WINCE Connection\n");
			m_state = STATE_CONNECTED;
			m_protocol->onConnect();
#endif
		}
		else if(ret == SOCKET_ERROR){
			//select failed
			closeConnectionError(ERROR_SELECT_FAIL_CONNECTING);
		}
		else{
			//should not reach this point
			closeConnectionError(ERROR_UNEXPECTED_SELECT_RETURN_VALUE);
		}

		break;
	}
	case STATE_CONNECTED:
	{
		//Try to read messages
		while(m_state == STATE_CONNECTED && getPendingInput() > 0){
			switch(m_readState){
				case READING_SIZE:
				{
					int ret = internalRead(2, true);
					if(ret != 2){
						checkSocketReadState();
						return;
					}
					if(!m_inputMessage.getU16(m_msgSize)){
						printf("Failed reading msg size\n");
						closeConnectionError(ERROR_UNEXPECTED_RECV_ERROR);
						return;
					}
					if(m_msgSize > NETWORK_MESSAGE_SIZE){
						closeConnectionError(ERROR_TOO_BIG_MESSAGE);
						return;
					}
				}
				case READING_CHECKSUM: {
					if(m_checksumEnable){
						m_readState = READING_CHECKSUM;
						int ret = internalRead(4, true);
						if(ret != 4){
							checkSocketReadState();
							return;
						}
						uint32_t checksum;
						if(!m_inputMessage.getU32(checksum)){
							printf("Failed reading checksum\n");
							closeConnectionError(ERROR_UNEXPECTED_RECV_ERROR);
							return;
						} else {
							m_msgSize-=4;
							m_readState = READING_MESSAGE;
						}
					} else
						m_readState = READING_MESSAGE;
				}
				case READING_MESSAGE:
				{
					int ret = internalRead(m_msgSize, false);
					if(ret <= 0){
						checkSocketReadState();
						return;
					}
					else if(ret != m_msgSize){
						m_msgSize -= ret;
						checkSocketReadState();

						return;
					}

					//decrypt incoming message if needed
					if(m_cryptoEnable && m_crypto){
						if(!m_crypto->decrypt(m_inputMessage)){
							closeConnectionError(ERROR_DECRYPT_FAIL);
							return;
						}
					} else {
                        // this is 8.41 specific
                        // data in unencrypted packets is now including unencrypted length,
                        // + encrypted length ... although packet is not encrypted
                        // let's just cut the cr/\p
                        /*uint16_t size = */m_inputMessage.getU16();
					}

					if(m_recordfile)
					{
					    uint32_t timestamp=SDL_GetTicks()-m_recordstart;
					    uint16_t size=m_inputMessage.getReadSize();

					    fwrite(&timestamp, 4, 1, m_recordfile);
					    fwrite(&size,2,1,m_recordfile);
					    fwrite(m_inputMessage.getReadBuffer(), m_inputMessage.getReadSize(), 1, m_recordfile);

					}


					//raise onRecv event
					if(!m_protocol->onRecv(m_inputMessage)){
						closeConnectionError(ERROR_PROTOCOL_ONRECV);
						return;
					}
					//resets input message state
					m_readState = READING_SIZE;
					m_inputMessage.reset();
					break;
				}
			}
		}
		checkSocketReadState();
		break;
	}
	case STATE_CLOSED:
	case STATE_ERROR:
		//nothing to do
		break;
	}
}

void Connection::closeConnectionError(int error)
{
	closeConnection();
	m_state = STATE_ERROR;
	callCallback(error);
}

unsigned long Connection::getPendingInput()
{
	#ifdef WIN32
	unsigned long size = 0;
	if(ioctlsocket(m_socket, FIONREAD, &size) == SOCKET_ERROR){
		closeConnectionError(ERROR_CANNOT_GET_PENDING_SIZE);
		return 0;
	}
	#else
	int size;
	if(ioctl(m_socket, FIONREAD, &size) == -1){
		closeConnectionError(ERROR_CANNOT_GET_PENDING_SIZE);
		return 0;
	}
	#endif
	return size;
}

void Connection::checkSocketReadState()
{
	if(m_state != STATE_CONNECTED)
		return;

	timeval tv =  {0, 0}; //non-blocking select
	fd_set read_set;
	FD_ZERO(&read_set);
	FD_SET(m_socket, &read_set);

	int ret = select(m_socket + 1, &read_set, NULL, NULL, &tv);
	if(ret == 1 && getPendingInput() == 0){
		closeConnectionError(ERROR_CLOSED_SOCKET);
	}
	else if(ret == SOCKET_ERROR){
		closeConnectionError(ERROR_SELECT_FAIL_CONNECTED);
	}
}

int Connection::internalRead(unsigned int n, bool all)
{
	unsigned long bytesToRead = getPendingInput();
	//If all is set check that we can read n bytes
	if(all && bytesToRead < n){
		return 0;
	}
	//we will read a max of n bytes
	if(bytesToRead > n){
		bytesToRead = n;
	}

	//read them
	socketret_t ret = recv(m_socket,
		m_inputMessage.getReadBuffer() + m_inputMessage.getReadSize(),
		bytesToRead, 0);

	if(ret != SOCKET_ERROR && ret == (int)bytesToRead){
		//we have read n bytes, so we resize inputMessage
		m_inputMessage.setReadSize(m_inputMessage.getReadSize() + bytesToRead);
		m_recvBytes+=bytesToRead;
		return bytesToRead;
	}
	else if(ret == 0){
		//peer has performed an orderly shutdown
		return 0;
	}
	else{
		if(getSocketError() == EWOULDBLOCK){
			return 0;
		}
		closeConnectionError(ERROR_RECV_FAIL);
		return -1;
	}
}

void Connection::sendMessage(NetworkMessage& msg)
{
	if(m_state != STATE_CONNECTED){
		DEBUGPRINT(DEBUGPRINT_ERROR, DEBUGPRINT_LEVEL_OBLIGATORY, "Calling send when state != STATE_CONNECTED(state = %d)\n", m_state);
		return;
	}

	if(msg.getSize() == 0){
		DEBUGPRINT(DEBUGPRINT_WARNING, DEBUGPRINT_LEVEL_OBLIGATORY, "Sending size = 0 in message\n");
		return;
	}

	//add message size
	msg.addHeader();

	//and encrypt if needed
	if(m_cryptoEnable && m_crypto){
		m_crypto->encrypt(msg);
	}


	//and add checksum if needed
	if(m_checksumEnable){
		msg.addChecksum();
	}



	//wait until all bytes are sent
	int sendBytes = 0;
	do{
		socketret_t b = send(m_socket, msg.getBuffer() + sendBytes, min(msg.getSize() - sendBytes, 1000), 0);
		if(b <= 0){
			closeConnectionError(ERROR_SEND_FAIL);
			return;
		}
		sendBytes += b;
	} while(sendBytes < msg.getSize());
	m_sentBytes += sendBytes;
}
