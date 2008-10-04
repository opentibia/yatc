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
class Thing;
class Item;

class ProtocolGame : public Protocol
{
	public:
		virtual ~ProtocolGame();

		virtual void onConnect();
		virtual bool onRecv(NetworkMessage& msg) = 0;

        //receive functions
        virtual bool parsePacket(uint8_t cmd, NetworkMessage& msg);
        virtual bool parseSelfAppear(NetworkMessage& msg); // 0x0a
        virtual bool parseGMActions(NetworkMessage& msg); // 0x0b
        virtual bool parseErrorMessage(NetworkMessage& msg); // 0x14
        virtual bool parseFYIMessage(NetworkMessage& msg); // 0x15
        virtual bool parseWaitingList(NetworkMessage& msg); // 0x16
        virtual bool parsePing(NetworkMessage& msg); // 0x1e
        virtual bool parseDeath(NetworkMessage& msg); // 0x28
        virtual bool parseCanReportBugs(NetworkMessage& msg); // 0x32
        virtual bool parseMapDescription(NetworkMessage& msg); // 0x64
        virtual bool parseMoveNorth(NetworkMessage& msg); // 0x65
        virtual bool parseMoveEast(NetworkMessage& msg); // 0x66
        virtual bool parseMoveSouth(NetworkMessage& msg); // 0x67
        virtual bool parseMoveWest(NetworkMessage& msg); // 0x68
        virtual bool parseUpdateTile(NetworkMessage& msg); // 0x69
        virtual bool parseTileAddThing(NetworkMessage& msg); // 0x6A
        virtual bool parseTileTransformThing(NetworkMessage& msg); // 0x6B
        virtual bool parseTileRemoveThing(NetworkMessage& msg); // 0x6C
        virtual bool parseCreatureMove(NetworkMessage& msg); // 0x6D
        virtual bool parseOpenContainer(NetworkMessage& msg); // 0x6E
        virtual bool parseCloseContainer(NetworkMessage& msg); // 0x6F
        virtual bool parseContainerAddItem(NetworkMessage& msg); // 0x70
        virtual bool parseContainerUpdateItem(NetworkMessage& msg); // 0x71
        virtual bool parseContainerRemoveItem(NetworkMessage& msg); // 0x72
        virtual bool parseInventorySetSlot(NetworkMessage& msg); // 0x78
        virtual bool parseInventoryResetSlot(NetworkMessage& msg); // 0x79
        virtual bool parseSafeTradeRequestAck(NetworkMessage& msg); // 0x7D
        virtual bool parseSafeTradeRequestNoAck(NetworkMessage& msg); // 0x7E
        virtual bool parseSafeTradeRequest(NetworkMessage& msg, bool ack); // 0x7D + 0x7E
        virtual bool parseSafeTradeClose(NetworkMessage& msg); // 0x7F
        virtual bool parseWorldLight(NetworkMessage& msg); // 0x82
        virtual bool parseMagicEffect(NetworkMessage& msg); // 0x83
        virtual bool parseAnimatedText(NetworkMessage& msg); // 0x84
        virtual bool parseDistanceShot(NetworkMessage& msg); // 0x85
        virtual bool parseCreatureSquare(NetworkMessage& msg); // 0x86
        virtual bool parseCreatureHealth(NetworkMessage& msg); // 0x8C
        virtual bool parseCreatureLight(NetworkMessage& msg); // 0x8D
        virtual bool parseCreatureOutfit(NetworkMessage& msg); // 0x8E
        virtual bool parseCreatureSpeed(NetworkMessage& msg); // 0x8F
        virtual bool parseCreatureSkulls(NetworkMessage& msg); // 0x90
        virtual bool parseCreatureShields(NetworkMessage& msg); // 0x91
		virtual bool parseItemTextWindow(NetworkMessage& msg); // 0x96
		virtual bool parseHouseTextWindow(NetworkMessage& msg); // 0x97
		virtual bool parsePlayerStats(NetworkMessage& msg); // 0xA0
		virtual bool parsePlayerSkills(NetworkMessage& msg); // 0xA1
		virtual bool parsePlayerIcons(NetworkMessage& msg); // 0xA2
		virtual bool parsePlayerCancelAttack(NetworkMessage& msg); // 0xA3
		virtual bool parseCreatureSpeak(NetworkMessage& msg); // 0xAA
		virtual bool parseChannelList(NetworkMessage& msg); // 0xAB
		virtual bool parseOpenChannel(NetworkMessage& msg); // 0xAC
		virtual bool parseOpenPrivatePlayerChat(NetworkMessage& msg); // 0xAD
		virtual bool parseOpenRuleViolation(NetworkMessage& msg); // 0xAE
		virtual bool parseRuleViolationAF(NetworkMessage& msg); // 0xAF
		virtual bool parseRuleViolationB0(NetworkMessage& msg); // 0xB0
		virtual bool parseRuleViolationB1(NetworkMessage& msg); // 0xB1
		virtual bool parseCreatePrivateChannel(NetworkMessage& msg); // 0xB2
		virtual bool parseClosePrivateChannel(NetworkMessage& msg); // 0xB3
		virtual bool parseTextMessage(NetworkMessage& msg); // 0xB4
		virtual bool parsePlayerCancelWalk(NetworkMessage& msg); // 0xB5
		virtual bool parseFloorChangeUp(NetworkMessage& msg); // 0xBE
		virtual bool parseFloorChangeDown(NetworkMessage& msg); // 0xBF
		virtual bool parseOutfitWindow(NetworkMessage& msg); // 0xC8
		virtual bool parseVipState(NetworkMessage& msg); // 0xD2
		virtual bool parseVipLogin(NetworkMessage& msg); // 0xD3
		virtual bool parseVipLogout(NetworkMessage& msg); // 0xD4
		virtual bool parseQuestList(NetworkMessage& msg); // 0xF0
		virtual bool parseQuestPartList(NetworkMessage& msg); // 0xF1

