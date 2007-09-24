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

#ifndef __YATC_CONTAINER_H__
#define __YATC_CONTAINER_H__

#include "objects.h"
#include <string>
#include <list>

class Item;

class Container
{
public:
	~Container();

	Item* getItem(uint32_t slot);
	bool addItem(Item* item);
	bool removeItem(uint32_t slot);
	bool updateItem(uint32_t slot, Item* newitem);

	void setItemId(uint16_t itemid) { m_itemid = itemid;}
	uint16_t getItemId() { return m_itemid;}

	void setName(const std::string& name){ m_name = name;}
	std::string getName() { return m_name;}

	void setCapacity(uint32_t cap){ m_capacity = cap;}
	uint32_t getCapacity(){ return m_capacity;}

	void setHasParent(bool v){ m_hasParent = v;}
	bool getHasParent() { return m_hasParent;}

	int32_t getSize(){ return m_items.size();}

private:

	Container();

	typedef std::list<Item*> ItemList;
	ItemList m_items;

	uint16_t m_itemid;
	uint32_t m_capacity;
	std::string m_name;
	bool m_hasParent;

	friend class Containers;
};


class Containers
{
public:
	#define MAX_ALLOWED_CONTAINERS 32
	~Containers();
	static Containers& getInstance(){
		static Containers instance;
		return instance;
	}

	void clear();

	Container* getContainer(uint32_t id);
	Container* createContainer(uint32_t id);
	bool deleteContainer(uint32_t id);


	Container* newTradeContainer();
	void closeTradeContainer();
	Container* getTradeContainer() { return m_tradeContainer;}

private:

	Container* m_tradeContainer;
	Container* m_containers[MAX_ALLOWED_CONTAINERS];

	Containers();
};

#endif