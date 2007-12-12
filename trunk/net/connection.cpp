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
#include "notifications.h"

#include "rsa.h"
#include "protocollogin.h"
#include "protocolgame80.h"
#include "../debugprint.h"

const char RSAKey_otserv[] = "109120132967399429278860960508995541528237502902798129123468757937266291492576446330739696001110603907230888610072655818825358503429057592827629436413108566029093628212635953836686562675849720620786279431090218017681061521755056710823876476444260558147179707119674283982419152118103759076030616683978566631413";
const char RSAKey_cip[]    = "124710459426827943004376449897985582167801707960697037164044904862948569380850421396904597686953877022394604239428185498284169068581802277612081027966724336319448537811441719076484340922854929273517308661370727105382899118999403808045846444647284499123164879035103627004668521005328367415259939915284902061793";

extern Connection* g_connection;

ProtocolConfig::ProtocolConfig()
{
	m_host = "localhost";
	m_port = 7171;
	setVersion(CLIENT_OS_WIN, CLIENT_VERSION_800);
	setServerType(SERVER_OTSERV);
	setVersionOverride(0); // no override
}

uint32_t ProtocolConfig::readSignature(const char* fn) {
    FILE*f=fopen(fn,"rb");
    uint32_t sig;
    fread(&sig, 4, 1, f);
    return sig;
}

void ProtocolConfig::setVersion(ClientOS_t os, ClientVersion_t version)
{
	m_os = os;
	switch(version){
	case CLIENT_VERSION_800:
		m_clientVersion = CLIENT_VERSION_800;

		// ivucica: imho this is now read from pic, dat and spr files and fed to protocolconfig another way
		//m_datSignature = 0x467FD7E6;
		//m_sprSignature = 0x467F9E74;
		//m_picSignature = 0x45670923;

		break;
	default:
		ASSERT(0);
		break;
	}
	m_datSignature = this->readSignature("Tibia.dat");
	m_sprSignature = this->readSignature("Tibia.spr");
	m_picSignature = this->readSignature("Tibia.pic");
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

void ProtocolConfig::createLoginConnection(int account, const std::string& password)
{
	ASSERT(g_connection == NULL);

	EncXTEA* crypto = new EncXTEA;
	Protocol* protocol = new ProtocolLogin(account, password);
	g_connection = new Connection(getInstance().m_host, getInstance().m_port, crypto, protocol);
}

ProtocolGame* ProtocolConfig::createGameConnection(int account, const std::string& password, const std::string& name, bool isGM)
{
	ASSERT(g_connection == NULL);

	ProtocolGame* protocol;
	switch(getInstance().m_clientVersion){
	case CLIENT_VERSION_800:
		protocol = new ProtocolGame80(account, password, name, isGM);
		break;
	default:
		return NULL;
		break;
	}

	EncXTEA* crypto = new EncXTEA;

	g_connection = new Connection(getInstance().m_host, getInstance().m_port, crypto, protocol);

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
		return "ERROR_CANNOT_RESOLVE_HOST";
	case ERROR_WRONG_HOST_ADDR_TYPE:
		return "ERROR_WRONG_HOST_ADDR_TYPE";
	case ERROR_CANNOT_CREATE_SOCKET:
		return "ERROR_CANNOT_CREATE_SOCKET";
	case ERROR_CANNOT_SET_NOBLOCKING_SOCKET:
		return "ERROR_CANNOT_SET_NOBLOCKING_SOCKET";
	case ERROR_CANNOT_CONNECT:
		return "ERROR_CANNOT_CONNECT";
	case ERROR_CONNECT_TIMEOUT:
		return "ERROR_CONNECT_TIMEOUT";
	case ERROR_SELECT_FAIL_CONNECTED:
		return "ERROR_SELECT_FAIL_CONNECTED";
	case ERROR_SELECT_FAIL_CONNECTING:
		return "ERROR_SELECT_FAIL_CONNECTING";
	case ERROR_UNSUCCESSFUL_CONNECTION:
		return "ERROR_UNSUCCESSFUL_CONNECTION";
	case ERROR_GETSOCKTOPT_FAIL:
		return "ERROR_GETSOCKTOPT_FAIL";
	case ERROR_UNEXPECTED_SELECT_RETURN_VALUE:
		return "ERROR_UNEXPECTED_SELECT_RETURN_VALUE";
	case ERROR_CANNOT_GET_PENDING_SIZE:
		return "ERROR_CANNOT_GET_PENDING_SIZE";
	case ERROR_RECV_FAIL:
		return "ERROR_RECV_FAIL";
	case ERROR_UNEXPECTED_RECV_ERROR:
		return "ERROR_UNEXPECTED_RECV_ERROR";
	case ERROR_DECRYPT_FAIL:
		return "ERROR_DECRYPT_FAIL";
	case ERROR_WRONG_MSG_SIZE:
		return "ERROR_WRONG_MSG_SIZE";
	case ERROR_SEND_FAIL:
		return "ERROR_SEND_FAIL";
	case ERROR_PROTOCOL_ONRECV:
		return "ERROR_PROTOCOL_ONRECV";
	case ERROR_CLOSED_SOCKET:
		return "ERROR_CLOSED_SOCKET";
	case ERROR_TOO_BIG_MESSAGE:
		return "ERROR_TOO_BIG_MESSAGE";
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

	m_socket = INVALID_SOCKET;
	m_ticks = 0;
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
		#else
		if(m_socket <= 0){
			m_socket = INVALID_SOCKET;
		#endif
			closeConnectionError(ERROR_CANNOT_CREATE_SOCKET);
			return;
		}

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
				//waitnig 20 seconds? -> timeout
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
			DEBUGPRINT(DEBUGPRINT_NORMAL, DEBUGPRINT_LEVEL_OBLIGATORY, "WINCE Connection\n");
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
						closeConnectionError(ERROR_UNEXPECTED_RECV_ERROR);
						return;
					}
					if(m_msgSize > NETWORK_MESSAGE_SIZE){
						closeConnectionError(ERROR_TOO_BIG_MESSAGE);
						return;
					}
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
		DEBUGPRINT(DEBUGPRINT_WARNING, DEBUGPRINT_LEVEL_OBLIGATORY, "Sending size = 0 message\n");
		return;
	}

	//add message size
	msg.addHeader();
	//and encrypt if needed
	if(m_cryptoEnable && m_crypto){
		m_crypto->encrypt(msg);
	}

	//wait until all bytes are sent
	int sendBytes = 0;
	do{
		socketret_t b = send(m_socket, msg.getBuffer() + sendBytes, std::min(msg.getSize() - sendBytes, 1000), 0);
		if(b <= 0){
			closeConnectionError(ERROR_SEND_FAIL);
			return;
		}
		sendBytes += b;
	} while(sendBytes < msg.getSize());
}
