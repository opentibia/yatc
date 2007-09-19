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

#ifndef __CONNECTION_H
#define __CONNECTION_H

#include "networkmessage.h"
#include "encryption.h"

#ifdef WIN32

#include "winsock2.h"
#include "windows.h"

#define socketret_t int
#define opt_t char
#define optlen_t int
#define CONNECT_WOULD_BLOCK WSAEWOULDBLOCK
#define EWOULDBLOCK WSAEWOULDBLOCK

#else

#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <errno.h>

extern int h_errno;

#define SOCKET int
#define closesocket close
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define SD_BOTH SHUT_RDWR

#define socketret_t ssize_t
#define opt_t int
#define optlen_t socklen_t
#define CONNECT_WOULD_BLOCK EINPROGRESS
#define EWOULDBLOCK EAGAIN

#endif


class Connection;

class ProtocolConfig
{
	public:
		~ProtocolConfig() {}

		static ProtocolConfig* getInstance()
		{
			static ProtocolConfig instance;
			return &instance;
		}

		int os;
		int clientVersion;
		int datSignature;
		int sprSignature;
		int picSignature;

	protected:
		ProtocolConfig();
};

class Protocol
{
	public:
		Protocol() : m_connection(NULL) {}
		virtual ~Protocol() {}

		virtual void onConnect() = 0;
		virtual bool onRecv(NetworkMessage& msg) = 0;

		void setConnection(Connection* con) { m_connection = con; }

	protected:
		Connection* m_connection;
};

typedef void (ConnectionCallback)(int message);

class Connection
{
	public:
		Connection(const std::string& host, uint16_t port, Encryption* crypto, Protocol* protocol);
		~Connection();

		enum STATE
		{
			STATE_INIT,
			STATE_CONNECTING,
			STATE_CONNECTED,
			STATE_CLOSED,
			STATE_ERROR
		};

		enum ConnectionError{
			ERROR_CANNOT_RESOLVE_HOST = 1,
			ERROR_WRONG_HOST_ADDR_TYPE,
			ERROR_CANNOT_CREATE_SOCKET,
			ERROR_CANNOT_SET_NOBLOCKING_SOCKET,
			ERROR_CANNOT_CONNECT,
			ERROR_SELECT_FAIL_CONNECTED,
			ERROR_SELECT_FAIL_CONNECTING,
			ERROR_UNSUCCESSFULL_CONNECTION,
			ERROR_GETSOCKTOPT_FAIL,
			ERROR_UNEXPECTED_SELECT_RETURN_VALUE,
			ERROR_CANNOT_GET_PENDING_SIZE,
			ERROR_RECV_FAIL,
			ERROR_DECRYPT_FAIL,
			ERROR_WRONG_MSG_SIZE,
			ERROR_SEND_FAIL,
			ERROR_PROTOCOL_ONRECV,
			ERROR_CONNECTED_SOCKET_ERROR
		};

		void executeNetwork();
		void setCallback(ConnectionCallback*);

		void closeConnection();
		STATE getState(){ return m_state; }
		int getSocketError();

		void sendMessage(NetworkMessage& msg);

		void setCryptoState(bool state){ m_cryptoEnable = state;}

		void setKey(char* key, int size){
			if(m_crypto){
				m_crypto->setKey(key, size);
			}
		}

	private:
		//functions
		void callCallback(int error);
		unsigned long getPendingInput();
		int internalRead(unsigned int n);
		void closeConnectionError(int error);
		void checkSocketReadState();

		//
		Encryption* m_crypto;
		Protocol* m_protocol;
		NetworkMessage m_inputMessage;

		//Remote host info
		std::string m_host;
		uint32_t m_ip;
		uint16_t m_port;

		//internal connection state
		bool m_cryptoEnable;

		enum READSTATE{
			READING_SIZE,
			READING_MESSAGE,
		};

		STATE m_state;
		READSTATE m_readState;
		int m_msgSize;

		//
		SOCKET m_socket;
		ConnectionCallback* m_callback;
};

#endif
