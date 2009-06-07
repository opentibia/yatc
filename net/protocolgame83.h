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


#ifndef __PROTOCOLGAME83_H
#define __PROTOCOLGAME83_H

#include "protocolgame.h"
#include "../notifications.h"

class Thing;
class Item;

class ProtocolGame83 : public ProtocolGame
{
	public:
		virtual ~ProtocolGame83();

		virtual bool onRecv(NetworkMessage& msg);

		virtual const char* getProtocolName() { return "Protocol83";}
		virtual ClientVersion_t getVersion() const { return CLIENT_VERSION_830; }

		//send
		virtual void sendEnableSharedExperience(bool enabled, uint8_t unknown);

        //receive f. impl
        virtual bool parsePacket(uint8_t cmd, NetworkMessage& msg);
		virtual bool parsePlayerStats(NetworkMessage& msg);
        virtual bool parseGMActions(NetworkMessage& msg);
        virtual bool parseOpenShopWindow(NetworkMessage& msg);
        virtual bool parsePlayerCash(NetworkMessage& msg);

        // translators
        virtual char translateSpeakClassFromInternal(SpeakClasses_t s);
        virtual SpeakClasses_t translateSpeakClassToInternal(char s);
		virtual MessageType_t translateTextMessageToInternal(uint8_t messageType);

		// readonly protocol props
		virtual bool hasChecksum() const { return true; }

	protected:
		ProtocolGame83(const std::string& accountname, const std::string& password, const std::string& name, bool isGM);

		Thing* internalGetThing(NetworkMessage& msg);
		Item* internalGetItem(NetworkMessage& msg, uint32_t itemid);
		bool internalSetOutfit(NetworkMessage& msg, Outfit_t& outfit);

		// protocol version assertion
		virtual void checkVersion();

		friend class ProtocolConfig;
};

#endif