		// 8.2+
		virtual bool parseOpenShopWindow(NetworkMessage& msg); // 0x7A
		virtual bool parsePlayerCash(NetworkMessage& msg); // 0x7B
		virtual bool parseCloseShopWindow(NetworkMessage& msg); // 0x7C
		virtual bool parseShowTutorial(NetworkMessage& msg); // 0xDC
		virtual bool parseAddMapMarker(NetworkMessage& msg); // 0xDD
	
	

	
		//send functions
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
		//virtual void sendGMReport(); //Message: 0xE7
		virtual void sendRequestQuestLog();
		virtual void sendRequestQuest(uint16_t questid);

		// 8.2+
        virtual void sendCloseShop();
        virtual void sendCloseNPCChannel();

		// protocol version assertion
		virtual void checkVersion() = 0;



        // translators
        virtual SpeakClasses_t translateSpeakClassToInternal(char s){return (SpeakClasses_t)s;}
        virtual char translateSpeakClassFromInternal(SpeakClasses_t s){return s;}
		virtual MessageType_t translateTextMessageToInternal(uint8_t messageType) {return (MessageType_t)messageType;}



	protected:

		ProtocolGame(int account, const std::string& password, const std::string& name, bool isGM);

		virtual void sendPing();
		virtual void sendRequestUpdateTile(const Position& pos);
		virtual void sendRequestUpdateContainer(uint8_t containerid);


		virtual bool setMapDescription(NetworkMessage& msg, int x, int y, int z, int width, int height);
		virtual bool setFloorDescription(NetworkMessage& msg, int x, int y, int z, int width, int height, int offset);
		virtual bool setTileDescription(NetworkMessage& msg, const Position& pos);

        virtual Thing* internalGetThing(NetworkMessage& msg) = 0;
        virtual Item* internalGetItem(NetworkMessage& msg, uint32_t itemid) = 0;
		virtual bool internalSetOutfit(NetworkMessage& msg, Outfit_t& outfit) = 0;

		int16_t m_skipTiles;

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
