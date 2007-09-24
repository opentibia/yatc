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

#include <algorithm>

#include "map.h"
#include "item.h"
#include "creature.h"
#include "globalvars.h"

Tile::Tile()
{
	m_ground = NULL;
	m_used = false;
}

Tile::~Tile()
{
	clear();
}

Thing* Tile::getThingByStackPos(int32_t pos)
{
	if(m_ground){
		if(pos == 0)
			return m_ground;

		pos--;
	}

	int size = m_objects.size();
	if(pos < size){
		return m_objects[pos];
	}

	return NULL;
}

const Thing* Tile::getThingByStackPos(int32_t pos) const
{
	if(m_ground){
		if(pos == 0)
			return m_ground;

		pos--;
	}

	int size = m_objects.size();
	if(pos < size){
		return m_objects[pos];
	}

	return NULL;
}

const Item* Tile::getGround() const
{
	return m_ground;
}

void Tile::clear()
{
	delete m_ground;
	m_ground = NULL;

	for(ThingVector::iterator it = m_objects.begin(); it != m_objects.end(); ++it){
		Thing::deleteThing(*it);
	}

	m_objects.clear();
	m_effects.clear();
}

int32_t Tile::getThingCount() const
{
	int32_t objectCount = m_objects.size();
	return (m_ground ? 1 : 0) + objectCount;
}

bool Tile::insertThing(Thing *thing, int32_t stackpos)
{
	if(thing == NULL){
		return false;
	}

	if(getThingCount() == 10){
		return false;
	}

	int pos = stackpos;

	Item* item = thing->getItem();
	if(!item){
		return false;
	}

	if(item->isGroundTile() && pos == 0){
		m_ground = item;
		return true;
	}

	if(item->isGroundTile() && pos != 0){
		return false;
	}

	if(m_ground){
		--pos;
	}

	ThingVector::iterator it = m_objects.begin();
	while(pos > 0 && it != m_objects.end()){
		pos--;
		++it;
	}
	if(pos > 0){
		return false;
	}

	m_objects.insert(it, item);
	return true;
}

bool Tile::removeThing(Thing *thing)
{
	if(thing == NULL){
		return false;
	}

	if(thing == m_ground){
		m_ground = NULL;
		return true;
	}

	ThingVector::iterator it = std::find(m_objects.begin(), m_objects.end(), thing);
	if(it != m_objects.end()){
		m_objects.erase(it);
		return true;
	}

	return false;
}

bool Tile::addThing(Thing *thing)
{
	if(thing == NULL){
		return false;
	}

	if(getThingCount() == 10){
		Thing* pushThing = getThingByStackPos(9);
		if(!removeThing(pushThing)){
			return false;
		}
		Thing::deleteThing(pushThing);
	}

	Item* item = thing->getItem();

	if(item && item->isGroundTile()){
		if(m_ground != NULL){
			return false;
		}

		m_ground = item;
		return true;
	}

	int thingOrder = thing->getOrder();

	ThingVector::iterator it = m_objects.begin();

	for(it = m_objects.begin(); it != m_objects.end(); ++it){
		int itThingOrder = (*it)->getOrder();
		if(itThingOrder > thingOrder){
			break;
		}
	}

	m_objects.insert(it, thing);
	return true;
}

void Tile::addEffect(uint32_t effect, double time)
{
	TileEffect tileEffect;
	tileEffect.effect = effect;
	tileEffect.time = time;
	m_effects.push_front(tileEffect);
}

//*************** Map **************************

#define POS2INDEX(x, y, z) ((uint64_t)(x & 0xFFFF) << 24 | (y & 0xFFFF) << 8 | (z & 0xFF))

Map::Map()
{
	//
}

Map::~Map()
{
	clear();
}

void Map::clear()
{
	for(uint32_t i = 0; i < TILES_CACHE; ++i){
		m_tiles[i].clear();
		m_tiles[i].m_used = false;
	}
	m_coordinates.clear();
}

Tile* Map::setTile(uint32_t x, uint32_t y, uint32_t z)
{
	Tile* tile = getTile(x, y, z);
	if(!tile){
		//search unused tile
		for(uint32_t i = 0; i < TILES_CACHE; ++i){
			if(m_tiles[i].m_used == false){
				internalPrepareTile(i, x, y, z);
				return &m_tiles[i];
			}
			else{
				const Position& tilePos = m_tiles[i].getPosition();
				if(!playerCanSee(tilePos.x, tilePos.y, tilePos.z)){
					uint64_t oldPosIndex = POS2INDEX(tilePos.x, tilePos.y, tilePos.z);
					CoordMap::iterator it = m_coordinates.find(oldPosIndex);
					if(it != m_coordinates.end()){
						m_coordinates.erase(it);
					}
					internalPrepareTile(i, x, y, z);
					return &m_tiles[i];
				}
			}
		}
		// TODO (mips_act#3#): Handle error, trying to create a tile but there isnt any free slot for it!
		return NULL;
	}
	else{
		return tile;
	}
}

void Map::internalPrepareTile(uint32_t i, uint32_t x, uint32_t y, uint32_t z)
{
	uint64_t posIndex = POS2INDEX(x, y, z);
	m_coordinates[posIndex] = i;
	m_tiles[i].setPosition(x, y, z);
	m_tiles[i].m_used = true;
}

Tile* Map::getTile(uint32_t x, uint32_t y, uint32_t z)
{
	uint64_t posIndex = POS2INDEX(x, y, z);
	CoordMap::iterator it = m_coordinates.find(posIndex);
	if(it != m_coordinates.end()){
		return &m_tiles[it->second];
	}
	else{
		return NULL;
	}
}

const Tile* Map::getTile(uint32_t x, uint32_t y, uint32_t z) const
{
	uint64_t posIndex = POS2INDEX(x, y, z);
	CoordMap::const_iterator it = m_coordinates.find(posIndex);
	if(it != m_coordinates.end()){
		return &m_tiles[it->second];
	}
	else{
		return NULL;
	}
}

bool Map::playerCanSee(int32_t x, int32_t y, int32_t z)
{
	const Position& playerPos = GlobalVariables::getPlayerPosition();
	if(playerPos.z <= 7){
		//we are on ground level or above (7 -> 0)
		//view is from 7 -> 0
		if(z > 7){
			return false;
		}
	}
	else if(playerPos.z >= 8){
		//we are underground (8 -> 15)
		//view is +/- 2 from the floor we stand on
		if(std::abs((int32_t)playerPos.z - z) > 2){
			return false;
		}
	}

	//negative offset means that the action taken place is on a lower floor than ourself
	int offsetz = playerPos.z - z;

	if((x >= (int32_t)playerPos.x - 9 + offsetz) && (x <= (int32_t)playerPos.x + 10 + offsetz) &&
		(y >= (int32_t)playerPos.y - 7 + offsetz) && (y <= (int32_t)playerPos.y + 8 + offsetz)){
		return true;
	}

	return false;
}
