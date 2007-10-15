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


#ifndef __PROTOCOLGAME80_H
#define __PROTOCOLGAME80_H

#include "protocolgame.h"
#include "notifications.h"

class Thing;
class Item;

class ProtocolGame80 : public ProtocolGame
{
	public:
		ProtocolGame80(int account, const std::string& password, const std::string& name, bool isGM);
		virtual ~ProtocolGame80() {};

		virtual bool onRecv(NetworkMessage& msg);

		const char* getProtocolName() { return "Protocol80";}

		//send functions impl

		// FIXME (ivucica#1#) i "implemented" these functions as stubs so that i can compile. i hate pure functions :(
		void sendLogout() {}
		void sendAutoWalk(const std::list<Direction>& steps) {}
		void sendStopAutoWalk()  {}
		void sendMove(Direction dir) {}
		void sendTurn(Direction dir) {}
		void sendThrow(const Position& fromPos, uint16_t itemid,
			uint8_t fromStackpos, const Position& toPos, uint8_t count) {}
		void sendRequestTrade(const Position& pos, uint16_t itemid,
			uint8_t stackpos, uint32_t playerid) {}
		void sendLookInTrade(bool inMyOffer, uint8_t index) {}
		void sendAcceptTrade() {}
		void sendRejectTrade() {}
		void sendUseItem(const Position& pos, uint16_t itemid,
			uint8_t stackpos) {}
		void sendUseItemWith(const Position& fromPos, uint16_t fromItemid,
			uint8_t fromStackpos, const Position& toPos, uint16_t toItemid,
			uint8_t toStackpos) {}
		void sendUseBattleWindow(const Position& pos, uint16_t itemid,
			uint8_t stackpos, uint32_t creatureid) {}
		void sendRotateItem(const Position& pos, uint16_t itemid,
			uint8_t stackpos) {}
		void sendCloseContainer(uint8_t containerIndex) {}
		void sendUpContainer(uint8_t containerIndex) {}
		void sendTextWindow(uint32_t windowId, const std::string& text) {}
		void sendHouseWindow(uint32_t windowId, int unk, const std::string& text) {}
		void sendLookItem(const Position& pos, uint16_t itemid,
			uint8_t stackpos) {}
		//
		void sendSay(SpeakClasses_t type, const std::string text) {}
		//
		void sendSay(SpeakClasses_t type, uint16_t channel,
			const std::string text) {}
		//
		void sendSay(SpeakClasses_t type, const std::string& sendToplayer,
			const std::string text) {}

		void sendRequestChannels() {}
		void sendOpenChannel(uint16_t channelid) {}
		void sendCloseChannel(uint16_t channelid) {}
		void sendOpenPrivatePlayerChat(const std::string& playerName) {}
		void sendOpenRuleViolation(const std::string& text) {}
		void sendCloseRuleViolation(const std::string& text) {}
		void sendCancelRuleViolation() {}
		void sendFightModes(uint8_t attack, uint8_t chase, uint8_t secure) {}
		void sendAttackCreature(uint32_t creatureid) {}
		void sendFollowCreature(uint32_t creatureid) {}
		void sendCreatePrivateChatChannel() {}
		void sendInvitePrivateChatChannel(const std::string& name) {}
		void sendExcludePrivateChatChannel(const std::string& name) {}
		void sendInviteParty(uint32_t playerid) {}
		void sendJoinParty(uint32_t playerid) {}
		void sendCancelInviteParty(uint32_t playerid) {}
		void sendPassParyLeader(uint32_t playerid) {}
		void sendLeaveParty() {}
		void sendCancelMove() {}
		void sendRequestOutfit() {}
		void sendSetOutfit(uint16_t looktype, uint8_t head, uint8_t body,
			uint8_t legs, uint8_t feet) {}
		void sendAddVip(const std::string& name) {}
		void sendRemVip(uint32_t playerid) {}
		void sendBugReport(const std::string& text) {}
		//virtual void sendGMReport() = 0; //Message: 0xE7
		void sendRequestQuestLog() {}
		void sendRequestQuest(uint16_t questid) {}

		void sendPing() {}
		void sendRequestUpdateTile(const Position& pos) {}
		void sendRequestUpdateContainer(uint8_t containerid) {}


	protected:
		bool setMapDescription(NetworkMessage& msg, int x, int y, int z, int width, int height);
		bool setFloorDescription(NetworkMessage& msg, int x, int y, int z, int width, int height, int offset);
		bool setTileDescription(NetworkMessage& msg, const Position& pos);
		Thing* internalGetThing(NetworkMessage& msg);
		Item* internalGetItem(NetworkMessage& msg, uint32_t itemid);
		bool internalSetOutfit(NetworkMessage& msg, Outfit_t& outfit);

		int16_t m_skipTiles;
};

#endif


