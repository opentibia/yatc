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

#include "../creatureui.h"
#include "thing.h"
#include "enums.h"
#include "position.h"

class Tile;

class Creature :  public CreatureUI, public Thing
{
public:
	virtual ~Creature() {}

	virtual uint32_t getID() const {return m_id;}
	virtual uint32_t getOrder() const { return 4;}

	virtual Creature* getCreature() {return this;}
	virtual const Creature* getCreature() const {return this;}

	bool isPlayer() const { return ((m_id & 0x10000000) != 0) && ((m_id & 0x20000000) == 0); }
	bool isMonster() const { return ((m_id & 0x40000000) != 0); }
	bool isNpc() const { return ((m_id & 0x10000000) != 0) && ((m_id & 0x20000000) != 0); }

	void setSquare(uint32_t color);
	uint32_t getSquareColor() const { return m_squareColor;}
	uint32_t getSquareStart() const { return m_squareStartTime;}

	void setMoving(const Position& oldPos, const Position& newPos);

	void setId(uint32_t id){ m_id = id;}
	uint32_t getId() const { return m_id;}

	void setName(const std::string& name) { m_name = name;}
	const std::string& getName() const { return m_name;}

	void setHealth(uint32_t health) { m_health = health;}
	uint32_t getHealth() {return m_health;}

	void setLookDir(Direction dir) { m_lookdir = dir;}
	Direction getLookDir() const { return m_lookdir;}

	void setTurnDir(Direction dir) { m_turndir = dir;}
	Direction getTurnDir() const { return m_turndir;}

	Outfit_t& getOutfit() { return m_outfit;}
	const Outfit_t& getOutfit() const { return m_outfit;}

	void setLightLevel(uint32_t level){ m_lightLevel = level;}
	uint32_t getLightLevel() const { return m_lightLevel;}

	void setLightColor(uint32_t color){ m_lightColor = color;}
	uint32_t getLightColor() const { return m_lightColor;}

	void setSpeed(uint32_t speed) { m_speed = speed;}
	uint32_t getSpeed() const { return m_speed;}

	void setSkull(uint32_t skull) { m_skull = skull;}
	uint32_t getSkull() const { return m_skull;}

	void setShield(uint32_t shield) { m_shield = shield;}
	uint32_t getShield() const { return m_shield;}

	void setEmblem(uint32_t emblem) { m_emblem = emblem;}
	uint32_t getEmblem() const { return m_emblem;}

	void setImpassable(bool impassable) { m_impassable = impassable;}
	bool isImpassable() const { return m_impassable;}

	const Position& getCurrentPos() const { return m_currentPos; }
	void setCurrentPos(const Position& pos) { m_currentPos = pos; }

	void resetSelf() { CreatureUI::resetSelf(); m_id = 0; }
protected:
	Creature();

	uint32_t m_id;
	std::string m_name;
	uint32_t m_health;
	Direction m_lookdir;
	Direction m_turndir;

	Outfit_t m_outfit;

	uint32_t m_lightLevel;
	uint32_t m_lightColor;
	uint32_t m_speed;
	uint32_t m_skull;
	uint32_t m_shield;
	uint32_t m_emblem;
	bool m_impassable;

	uint32_t m_squareColor;
	uint32_t m_squareStartTime;

	uint32_t m_moveStartTime;
	Position m_moveOldPos;

	Position m_currentPos;

	friend class Creatures;
};

class Creatures
{
public:
	~Creatures() {}

	static Creatures& getInstance(){
	    if (!m_instance) m_instance = new Creatures;
		return *m_instance;
	}
	static void destroyInstance() {
	    delete m_instance;
	    m_instance = NULL;
	}
	bool loadGfx();
	void unloadGfx();
	void clear();

	Creature* getPlayer();
	Creature* getCreature(uint32_t id);

	Creature* addCreature(uint32_t id);
	void removeCreature(uint32_t id);

	Creature* lookup(const std::string& name);

	static uint32_t OutfitLookupTable[];

private:

	static int16_t reserveCreature(uint32_t id);
	// Thomac says client can handle 250 now (see OTServ rev 4969)
	#define CREATURES_ARRAY 250
	static Creature m_creaturesArray[CREATURES_ARRAY];

	Creatures() {}

	typedef std::map<uint32_t, uint16_t> CreatureMap;
	CreatureMap m_creaturesId;

	static Creatures* m_instance;
};

#endif
