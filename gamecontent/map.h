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

class Item;
class Creature;
class Thing;

typedef std::vector<Thing*> ThingVector;

#define MAP_LAYER 16

class Position
{
public:
	Position()
	{
		x = y = z = 0;
	}
	Position(uint32_t _x, uint32_t _y, uint32_t _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}
	~Position(){};

	uint32_t x, y, z;
};

class Tile{
public:
	Tile();
	~Tile();

	void clear();
	int32_t getThingCount() const;
	bool insertThing(Thing* thing, int32_t stackpos);
	bool removeThing(Thing* thing);
	bool addThing(Thing* thing);
	Thing* getThingByStackPos(int32_t pos);
	const Thing* getThingByStackPos(int32_t pos) const;
	const Item* getGround() const;

	struct TileEffect{
		uint32_t effect;
		double time;
	};

	typedef std::list<TileEffect> EffectList;

	void addEffect(uint32_t effect, double time);
	EffectList& getEffects(){return m_effects;}
	const EffectList& getEffects() const {return m_effects;}

	const Position& getPosition() const { return m_pos;}
	void setPosition(uint32_t x, uint32_t y, uint32_t z){
		m_pos.x = x;
		m_pos.y = y;
		m_pos.z = z;
	}

private:
	Item* m_ground;
	ThingVector m_objects;
	Position m_pos;

	bool m_used;

	EffectList m_effects;

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
	Tile* getTile(uint32_t x, uint32_t y, uint32_t z);
	Tile* getTile(const Position& pos) {return getTile(pos.x, pos.y, pos.z);}

	bool playerCanSee(int32_t x, int32_t y, int32_t z);

private:
	Map();
	void internalPrepareTile(uint32_t i, uint32_t x, uint32_t y, uint32_t z);

	typedef std::map<uint64_t, uint16_t> CoordMap;
	CoordMap m_coordinates;
	#define TILES_CACHE 4096
	Tile m_tiles[TILES_CACHE];
};

#endif
