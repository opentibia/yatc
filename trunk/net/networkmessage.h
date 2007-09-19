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

#ifndef __NETMESSAGE_H
#define __NETMESSAGE_H
#include <string>

#define NETWORK_MESSAGE_SIZE 8192

class NetworkMessage
{
	public:
		NetworkMessage(int accessType);
		~NetworkMessage() {}

		void Reset();

		uint32_t getU32();
		uint16_t getU16();
		uint8_t getU8();
		std::string getString();

		void addU32(uint32_t value);
		void addU16(uint16_t value);
		void addU8(uint8_t value);
		void addString(const std::string& value) { addString(value.c_str()); }
		void addString(const char* value);
		void addPaddingBytes(uint32_t n);
		void addHeader();

		void skipBytes(int size){
			m_readPos += size;
		}

		char* getBuffer(){ return m_buffer + m_start; }
		char* getReadBuffer(){ return m_buffer + m_readPos; }

		int getReadPos() const { return m_readPos; }
		void setReadPos(int pos) { m_readPos = pos; }

		void setSize(int size) { m_size = size; }
		int getSize() const { return m_size; }
		void setReadSize(int size) { m_size = m_readPos + size; }
		int getReadSize() const { return m_size - m_readPos; }

		bool canRead(int bytes) const;
		bool canWrite(int bytes) const;

		enum{
			CAN_READ = 1,
			CAN_WRITE = 2,
		};
		void setType(int v){ m_accessType = v; }
		int getType() const { return m_accessType; }
	protected:

		int m_accessType;
		int m_start;
		int m_size;
		int m_readPos;
		int m_writePos;
		char m_buffer[NETWORK_MESSAGE_SIZE];
};

#endif
