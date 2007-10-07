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


#ifndef __PROTOCOLGAME_H
#define __PROTOCOLGAME_H

#include <string>
#include "connection.h"
#include "../gamecontent/enums.h"

class NetworkMessage;

class ProtocolGame : public Protocol
{
	public:
		virtual ~ProtocolGame();

		virtual void onConnect();
		virtual bool onRecv(NetworkMessage& msg) = 0;

		//send functions - pure virtual
		virtual void sendLogout() = 0;
		virtual void sendAutoWalk(const std::list<Direction>& steps) = 0;
		virtual void sendStopAutoWalk() = 0;
		virtual void sendMove(Direction dir) = 0;
		virtual void sendTurn(Direction dir) = 0;
		virtual void sendThrow(const Position& fromPos, uint16_t itemid,
			uint8_t fromStackpos, const Position& toPos, uint8_t count) = 0;
		virtual void sendRequestTrade(const Position& pos, uint16_t itemid,
			uint8_t stackpos, uint32_t playerid) = 0;
		virtual void sendLookInTrade(bool inMyOffer, uint8_t index) = 0;
		virtual void sendAcceptTrade() = 0;
		virtual void sendRejectTrade() = 0;
		virtual void sendUseItem(const Position& pos, uint16_t itemid,
			uint8_t stackpos) = 0;
		virtual void sendUseItemWith(const Position& fromPos, uint16_t fromItemid,
			uint8_t fromStackpos, const Position& toPos, uint16_t toItemid,
			uint8_t toStackpos) = 0;
		virtual void sendUseBattleWindow(const Position& pos, uint16_t itemid,
			uint8_t stackpos, uint32_t creatureid) = 0;
		virtual void sendRotateItem(const Position& pos, uint16_t itemid,
			uint8_t stackpos) = 0;
		virtual void sendCloseContainer(uint8_t containerIndex) = 0;
		virtual void sendUpContainer(uint8_t containerIndex) = 0;
		virtual void sendTextWindow(uint32_t windowId, const std::string& text) = 0;
		virtual void sendHouseWindow(uint32_t windowId, int unk, const std::string& text) = 0;
		virtual void sendLookItem(const Position& pos, uint16_t itemid,
			uint8_t stackpos) = 0;
		//
		virtual void sendSay(SpeakClasses_t type, const std::string text) = 0;
		//
		virtual void sendSay(SpeakClasses_t type, uint16_t channel,
			const std::string text) = 0;
		//
		virtual void sendSay(SpeakClasses_t type, const std::string& sendToplayer,
			const std::string text) = 0;

		virtual void sendRequestChannels() = 0;
		virtual void sendOpenChannel(uint16_t channelid) = 0;
		virtual void sendCloseChannel(uint16_t channelid) = 0;
		virtual void sendOpenPrivatePlayerChat(const std::string& playerName) = 0;
		virtual void sendOpenRuleViolation(const std::string& text) = 0;
		virtual void sendCloseRuleViolation(const std::string& text) = 0;
		virtual void sendCancelRuleViolation() = 0;
		virtual void sendFightModes(uint8_t attack, uint8_t chase, uint8_t secure) = 0;
		virtual void sendAttackCreature(uint32_t creatureid) = 0;
		virtual void sendFollowCreature(uint32_t creatureid) = 0;
		virtual void sendCreatePrivateChatChannel() = 0;
		virtual void sendInvitePrivateChatChannel(const std::string& name) = 0;
		virtual void sendExcludePrivateChatChannel(const std::string& name) = 0;
		virtual void sendInviteParty(uint32_t playerid) = 0;
		virtual void sendJoinParty(uint32_t playerid) = 0;
		virtual void sendCancelInviteParty(uint32_t playerid) = 0;
		virtual void sendPassParyLeader(uint32_t playerid) = 0;
		virtual void sendLeaveParty() = 0;
		virtual void sendCancelMove() = 0;
		virtual void sendRequestOutfit() = 0;
		virtual void sendSetOutfit(uint16_t looktype, uint8_t head, uint8_t body,
			uint8_t legs, uint8_t feet) = 0;
		virtual void sendAddVip(const std::string& name) = 0;
		virtual void sendRemVip(uint32_t playerid) = 0;
		virtual void sendBugReport(const std::string& text) = 0;
		//virtual void sendGMReport() = 0; //Message: 0xE7
		virtual void sendRequestQuestLog() = 0;
		virtual void sendRequestQuest(uint16_t questid) = 0;

	protected:

		ProtocolGame(int account, const std::string& password, const std::string& name, bool isGM);

		virtual void sendPing() = 0;
		virtual void sendRequestUpdateTile(const Position& pos) = 0;
		virtual void sendRequestUpdateContainer(uint8_t containerid) = 0;

		std::string m_password;
		std::string m_name;
		int m_account;
		bool m_isGM;
		NetworkMessage m_outputMessage;

		friend class SendMessageFunction;
};

class SendMessageFunction
{
public:
	inline SendMessageFunction(ProtocolGame* protocol)
	{
		m_protocol = protocol;
		protocol->m_outputMessage.reset();
	}

	inline ~SendMessageFunction()
	{
		ASSERT(m_protocol->m_outputMessage.isMessageTypeAdded());
		#ifndef DISABLE_SEND_FUNCTIONS
		m_protocol->m_connection->sendMessage(m_protocol->m_outputMessage);
		#endif
	}

protected:
	ProtocolGame* m_protocol;
};

#define PROTOCOLGAME_SEND_FUNCTION SendMessageFunction SEND_FUNCTION_(this);

#endif
