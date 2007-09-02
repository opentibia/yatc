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
		delete *it;
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
Map::Map()
{
	//
}

Map::~Map()
{
	for(uint32_t i = 0; i < TILES_CACHE; ++i){
		m_tiles[i].clear();
		m_tiles[i].m_used = false;
	}
	m_coordinates.clear();
}

Tile* Map::setTile(const Position& pos)
{
	return setTile(pos.x, pos.y, pos.z);
}

Tile* Map::setTile(uint32_t x, uint32_t y, uint32_t z)
{
	Tile* tile = getTile(x, y, z);
	if(!tile){
		//search unused tile
		for(unsigned int i = 0; i < TILES_CACHE; ++i){
			if(m_tiles[i].m_used == false){
				uint64_t posIndex =
					(x & 0xFFFF) << 24 | (y & 0xFFFF) << 8 | (z & 0xFF);
				m_coordinates[posIndex] = i;
				m_tiles[i].m_used = true;
				return &m_tiles[i];
			}
		}
		// TODO (mips_act#3#): Handle error, trying to create a tile but there isnt any free slot for it!
		return NULL;
	}
	else{
		return tile;
	}
}

Tile* Map::getTile(const Position& pos)
{
	return getTile(pos.x, pos.y, pos.z);
}

Tile* Map::getTile(uint32_t x, uint32_t y, uint32_t z)
{
	uint64_t posIndex =
		(x & 0xFFFF) << 24 | (y & 0xFFFF) << 8 | (z & 0xFF);
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
	uint64_t posIndex =
		(x & 0xFFFF) << 24 | (y & 0xFFFF) << 8 | (z & 0xFF);
	CoordMap::const_iterator it = m_coordinates.find(posIndex);
	if(it != m_coordinates.end()){
		return &m_tiles[it->second];
	}
	else{
		return NULL;
	}
}

void Map::cleanMap()
{
	// TODO (mips_act#3#): Implement getPlayerPos()
	/*
	const Position& playerPos = getPlayerPos();
	CoordMap::iterator it;
	for(it = m_coordinates.begin(); it != m_coordinates.end(); ){
		int32_t x, y, z;
		x = (it->first >> 24) & 0xFFFF;
		y = (it->first >> 8) & 0xFFFF;
		z = (it->first & 0xFF);
		if(std::abs(x - (int32_t)playerPos.x - (z - (int32_t)playerPos.z)) > 18 ||
		  std::abs(y - (int32_t)playerPos.y - (z - (int32_t)playerPos.z)) > 14){
			m_tiles[it->second].clear();
			m_tiles[it->second].m_used = false;
			m_coordinates.erase(it++);
		}
		else{
			++it;
		}
	}
	*/
}
