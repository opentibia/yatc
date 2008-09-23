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

		//send functions impl
		virtual void sendLogout();
		virtual void sendAutoWalk(const std::list<Direction>& steps);
		virtual void sendStopAutoWalk();
		virtual void sendMove(Direction dir);
		virtual void sendTurn(Direction dir);
		virtual void sendThrow(const Position& fromPos, uint16_t itemid,
			uint8_t fromStackpos, const Position& toPos, uint8_t count);
		virtual void sendRequestTrade(const Position& pos, uint16_t itemid,
			uint8_t stackpos, uint32_t playerid);
		virtual void sendLookInTrade(bool inMyOffer, uint8_t index);
		virtual void sendAcceptTrade();
		virtual void sendRejectTrade();
		virtual void sendUseItem(const Position& pos, uint16_t itemid,
			uint8_t stackpos);
		virtual void sendUseItemWith(const Position& fromPos, uint16_t fromItemid,
			uint8_t fromStackpos, const Position& toPos, uint16_t toItemid,
			uint8_t toStackpos);
		virtual void sendUseBattleWindow(const Position& pos, uint16_t itemid,
			uint8_t stackpos, uint32_t creatureid);
		virtual void sendRotateItem(const Position& pos, uint16_t itemid,
			uint8_t stackpos);
		virtual void sendCloseContainer(uint8_t containerIndex);
		virtual void sendUpContainer(uint8_t containerIndex);
		virtual void sendTextWindow(uint32_t windowId, const std::string& text);
		virtual void sendHouseWindow(uint32_t windowId, int unk, const std::string& text);
		virtual void sendLookItem(const Position& pos, uint16_t itemid,
			uint8_t stackpos);
		//
		virtual void sendSay(SpeakClasses_t type, const std::string text);
		//
		virtual void sendSay(SpeakClasses_t type, uint16_t channel,
			const std::string text);
		//
		virtual void sendSay(SpeakClasses_t type, const std::string& sendToplayer,
			const std::string text);

		virtual void sendRequestChannels();
		virtual void sendOpenChannel(uint16_t channelid);
		virtual void sendCloseChannel(uint16_t channelid);
		virtual void sendOpenPrivatePlayerChat(const std::string& playerName);
		virtual void sendOpenRuleViolation(const std::string& text);
		virtual void sendCloseRuleViolation(const std::string& text);
		virtual void sendCancelRuleViolation();
		virtual void sendFightModes(uint8_t attack, uint8_t chase, uint8_t secure);
		virtual void sendAttackCreature(uint32_t creatureid);
		virtual void sendFollowCreature(uint32_t creatureid);
		virtual void sendCreatePrivateChatChannel();
		virtual void sendInvitePrivateChatChannel(const std::string& name);
		virtual void sendExcludePrivateChatChannel(const std::string& name);
		virtual void sendInviteParty(uint32_t playerid);
		virtual void sendJoinParty(uint32_t playerid);
		virtual void sendCancelInviteParty(uint32_t playerid);
		virtual void sendPassPartyLeader(uint32_t playerid);
		virtual void sendLeaveParty();
		virtual void sendCancelMove();
		virtual void sendRequestOutfit();
		virtual void sendSetOutfit(uint16_t looktype, uint8_t head, uint8_t body,
			uint8_t legs, uint8_t feet);
		virtual void sendAddVip(const std::string& name);
		virtual void sendRemVip(uint32_t playerid);
		virtual void sendBugReport(const std::string& text);
		virtual void sendRequestQuestLog();
		virtual void sendRequestQuest(uint16_t questid);


        virtual void sendCloseShop(); // player closes npc shop
        virtual void sendCloseNPCChannel();

        // translators
        virtual char translateSpeakClassFromInternal(SpeakClasses_t s);
        virtual SpeakClasses_t translateSpeakClassToInternal(char s);

	protected:
		ProtocolGame82(int account, const std::string& password, const std::string& name, bool isGM);

		virtual void sendPing();
		virtual void sendRequestUpdateTile(const Position& pos);
		virtual void sendRequestUpdateContainer(uint8_t containerid);

		bool setMapDescription(NetworkMessage& msg, int x, int y, int z, int width, int height);
		bool setFloorDescription(NetworkMessage& msg, int x, int y, int z, int width, int height, int offset);
		bool setTileDescription(NetworkMessage& msg, const Position& pos);
		Thing* internalGetThing(NetworkMessage& msg);
		Item* internalGetItem(NetworkMessage& msg, uint32_t itemid);
		bool internalSetOutfit(NetworkMessage& msg, Outfit_t& outfit);

		int16_t m_skipTiles;

		// protocol version assertion
		virtual void checkVersion();

		friend class ProtocolConfig;
};

#endif


