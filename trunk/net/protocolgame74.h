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


#ifndef __PROTOCOLGAME74_H
#define __PROTOCOLGAME74_H

#include "protocolgame.h"

class ProtocolGame74 : public ProtocolGame
{
	public:
		virtual ~ProtocolGame74();

        virtual void onConnect();
        virtual void sendLogin(NetworkMessage*msg=NULL);
        virtual bool onRecv(NetworkMessage& msg);

        //receive functions
		virtual bool parsePlayerStats(NetworkMessage& msg); // 0xA0
		virtual bool parsePlayerSkills(NetworkMessage& msg); // 0xA1
		virtual bool parseCreatureSpeak(NetworkMessage& msg); // 0xAA
		virtual bool parseOutfitWindow(NetworkMessage& msg); // 0xC8

		//send functions
		virtual void sendFightModes(uint8_t attack, uint8_t chase, uint8_t secure);
		virtual void sendSetOutfit(uint16_t looktype, uint8_t head, uint8_t body,
			uint8_t legs, uint8_t feet, uint8_t addons=0);
		virtual void sendGMReport(const std::string& targetplayer, uint8_t reason, const std::string& comment, const std::string& statement, uint8_t action, bool ipban);

		// protocol version assertion
		virtual void checkVersion();
		virtual const char* getProtocolName() { return "Protocol7.4";}
		virtual ClientVersion_t getVersion() const { return CLIENT_VERSION_740; }


        // translators
        //virtual SpeakClasses_t translateSpeakClassToInternal(char s){return (SpeakClasses_t)s;}
        //virtual char translateSpeakClassFromInternal(SpeakClasses_t s){return s;}
		//virtual MessageType_t translateTextMessageToInternal(uint8_t messageType) {return (MessageType_t)messageType;}

		// readonly properties of protocol
		virtual bool hasChecksum() const { return false; }

	protected:
		ProtocolGame74(const std::string& accountname, const std::string& password, const std::string& name, bool isGM);

        virtual Thing* internalGetThing(NetworkMessage& msg);
        virtual Item* internalGetItem(NetworkMessage& msg, uint32_t itemid);
		virtual bool internalSetOutfit(NetworkMessage& msg, Outfit_t& outfit);

		friend class ProtocolConfig;
};
#endif
