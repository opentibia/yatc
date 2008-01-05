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

#ifndef __YATC_CREATURE_H__
#define __YATC_CREATURE_H__

#include <string>
#include <map>

#include "creatureui.h"
#include "thing.h"
#include "enums.h"
#include "map.h"

class Tile;

class Creature :  public CreatureUI, public Thing
{
public:
	virtual ~Creature() {}

	virtual uint32_t getID() const {return m_id;}
	virtual uint32_t getOrder() const { return 4;}

	virtual Creature* getCreature() {return this;}
	virtual const Creature* getCreature() const {return this;}

	bool isPlayer() { return (m_id & 0x10000000 != 0) && (m_id & 0x20000000 == 0); }
	bool isMonster() { return (m_id & 0x40000000 != 0); }
	bool isNpc() { return (m_id & 0x10000000 != 0) && (m_id & 0x20000000 != 0); }

	void setSquare(uint32_t color);
	uint32_t getSquareColor(){ return m_squareColor;}
	uint32_t getSquareStart() { return m_squareStartTime;}

	void setMoving(const Position& oldPos);

	void setId(uint32_t id){ m_id = id;}
	uint32_t getId() { return m_id;}

	void setName(const std::string& name) { m_name = name;}
	const std::string& getName() { return m_name;}

	void setHealth(uint32_t health) { m_health = health;}
	uint32_t getHealth() {return m_health;}

	void setLookDir(Direction dir) { m_lookdir = dir;}
	Direction getLookDir(){ return m_lookdir;}

	Outfit_t& getOutfit() { return m_outfit;}
	const Outfit_t& getOutfit() const { return m_outfit;}

	void setLightLevel(uint32_t level){ m_lightLevel = level;}
	uint32_t getLightLevel() { return m_lightLevel;}

	void setLightColor(uint32_t color){ m_lightColor = color;}
	uint32_t getLightColor() { return m_lightColor;}

	void setSpeed(uint32_t speed) { m_speed = speed;}
	uint32_t getSpeed() { return m_speed;}

	void setSkull(uint32_t skull) { m_skull = skull;}
	uint32_t getSkull() { return m_skull;}

	void setShield(uint32_t shield) { m_shield = shield;}
	uint32_t getShield() { return m_shield;}

protected:
	Creature();

	uint32_t m_id;
	std::string m_name;
	uint32_t m_health;
	Direction m_lookdir;

	Outfit_t m_outfit;

	uint32_t m_lightLevel;
	uint32_t m_lightColor;
	uint32_t m_speed;
	uint32_t m_skull;
	uint32_t m_shield;

	uint32_t m_squareColor;
	uint32_t m_squareStartTime;

	uint32_t m_moveStartTime;
	Position m_moveOldPos;

	friend class Creatures;
};

class Creatures
{
public:
	~Creatures() {}

	static Creatures& getInstance(){
		static Creatures instance;
		return instance;
	}
	void clear();

	Creature* getPlayer();
	Creature* getCreature(uint32_t id);

	Creature* addCreature(uint32_t id);
	void removeCreature(uint32_t id);

	static uint32_t OutfitLookupTable[];

private:

	static int16_t reserveCreature(uint32_t id);
	// 150 is the size for 7.8-8.0 clients
	#define CREATURES_ARRAY 150
	static Creature m_creaturesArray[CREATURES_ARRAY];

	Creatures() {}

	typedef std::map<uint32_t, uint16_t> CreatureMap;
	CreatureMap m_creaturesId;
};

#endif
