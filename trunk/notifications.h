//////////////////////////////////////////////////////////////////////
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

#ifndef __NOTIFICATION_H
#define __NOTIFICATION_H

#include <string>
#include <list>
#include "gamecontent/enums.h"

class Position;

class Notifications{
	public:

		~Notifications() {};

		//IMPORTANT: All strings and structs are passed by reference,
		// so remember to use a local copy if you are going to need them later.

		//**** General Notifications ****
		static void onConnectionError(int message);
		// onProtocolError is called before closing a connection due a protocol error
		static void onProtocolError(bool fatal);
		static void onEnterGame();

		//**** Login Messages ****
		static void changeLoginServer(){}
		static void openMessageWindow(WindowMessage_t type, const std::string& message);
		static void openWaitingList(const std::string& message, int time);
		static void openCharactersList(const std::list<CharacterList_t>& list, int premDays);

		//**** inGame events ****
		static void onVipLogin(uint32_t id) {}
		static void onVipLogout(uint32_t id) {}
		static void onVipState(uint32_t id, const std::string& name, bool state) {}
		static void onCancelWalk(Direction direction) {}
		static void onCancelAttack() {}
		//static void onDistanceEffect(const Position& from, const Position& to) {}
		static void onWalk(); //(const Position& from, const Position& to) {} // for now we don't need to know this information
		static void onChangeIcons() {}
		static void onChangeCreatureSkull(uint32_t id);
		static void onTextMessage(MessageType_t type, const std::string& message);
		//SAY,WHISPER, YELL, MONSTER_SAY, MONSTER_YELL
		static void onCreatureSpeak(SpeakClasses_t type, int n, const std::string& name, int level,
			const Position& pos, const std::string& message);
		//SPEAK_CHANNEL_Y, SPEAK_CHANNEL_R1, SPEAK_CHANNEL_R2, SPEAK_CHANNEL_O
		static void onCreatureSpeak(SpeakClasses_t type, int n, const std::string& name, int level,
			int channel, const std::string& message);
		//SPEAK_PRIVATE, SPEAK_PRIVATE_RED, SPEAK_BROADCAST
		static void onCreatureSpeak(SpeakClasses_t type, int n, const std::string& name, int level,
			const std::string& message);
		static void onCreatureMove(uint32_t id);

		//**** Open/Close inGame windows ****
		//open item text window
		static void openItemText(int windowId, int itemid, int maxTextLenght,
			const std::string& text, const std::string& lastChange,
			const std::string& lastChangeDate){}

		//open trade window
		static void openTradeWindow(bool ack){}
		static void closeTradeWindow(){}

		//open house window
		static void openHouseText(int windowId, int unk, const std::string& text){}

		//open outfit window
		static void openOutfit(const Outfit_t& current, const std::list<AvailOutfit_t>& available){}

		//open relogin window
		static void openRelogin(){}

		//open/close container
		static void openContainer(int cid){}
		static void closeContainer(int cid){}

		//channels
		static void openChannelList(const std::list<ChannelList_t>& list){}
		static void openChannel(int id, const std::string& name){}
		static void openPrivateChannel(int id, const std::string& name){}
		static void closePrivateChannel(int id){}
		static void openPrivatePlayerChat(const std::string& name){}

		//TODO. Rule violation

		//TODO. Quest windows

	protected:
		Notifications() {};
};


#endif
