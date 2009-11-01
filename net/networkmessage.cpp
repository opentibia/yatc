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

#include "networkmessage.h"
#include "stdio.h"
#include "../gamecontent/map.h"

NetworkMessage::NetworkMessage(int type)
{
	reset();
	m_accessType = type;
}

void NetworkMessage::reset()
{
	m_readPos = 0;
	m_size = 0;

	m_start = 4;
	m_writePos = 4;

	m_messageTypeAdded = false;
}

bool NetworkMessage::canRead(int bytes) const
{
	if((m_accessType & CAN_READ) && (getReadSize() >= bytes)){
		return true;
	}
	else{
		return false;
	}
}

bool NetworkMessage::canWrite(int bytes) const
{
	if((m_accessType & CAN_WRITE) && (m_writePos + bytes < NETWORK_MESSAGE_SIZE - 32)){
		return true;
	}
	else{
		return false;
	}
}

uint32_t NetworkMessage::getU32()
{
	if(canRead(4)){
		uint32_t v = (((uint8_t)m_buffer[m_readPos]            | ((uint8_t)m_buffer[m_readPos + 1] <<  8) |
					  ((uint8_t)m_buffer[m_readPos + 2] << 16) | ((uint8_t)m_buffer[m_readPos + 3] << 24)));
		m_readPos += 4;
		return v;
	}
	else{
		return 0;
	}
}

uint16_t NetworkMessage::getU16()
{
	if(canRead(2)){
		uint16_t v = ((uint8_t)m_buffer[m_readPos] | ((uint8_t)m_buffer[m_readPos + 1] << 8));
		m_readPos += 2;
		return v;
	}
	else{
		return 0;
	}
}

uint8_t NetworkMessage::getU8()
{
	if(canRead(1)){
		return m_buffer[m_readPos++];
	}
	else{
		return 0;
	}
}

std::string NetworkMessage::getString()
{
	uint16_t stringSize = getU16();
	if(stringSize != 0 && canRead(stringSize)){
		const char* v = (const char*)(m_buffer + m_readPos);
		m_readPos += stringSize;
		return std::string(v, stringSize);
	}
	else{
		return "";
	}
}


bool NetworkMessage::getU32(uint32_t& v)
{
	if(canRead(4)){
		v = (((uint8_t)m_buffer[m_readPos]            | ((uint8_t)m_buffer[m_readPos + 1] <<  8) |
			((uint8_t)m_buffer[m_readPos + 2] << 16) | ((uint8_t)m_buffer[m_readPos + 3] << 24)));
		m_readPos += 4;

		return true;
	}
	else{
		return false;
	}
}
bool NetworkMessage::getU16(uint16_t& v)
{
	if(canRead(2)){
		v = ((uint8_t)m_buffer[m_readPos] | ((uint8_t)m_buffer[m_readPos + 1] << 8));
		m_readPos += 2;
		return true;
	}
	else{
		return false;
	}
}

bool NetworkMessage::inspectU16(uint16_t& v)
{
	if(canRead(2)){
		v = ((uint8_t)m_buffer[m_readPos] | ((uint8_t)m_buffer[m_readPos + 1] << 8));
		return true;
	}
	else{
		return false;
	}
}

bool NetworkMessage::getU8(uint8_t& v)
{
	if(canRead(1)){
		v = m_buffer[m_readPos++];
		return true;
	}
	else{
		return false;
	}
}

bool NetworkMessage::getString(std::string& v)
{
	uint16_t stringSize;
	if(getU16(stringSize) && canRead(stringSize)){
		const char* cstr = (const char*)(m_buffer + m_readPos);
		m_readPos += stringSize;
		v = std::string(cstr, stringSize);
		return true;
	}
	else{
		return false;
	}
}

bool NetworkMessage::getPosition(Position& p)
{
	uint16_t u16;
	if(!getU16(u16)){
		return false;
	}
	p.x = u16;

	if(!getU16(u16)){
		return false;
	}
	p.y = u16;

	uint8_t u8;
	if(!getU8(u8)){
		return false;
	}
	p.z = u8;
	return true;
}

void NetworkMessage::addU32(uint32_t value)
{
	if(canWrite(4)){
		m_buffer[m_writePos++] = (uint8_t)(value);
		m_buffer[m_writePos++] = (uint8_t)(value >>  8);
		m_buffer[m_writePos++] = (uint8_t)(value >> 16);
		m_buffer[m_writePos++] = (uint8_t)(value >> 24);
		m_size += 4;
	}
}

void NetworkMessage::addU16(uint16_t value)
{
	if(canWrite(2)){
		m_buffer[m_writePos++] = (uint8_t)(value);
		m_buffer[m_writePos++] = (uint8_t)(value >>  8);
		m_size += 2;
	}
}

void NetworkMessage::addU8(uint8_t value)
{
	if(canWrite(1)){
		m_buffer[m_writePos++] = value;
		m_size += 1;
	}
}

void NetworkMessage::addString(const char* value)
{
	int stringSize = strlen(value);
	if(canWrite(2 + stringSize)){
		addU16(stringSize);
		memcpy(m_buffer + m_writePos, value, stringSize);
		m_writePos += stringSize;
		m_size += stringSize;
	}
}

void NetworkMessage::addRaw(const void* content, size_t amount)
{
    if (canWrite(amount))
    {
        memcpy(m_buffer + m_writePos, content, amount);
        m_writePos += amount;
        m_size += amount;
    }
}

void NetworkMessage::addPosition(const Position& pos)
{
	if(canWrite(5)){
		addU16(pos.x);
		addU16(pos.y);
		addU8(pos.z);
	}
}

void NetworkMessage::addPaddingBytes(uint32_t n)
{
	if(canWrite(n)){
		memset((void*)&m_buffer[m_writePos], 0xAC, n);
		m_size += n;
	}
}

void NetworkMessage::addHeader()
{
	if(m_start >= 2){
		m_start = m_start - 2;
		m_buffer[m_start] = (uint8_t)(m_size);
		m_buffer[m_start + 1] = (uint8_t)(m_size >> 8);
		m_size += 2;
	}
}


uint32_t NetworkMessage::getChecksum(uint16_t offset)
{
    // for 8.3+
    // implementation of adler algorithm as per wikipedia
    // this will just calculate checksum, it still needs to be added
    // preparation
    const uint16_t MOD_ADLER=65521;
    uint8_t* data = ((uint8_t*)m_buffer+m_start)+offset;
    size_t len = m_size - offset;

    // algo
    uint32_t a = 1, b = 0;

    while (len > 0)
    {
        size_t tlen = len > 5552 ? 5552 : len;
        len -= tlen;
        do
        {
            a += *data++;
            b += a;
        } while (--tlen);

        a %= MOD_ADLER;
        b %= MOD_ADLER;
    }

    return (b << 16) | a;

}

void NetworkMessage::addChecksum()
{
    // for 8.3+
    // adds 32-bit adler checksum to bytes 2-6 and shifts remaining onwards
    // only call after msg is encrypted!

	if(canWrite(4)){
		uint32_t sum = getChecksum(2);
		memmove(m_buffer + m_start + 6, m_buffer + m_start + 2, m_size - 2); // move everything but first two bytes
		*((uint32_t*)(m_buffer+m_start+2)) = sum;
		m_size += 4;

		m_buffer[m_start] = (uint8_t)(m_size-2);
		m_buffer[m_start + 1] = (uint8_t)((m_size-2) >> 8); // dunno why -2, but apparently it works.

	}

}
