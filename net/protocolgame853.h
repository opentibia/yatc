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


#ifndef __PROTOCOLGAME853_H
#define __PROTOCOLGAME853_H

#include "protocolgame85.h"

class Thing;
class Item;

class ProtocolGame853 : public ProtocolGame85
{
	public:
		virtual ~ProtocolGame853();

		virtual const char* getProtocolName() { return "Protocol853";}
		virtual ClientVersion_t getVersion() const { return CLIENT_VERSION_853; }

        virtual void onConnect();
        virtual bool parseTileAddThing(NetworkMessage& msg);
        virtual bool parsePacket(uint8_t cmd, NetworkMessage& msg);

	protected:
		ProtocolGame853(const std::string& accountname, const std::string& password, const std::string& name, bool isGM);

		virtual void checkVersion();
		friend class ProtocolConfig;
};

#endif
