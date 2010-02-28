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

#ifndef __YATC_MAP_H__
#define __YATC_MAP_H__

#include <vector>
#include <map>
#include <list>
#include <string>
#include <bitset>

#include "position.h"

#include "../effectui.h"
#include "../distanceui.h"

#include "../stdinttypes.h"

#include "enums.h"
#include "../fassert.h"

class Item;
class Creature;
class Thing;
class ObjectType;

typedef std::vector<Thing*> ThingVector;

#define MAP_LAYER 16

enum MSG_RANGE
{
    MSG_SAY, MSG_WHISP, MSG_YELL, MSG_PVT
};

class DistanceEffect : public DistanceUI
{
public:
	DistanceEffect(const Position& from, const Position& to, uint32_t type);
	virtual ~DistanceEffect(){};
};

class Effect : public EffectUI
{
public:
	Effect(uint32_t type);
	virtual ~Effect(){};
};

class AnimatedText{
public:
	AnimatedText(const Position& pos, uint32_t color, const std::string& text);
	virtual ~AnimatedText(){}

	uint32_t getStartTime(){ return m_startTime;}
	uint32_t getColor() { return m_color;}
	const std::string& getText() { return m_text;}
	const Position& getPosition() { return m_pos;}

	bool canBeDeleted();

private:
	uint32_t m_startTime;
	uint32_t m_color;
	std::string m_text;
	Position m_pos;
};

class PublicMessage{
public:
    PublicMessage(TextColor_t color, const std::string& text, const std::string& sender, const Position& pos, bool showName, int linecount, MSG_RANGE range);

	uint32_t getStartTime(){ return m_startTime;}
	TextColor_t getColor() { return m_color;}
	const std::string& getText() { return m_text;}
	const std::string& getSender() { return m_sender; }
	const Position& getPosition() { return m_pos;}
	bool shouldShowName(){ return m_showName; }
    int getLinecount() { return m_linecount;}

    bool canBeDeleted();
    void set_handled(bool handled);
    bool is_handled() {return m_handled; }

    void set_relativePos(int pos);
    int get_relativePos() {return m_relativePos; }
    MSG_RANGE get_range() {return m_range; }
private:
    uint32_t m_startTime;
    TextColor_t m_color;
    std::string m_text;
    std::string m_sender;
    Position m_pos;
    bool m_showName;
    int m_linecount;

    bool m_handled;
    int m_relativePos;
    MSG_RANGE m_range;
};

class PrivateMessage{
public:
    PrivateMessage(TextColor_t color, const std::string& text, const std::string& sender, int linecount);

    void setOnScreen(bool screen);
    bool onScreen(){ return m_onscreen; }

	uint32_t getStartTime(){ return m_startTime;}
	TextColor_t getColor() { return m_color;}
	const std::string& getText() { return m_text;}
	const std::string& getSender() { return m_sender; }
    int getLinecount() { return m_linecount;}

    bool canBeDeleted();

private:
    uint32_t m_startTime;
    TextColor_t m_color;
    std::string m_text;
    std::string m_sender;
    Position m_pos;
    bool m_showName;
    int m_linecount;

    bool m_handled;
    int m_relativePos;
    MSG_RANGE m_range;

    bool m_onscreen;
};

class Tile{
public:
	Tile();
	~Tile();

	void clear();
	int32_t getThingCount() const;
	bool insertThing(Thing* thing, int32_t stackpos);
	bool removeThing(Thing* thing);
	bool addThing(Thing* thing, bool pushThing = false);
	Thing* getThingByStackPos(int32_t pos);
	const Thing* getThingByStackPos(int32_t pos) const;
	const Creature* getTopCreature() const;
	const Item* getGround() const;
	int getUseStackpos() const;
	int getExtendedUseStackpos() const;
	uint8_t  getMinimapColor() const;
	uint16_t getSpeedIndex() const;

	typedef std::list<Effect*> EffectList;
	void addEffect(uint32_t effect);
	EffectList& getEffects() { return m_effects;}

    const Position& getPos() const { return m_position; }

    bool isTileBlocking() const;

    bool blockPath() const;

private:
	Item* m_ground;
	ThingVector m_objects;
	EffectList m_effects;
	Position m_position;

	void setPos(const Position& pos) { m_position = pos; }

	friend class Map;
};

class Map
{
public:
	~Map();

	static Map& getInstance() {
		static Map instance;
		return instance;
	}

	void clear();

	//selects an empty tile a assigns it to x,y,z
	Tile* setTile(uint32_t x, uint32_t y, uint32_t z);
	Tile* setTile(const Position& pos) {return setTile(pos.x, pos.y, pos.z);}

	//returns the tile at position x,y,z
	const Tile* getTile(uint32_t x, uint32_t y, uint32_t z) const;
	const Tile* getTile(const Position& pos) const;
	Tile* getTile(uint32_t x, uint32_t y, uint32_t z);
	Tile* getTile(const Position& pos) {return getTile(pos.x, pos.y, pos.z);}

	bool playerCanSee(int32_t x, int32_t y, int32_t z);

	typedef std::list<DistanceEffect*> DistanceEffectList;
	void addDistanceEffect(const Position& from, const Position& to, uint32_t type);
	DistanceEffectList& getDistanceEffect(uint8_t floor) { ASSERTFRIENDLY(floor < 16, "Attempted to grab distance effect on invalid floor");  return m_distanceEffects[floor];}

	typedef std::list<AnimatedText> AnimatedTextList;
	void addAnimatedText(const Position& pos, uint32_t color, const std::string& text);
	AnimatedTextList& getAnimatedTexts(uint8_t floor) { return m_animatedTexts[floor];}

	typedef std::list<PublicMessage> PublicMessageList;
	typedef std::list<PrivateMessage> PrivateMessageList;
	void addPublicMessage(const Position& pos, TextColor_t color, const std::string& text, const std::string& sender, MSG_RANGE range, bool showName = true);
	void addPrivateMessage(TextColor_t color, const std::string& text, const std::string& sender);
	PublicMessageList& getPublicMessages() { return m_publicMessages; }
	PrivateMessageList& getPrivateMessages() { return m_privateMessages; }

	std::list<Direction> getPathTo(int x, int y, int z);

private:
	Map();

	typedef std::map<uint64_t, uint16_t> CoordMap;
	CoordMap m_coordinates;
    #ifndef WINCE
        #define TILES_CACHE 4096
    #else
        #define TILES_CACHE 512
    #endif
	Tile m_tiles[TILES_CACHE];

	typedef std::vector<uint32_t> FreeTiles;
	FreeTiles m_freeTiles;

	AnimatedTextList m_animatedTexts[16];
	DistanceEffectList m_distanceEffects[16];
	PublicMessageList m_publicMessages;
	PrivateMessageList m_privateMessages;
};
#endif
