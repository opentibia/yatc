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

#ifndef __YATC_ENUMS_H__
#define __YATC_ENUMS_H__

#include <string>
#include "../stdinttypes.h"

enum slots_t {
    SLOT_NONE = -1,

	SLOT_FIRST = 1,
	SLOT_HEAD = SLOT_FIRST,
	SLOT_NECKLACE = 2,
	SLOT_BACKPACK = 3,
	SLOT_ARMOR = 4,
	SLOT_RIGHT = 5,
	SLOT_LEFT = 6,
	SLOT_LEGS = 7,
	SLOT_FEET = 8,
	SLOT_RING = 9,
	SLOT_AMMO = 10,
	SLOT_LAST = SLOT_AMMO + 1
};

enum Stat_t{
	STAT_HEALTH,
	STAT_HEALTH_MAX,
	STAT_CAPACITY,
	STAT_EXPERIENCE,
	STAT_MANA,
	STAT_MANA_MAX,
	STAT_SOUL,
	STAT_STAMINA,
	STAT_LAST
};

enum SkillList_t{
	SKILL_LEVEL,
	SKILL_MAGIC,
	SKILL_FIST,
	SKILL_CLUB,
	SKILL_SWORD,
	SKILL_AXE,
	SKILL_DISTANCE,
	SKILL_SHIELD,
	SKILL_FISH,
	SKILL_LAST
};
enum SkillAttr_t{
	SKILL_ATTR_LEVEL,
	SKILL_ATTR_PERCENT,
	SKILL_ATTR_LAST
};

enum Direction
{
	DIRECTION_NORTH = 0,
	DIRECTION_EAST = 1,
	DIRECTION_SOUTH = 2,
	DIRECTION_WEST = 3,
	DIRECTION_NE = 4,
	DIRECTION_NW = 5,
	DIRECTION_SE = 6,
	DIRECTION_SW = 7
};

enum WindowMessage_t{
	MESSAGE_ERROR,
	MESSAGE_INFORMATION,
	MESSAGE_MOTD
};

enum Skulls_t{
	SKULL_NONE      = 0,
	SKULL_YELLOW    = 1,
	SKULL_GREEN     = 2,
	SKULL_WHITE     = 3,
	SKULL_RED       = 4,
	SKULL_BLACK		= 5, // added in Tibia 8.5
};

enum PartyShields_t{
	SHIELD_NONE = 0,
	SHIELD_WHITEYELLOW = 1,
	SHIELD_WHITEBLUE = 2,
	SHIELD_BLUE = 3,
	SHIELD_YELLOW = 4,
	SHIELD_BLUE_SHAREDEXP = 5,
	SHIELD_YELLOW_SHAREDEXP = 6,
	SHIELD_BLUE_NOSHAREDEXP_BLINK = 7,
	SHIELD_YELLOW_NOSHAREDEXP_BLINK = 8,
	SHIELD_BLUE_NOSHAREDEXP = 9,
	SHIELD_YELLOW_NOSHAREDEXP = 10
};

enum GuildEmblems_t{
	EMBLEM_NONE = 0,
	EMBLEM_GREEN = 1,
	EMBLEM_RED = 2,
	EMBLEM_BLUE = 3
};

enum MessageType_t{
	MSG_STATUS_CONSOLE_ORNG = 0x11, /*Orange message in the console (TODO (ivucica#1#): check with real client if it appears in game window and where)*/
	MSG_STATUS_WARNING      = 0x12, /*Red message in game window and in the console*/
	MSG_EVENT_ADVANCE       = 0x13, /*White message in game window and in the console*/
	MSG_EVENT_DEFAULT       = 0x14, /*White message at the bottom of the game window and in the console*/
	MSG_STATUS_DEFAULT      = 0x15, /*White message at the bottom of the game window and in the console*/
	MSG_INFO_DESCR          = 0x16, /*Green message in game window and in the console*/
	MSG_STATUS_SMALL        = 0x17, /*White message at the bottom of the game window"*/
	MSG_STATUS_CONSOLE_BLUE = 0x18, /*Blue message in the console*/
	MSG_STATUS_CONSOLE_RED  = 0x19, /*Red message in the console*/

