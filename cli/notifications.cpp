//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
// Notifications to CLI
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

#include <iostream>
#include <sstream>
#include "../notifications.h"
#include "../net/connection.h"
extern Connection* g_connection;

extern std::string g_recordfilename;

#include "../gamecontent/container.h"
#include "../gamecontent/shop.h"
#include "../gamecontent/creature.h"
#include "../gamecontent/globalvars.h"
#include "../gamecontent/inventory.h"
#include "../gamecontent/map.h"
#include "../debugprint.h"
#include "../util.h"

void Notifications::openCharactersList(const std::list<CharacterList_t>& list, int premDays)
{
  std::cout << "character list" << std::endl;
  for (std::list<CharacterList_t>::const_iterator it = list.begin(); it != list.end(); it++)
    {
      std::cout << it->name << " (" << it->world << ")" << std::endl; 
    }
}

void Notifications::onConnectionError(int message)
{
  if(message == Connection::ERROR_CLOSED_SOCKET)
    {
      Containers::getInstance().clear();
      Creatures::getInstance().clear();
      GlobalVariables::clear();
      Inventory::getInstance().clear();
      Map::getInstance().clear();
      std::cout << "disconnected" << std::endl;
      exit(0);
    }
  else
    {
      std::cout << "connection error: " << Connection::getErrorDesc(message) << std::endl;
      exit(1);
    }
}

void Notifications::onProtocolError(bool fatal)
{
  Protocol* protocol = g_connection->getProtocol();
  std::string error = protocol->getErrorDesc();
  DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "\n********************************************************\n");
  if(fatal){
    DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "PROTOCOL ERROR: %s\n", error.c_str());
  }
  else{
    DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "PROTOCOL WARNING: %s\n", error.c_str());
  }
  const std::list<uint8_t>& serverCmd = protocol->getLastServerCmd();
  std::list<uint8_t>::const_iterator it;
  DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Last received cmd: ");
  for(it = serverCmd.begin(); it != serverCmd.end(); ++it){
    DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "%02x ", *it);
  }
  DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "\n");
  DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Total Received: %d\n", protocol->getCurrentMsgN());
  const NetworkMessage* msg = protocol->getCurrentMsg();
  if(msg){
    const unsigned char* buffer = (const unsigned char*)msg->getBuffer();
    if(buffer){
      int32_t msgRealSize = msg->getSize() - msg->getStart();
      DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "Last received msg: (%x/%x)\n", (int)((long)msg->getReadBuffer() - (long)msg->getBuffer()), msgRealSize);
#define LINE_SIZE 16
      for(int32_t i = 0; i < msgRealSize; i += LINE_SIZE){
	int32_t pos = i;
	DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "%04x : ", i);
	for(int32_t j = 0; j < LINE_SIZE; ++j, ++pos){
	  if(pos < msgRealSize){
	    DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "%02x ", buffer[pos]);
	  }
	  else{
	    DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "   ");
	  }
	}
	DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "; ");
	pos = i;
	for(uint32_t j = 0; j < LINE_SIZE; ++j, ++pos){
	  if(pos < msgRealSize){
	    if(buffer[pos] >= 32 && buffer[pos] < 127){
	      DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "%c", buffer[pos]);
	    }
	    else{
	      DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, ".");
	    }
	  }
	  else{
	    DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, " ");
	  }
	}
	DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "\n");
      }
    }
  }
  DEBUGPRINT(DEBUGPRINT_LEVEL_OBLIGATORY, DEBUGPRINT_NORMAL, "********************************************************\n");
  {
    std::stringstream s;
    if (fatal)
      s << "There was a protocol error: " << error << std::endl;
    else
      s << "There was a protocol warning: " << error << std::endl;
    s << std::endl;
#if !defined(WIN32) && !defined(__APPLE__)
    s << "You use GNU/Linux version: please see your terminal for more information." << std::endl;
#endif
#if defined(WIN32)
    s << "You use Windows version: please see stdout.txt for more information." << std::endl;
#endif
#if defined(__APPLE__)
    s << "You use MacOSX: please see Console.app (in /Applications/Utilities) for more information." << std::endl;
#endif
    s << std::endl;
    s << "Report this bug if you are able to reproduce it." << std::endl;
    s << "Tell us what we have to do to reproduce the bug, and provide us with" << std::endl;
    s << "last 20 lines of output as described above." << std::endl;
    s << std::endl;
    s << "In your report please also include debug record file " << g_recordfilename << "." << std::endl;
    s << "Note, this will include information which can be used to identify" << std::endl;
    s << "your character, your VIP list, your full chat log for the session" << std::endl;
    s << "(including private messages). It will not include username, password," << std::endl;
    s << "or server name." << std::endl;
    s << std::endl;
    NativeGUIError(s.str().c_str(), "Protocol error");
  }
}

