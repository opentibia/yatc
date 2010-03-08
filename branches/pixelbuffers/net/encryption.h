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

#ifndef __ENCRYPTION_H
#define __ENCRYPTION_H

#include "../stdinttypes.h"
#include "memory.h"

class NetworkMessage;

class Encryption
{
	public:
		Encryption(){}
		virtual ~Encryption(){}

		virtual bool encrypt(NetworkMessage& msg) = 0;
		virtual bool decrypt(NetworkMessage& msg) = 0;
		virtual bool setKey(char* key, int size) = 0;
};


class EncXTEA : public Encryption
{
	public:
		EncXTEA();
		virtual ~EncXTEA(){}

		virtual bool encrypt(NetworkMessage& msg);
		virtual bool decrypt(NetworkMessage& msg);

		virtual bool setKey(char* key, int size)
		{
			if(size != sizeof(uint32_t)*4){
				return false;
			}

			memcpy(m_key, key, sizeof(uint32_t)*4);
			return true;
		}

	private:
		uint32_t m_key[4];
};

#endif