	MSG_STATUS_CONSOLE_ORNG2        /*Second orange message type added in 8.4 */
};

enum SpeakClasses_t{
	SPEAK_SAY           = 0x01,
	SPEAK_WHISPER       = 0x02,
	SPEAK_YELL          = 0x03,

	SPEAK_PRIVATE       = 0x04, // player to player, before 8.2 this was 0x04, after this is 0x06

	SPEAK_BROADCAST     = 0x09,
	SPEAK_PRIVATE_RED   = 0x0B,	//@name@text

	SPEAK_CHANNEL_Y     = 0x05,	//yellow
	SPEAK_CHANNEL_R1    = 0x0A,	//red - #c text
	SPEAK_CHANNEL_O     = 0x0C,	//orange
	SPEAK_CHANNEL_R2    = 0x0E,	//red anonymous - #d text

	SPEAK_CHANNEL_UNK6  = 0x06,
	SPEAK_CHANNEL_UNK7  = 0x07,
	SPEAK_CHANNEL_UNK8  = 0x08,

	SPEAK_MONSTER_SAY   = 0x10,
	SPEAK_MONSTER_YELL  = 0x11,


	SPEAK_PRIVATE_PN, // player to npc, from 8.2 onwards
	SPEAK_PRIVATE_NP,  // npc to player, from 8.2 onwards
	SPEAK_RVR_CHANNEL, // initial send to ctrl+r channel, 8.2 onwards (?!)
	SPEAK_RVR_ANSWER, // answer to ctrl+r channel, 8.2 onwards (?!)
	SPEAK_RVR_CONTINUE, // answering answer to report, 8.2 onwards (?!)

    SPEAK_CHANNEL_W // white, 8.4 onwards
};

struct CharacterList_t{
	std::string name;
	std::string world;
	uint32_t ip;
	uint16_t port;
	void* extra;
};

struct ChannelList_t{
	int id;
	std::string name;
};

struct AvailOutfit_t{
	int id;
	std::string name;
	int addons;
};

struct Outfit_t{
	uint32_t m_looktype;
	uint32_t m_lookhead;
	uint32_t m_lookbody;
	uint32_t m_looklegs;
	uint32_t m_lookfeet;
	uint32_t m_lookitem;
	uint32_t m_addons;
};

enum TextColor_t {
        TEXTCOLOR_BLUE        = 5,
        TEXTCOLOR_LIGHTBLUE   = 35,
        TEXTCOLOR_LIGHTGREEN  = 30,
        TEXTCOLOR_PURPLE      = 83,
        TEXTCOLOR_LIGHTGREY   = 129,
        TEXTCOLOR_DARKRED     = 144,
        TEXTCOLOR_RED         = 180,
        TEXTCOLOR_ORANGE      = 198,
        TEXTCOLOR_YELLOW      = 210,
        TEXTCOLOR_WHITE       = 215,
        TEXTCOLOR_NONE        = 255
};

enum ConditionIcons_t{
	ICON_POISON     = 1,
	ICON_BURN       = 2,
	ICON_ENERGY     = 4,
	ICON_DRUNK      = 8,
	ICON_MANASHIELD = 16,
	ICON_PARALYZE   = 32,
	ICON_HASTE      = 64,
	ICON_SWORDS     = 128,
	ICON_DROWNING   = 256,
	ICON_FREEZING   = 512,
	ICON_DAZZLED    = 1024,
	ICON_CURSED     = 2048,
	ICON_PARTY_BUFF = 4096,
	ICON_PZBLOCK 	= 8192,
	ICON_PZ 		= 16384,
};


#endif
