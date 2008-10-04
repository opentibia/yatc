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


#ifndef __PROTOCOLGAME82_H
#define __PROTOCOLGAME82_H

#include "protocolgame.h"
#include "../notifications.h"

class Thing;
class Item;

class ProtocolGame82 : public ProtocolGame
{
	public:
		virtual ~ProtocolGame82();

		virtual bool onRecv(NetworkMessage& msg);

		const char* getProtocolName() { return "Protocol82";}

        //receive f. impl
        virtual bool parsePacket(uint8_t cmd, NetworkMessage& msg);


        // translators
        virtual char translateSpeakClassFromInternal(SpeakClasses_t s);
        virtual SpeakClasses_t translateSpeakClassToInternal(char s);
		virtual MessageType_t translateTextMessageToInternal(uint8_t messageType);

	protected:
		ProtocolGame82(const std::string& accountname, const std::string& password, const std::string& name, bool isGM);

		Thing* internalGetThing(NetworkMessage& msg);
		Item* internalGetItem(NetworkMessage& msg, uint32_t itemid);
		bool internalSetOutfit(NetworkMessage& msg, Outfit_t& outfit);

		// protocol version assertion
		virtual void checkVersion();

		friend class ProtocolConfig;
};

#endif