void Notifications::openMessageWindow(WindowMessage_t type, const std::string& message)
{
  if(type == MESSAGE_MOTD) {
    std::string text;
    int motdnum;
    sscanf(message.c_str(), "%d", &motdnum);

    text = message.substr(message.find('\n')+1);
    std::cout << "MOTD: " << message << std::endl;
  }
  else{
    std::cout << "Message: " << message << std::endl;
  }
}


void Notifications::openWaitingList(const std::string& message, int time) {
  std::stringstream message2;
  message2 << message << std::endl << std::endl << "Retry in " << time << "seconds";
  std::cout << "Waiting list: " << message2.str() << std::endl;
}

void Notifications::onTextMessage(MessageType_t type, const std::string& message)
{
  std::cout << "Text message: " << message << std::endl;
}

void Notifications::onEnterGame()
{
  std::cout << "Entering game" << std::endl;
}

void Notifications::onCancelWalk(Direction direction)
{
  std::cout << "Cancel walk" << std::endl;
}

void Notifications::onCancelAttack()
{
  GlobalVariables::setAttackID(0);
}

void Notifications::onWalk()
{
  std::cout << "Walk" << std::endl;
}

void Notifications::onCreatureSpeak(SpeakClasses_t type, int n, const std::string& name, int level, const Position& pos, const std::string& message)
{
  std::cout << "Creature speak: " << name << " (" << level << " @ " << pos.x << ", " << pos.y << "): " << message << std::endl;
}

void Notifications::onCreatureSpeak(SpeakClasses_t type, int n, const std::string& name, int level, int channel, const std::string& message)
{
  std::cout << "Creature speak: " << name << " (" << level << " in channel " << channel << "): " << message << std::endl;
}

void Notifications::onCreatureSpeak(SpeakClasses_t type, int n, const std::string& name, int level, const std::string& message)
{
  std::cout << "Creature speak: " << name << " (" << level << "): " << message << std::endl;
}

void Notifications::onCreatureMove(uint32_t id, const Position& oldPos, const Position& newPos)
{
  std::cout << "Creature move " << oldPos.x << ", " << oldPos.y << " -> " << newPos.x << ", " << newPos.y << std::endl;
}

void Notifications::onChangeStats()
{
  std::cout << "Stats have changed" << std::endl;
}

void Notifications::onChangeSkills()
{
  std::cout << "Skills have changed" << std::endl;
}

//open/close container
void Notifications::openContainer(int cid)
{
  std::cout << "Container opened: " << cid << std::endl;
}

void Notifications::closeContainer(int cid)
{
  std::cout << "Container closed: " << cid << std::endl;
}

//open/close shop
void Notifications::openShopWindow(const std::list<ShopItem>& itemlist)
{
  std::cout << "Shop window opening" << std::endl;
}
void Notifications::closeShopWindow()
{
  std::cout << "Shop window closing" << std::endl;
}

//safe trade
void Notifications::openTradeWindow(bool ack)
{
  std::cout << "Trade window opening" << std::endl;
}

void Notifications::closeTradeWindow()
{
  std::cout << "Trade window closing" << std::endl;
}

// when we receive player cash, we may need to update shop window
void Notifications::onUpdatePlayerCash(uint32_t newcash) {
  std::cout << "Player cash updated: " << newcash << std::endl;
}
//tutorials
void Notifications::showTutorial(uint8_t tutorialId)
{
  std::cout << "Tutorial id: " << tutorialId << std::endl;
}

//minimap mark
void Notifications::addMapMark(uint8_t icon, const Position& pos, const std::string& desc)
{
}

// open the outfit window with these outfits as available
void Notifications::openOutfitWindow(const Outfit_t& current, const std::list<AvailOutfit_t>& available)
{
  std::cout << "Outfit window opening" << std::endl;
}

// when tile is updated...
void Notifications::onTileUpdate(const Position& pos)
{
  std::cout << "Tile updated at " << pos.x << ", " << pos.y << std::endl;
}

void Notifications::openItemText(int windowId, int itemid, int maxTextLenght,
	const std::string& text, const std::string& lastChange,
	const std::string& lastChangeDate)
{
  std::cout << "Item text open: " << text << std::endl;
}

void Notifications::openRelogin()
{
  std::cout << "You have died" << std::endl;
}

void Notifications::onChangeIcons()
{
  std::cout << "Icons changed" << std::endl;
}
void Notifications::onAddCreature(uint32_t id)
{
  std::cout << "Creature added: " << id << std::endl;
}
void Notifications::onRemoveCreature(uint32_t id)
{
  std::cout << "Creature removed: " << id << std::endl;
}
void Notifications::onCreatureChangeHealth(uint32_t id, uint32_t health)
{
  std::cout << "Creature " << id << " health now " << health << std::endl;
}


void Notifications::onVipLogin(uint32_t id)
{
  std::cout << "VIP login: " << id << std::endl;
}
void Notifications::onVipLogout(uint32_t id)
{
  std::cout << "VIP logout: " << id << std::endl;
}
void Notifications::onVipState(uint32_t id, const std::string& name, bool state)
{
  std::cout << "VIP " << id << " state changed: " << name << ", " << (state ? "on" : "off") << std::endl;
}
