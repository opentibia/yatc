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

#include "connection.h"
#include "encryption.h"


ProtocolConfig::ProtocolConfig()
{
	os = 2; // 1 = linux, 2 = windows
	clientVersion = 800;
	datSignature = 0;
	sprSignature = 0;
	picSignature = 0;
}

//*****************************************************

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
	m_callback = NULL;
}

Connection::~Connection()
{
	closeConnection();
	delete m_protocol;
	delete m_crypto;
}

void Connection::setCallback(ConnectionCallback* callback)
{
	m_callback = callback;
}

void Connection::callCallback(int message)
{
	if(m_callback){
		m_callback(message);
	}
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
		}
		else if(ret == 1 && FD_ISSET(m_socket, &write_set)){
			//Check if it was a successful connection
			int optError;
			optlen_t optErrorLen = sizeof(int);
			int ret = getsockopt(m_socket, SOL_SOCKET, SO_ERROR, (opt_t*)&optError, &optErrorLen);
			if(ret != SOCKET_ERROR && optError == 0){
				//connection succeeded
				m_state = STATE_CONNECTED;

				//raise onConnect event
				m_protocol->onConnect();
			}
			else if(ret != SOCKET_ERROR && optError != 0){
				//connection failed
				closeConnectionError(ERROR_UNSUCCESSFULL_CONNECTION);
			}
			else{
				//call to getsockopt failed
				closeConnectionError(ERROR_GETSOCKTOPT_FAIL);
			}
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
		while(getPendingInput() > 0){
			switch(m_readState){
				case READING_SIZE:
				{
					int ret = internalRead(2);
					if(ret != 2){
						break;
					}
					m_msgSize = m_inputMessage.getU16();

					m_readState = READING_MESSAGE;
				}
				case READING_MESSAGE:
				{
					int ret = internalRead(m_msgSize);
					if(ret != m_msgSize){
						break;
					}
					//decrypt incoming message if needed
					if(m_cryptoEnable && m_crypto){
						if(!m_crypto->decrypt(m_inputMessage)){
							closeConnectionError(ERROR_DECRYPT_FAIL);
							break;
						}
					}
					//raise onRecv event
					if(!m_protocol->onRecv(m_inputMessage)){
						closeConnectionError(ERROR_PROTOCOL_ONRECV);
						break;
					}
					//resets input message state
					m_readState = READING_SIZE;
					m_inputMessage.Reset();
					break;
				}
			}
		}
		//Check socket state
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
	callCallback(error);
	m_state = STATE_ERROR;
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
	if(getPendingInput() != 0)
		return;

	timeval tv =  {0, 0}; //non-blocking select
	fd_set read_set;
	FD_ZERO(&read_set);
	FD_SET(m_socket, &read_set);

	int ret = select(m_socket + 1, &read_set, NULL, NULL, &tv);
	if(ret == 1){
		//Connection closed or error?
		closeConnectionError(ERROR_CONNECTED_SOCKET_ERROR);
	}
	else if(ret == SOCKET_ERROR){
		closeConnectionError(ERROR_SELECT_FAIL_CONNECTED);
	}
}

int Connection::internalRead(unsigned int n)
{
	//Check that we can read n bytes
	if(getPendingInput() < n){
		return 0;
	}

	//read them
	socketret_t ret = recv(m_socket, m_inputMessage.getReadBuffer(), n, 0);

	if(ret != SOCKET_ERROR && ret == (int)n){
		//we have read n bytes, so we resize inputMessage
		m_inputMessage.setReadSize(n);
		return n;
	}
	else if(ret == 0){
		//peer has performed an orderly shutdown
		return 0;
	}
	else{
		closeConnectionError(ERROR_RECV_FAIL);
		return -1;
	}
}

void Connection::sendMessage(NetworkMessage& msg)
{
	if(m_state != STATE_CONNECTED){
		printf("Calling send when state == STATE_CONNECTED(state = %d)\n", m_state);
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
