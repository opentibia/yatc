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
#include "connectionreplay.h"

// description of purpose included in connectionreplay.h

ConnectionReplay::ConnectionReplay(const std::string& file, Protocol* protocol) :
Connection(file,0,NULL,protocol) // we don't need crypto, we'll use m_host as storage for filename, port is of no use to us, etc
{
    m_file = NULL; // at construction of connectionreplay, do nothing; open connection later, in execNetwork, at connection phase
    m_nextticks = m_lastticks = 0;
}

ConnectionReplay::~ConnectionReplay()
{
}


void ConnectionReplay::executeNetwork()
{
    printf("CR: executing net\n");
    switch(m_state){
	case STATE_INIT:
	{
		//Open file

		m_ip = INADDR_NONE;
		if (!(m_file=fopen(m_host.c_str(),"rb")))
		{
            closeConnectionError(ERROR_CANNOT_RESOLVE_HOST);
            return;
		}

        //Delay since start of replay
        //We have this as an increasing-from-zero var instead of just containing
        //SDL_GetTicks()'s return value at start of the replay and then subtracting it
        //because we want to be able to PAUSE the replay.
        m_ticks = 0;
        //Ticks during last executeNetwork()
        m_lastticks = SDL_GetTicks();
        //
        fread(&m_nextticks,4,1,m_file);
        printf("NEXTTICKS: %d\n", m_nextticks);

		//Reset traffic counter
		m_sentBytes = 0;
		m_recvBytes = 0;

		//And connect
        //connection succeeds
        m_state = STATE_CONNECTED;

        m_inputMessage.reset();
		break;
	}
	case STATE_CONNECTING:
	{
	    // should never happen. still...
	    printf("ConnectionReplay::executeNetwork(): Warning: passed through STATE_CONNECTING.\n");
	    m_state = STATE_CONNECTED;
        break;
	}
    case STATE_CONNECTED:
    {
        //Update ticks
        m_ticks += SDL_GetTicks() - m_lastticks;
        m_lastticks = SDL_GetTicks();
        printf("Ticks: %d\n", m_ticks);

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
					printf("MsgSize: %d\n", m_msgSize);
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
                        // works for 8.40 too, however!
                        // data in unencrypted packets is now including unencrypted length,
                        // + encrypted length ... although packet is not encrypted
                        // let's just cut the cr/\p

                        // DONT DO THIS FOR REPLAY
                        /*uint16_t size = */ //m_inputMessage.getU16();
					}
					//raise onRecv event
					if(!m_protocol->onRecv(m_inputMessage)){
						closeConnectionError(ERROR_PROTOCOL_ONRECV);
						return;
					}
					//resets input message state
					m_readState = READING_SIZE;
					m_inputMessage.reset();

					fread(&m_nextticks, 4, 1, m_file);
					//m_ticks = 0; // nextticks is a delta, let's hack
					//m_lastticks = SDL_GetTicks();
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

unsigned long ConnectionReplay::getPendingInput()
{
    if (m_ticks < m_nextticks) // not ready yet for the next message
        return 0;
    switch(m_readState)
    {
        case READING_SIZE:
            return 2;
            break;
        case READING_MESSAGE:
            return m_msgSize;
            break;
    }
    return 0;
}

void ConnectionReplay::closeConnection()
{
    fclose(m_file);

	m_state = STATE_CLOSED;
}

int ConnectionReplay::internalRead(unsigned int n, bool all)
{
    fread(m_inputMessage.getReadBuffer() + m_inputMessage.getReadSize(), n, 1, m_file);
    m_inputMessage.setReadSize(m_inputMessage.getReadSize() + n);

    return n;
}

void ConnectionReplay::checkSocketReadState()
{
    // FIXME should handle EOF
	if(m_state != STATE_CONNECTED)
		return;
/*
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
*/
}

