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

	uint32_t m_itemid;
	std::string m_name;
	uint32_t m_capacity;
	bool m_hasParent;
	uint32_t m_size;

	Item* getItem(uint32_t slot);
	bool addItem(Item* item);
	bool removeItem(uint32_t slot);
	bool updateItem(uint32_t slot, Item* newitem);

private:
	Container();

	typedef std::list<Item*> ItemList;
	ItemList m_items;

	friend class Containers;
};


class Containers
{
public:
	#define MAX_ALLOWED_CONTAINERS 32
	~Containers();
	static Containers* getInstance(){ return &m_instance; }

	Container* getContainer(uint32_t id);
	Container* createContainer(uint32_t id);
	bool deleteContainer(uint32_t id);

	void clear();

private:

	Array<Container*> m_containers;
	Containers();
	static Containers m_instance;
};

#endif
