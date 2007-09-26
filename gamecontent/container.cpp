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

#include "container.h"
#include "item.h"

//**********Container**************
Container::Container()
{
	m_itemid = 0;
	m_capacity = 20;
	m_hasParent = false;
}

Container::~Container()
{
	for(ItemList::iterator it = m_items.begin(); it != m_items.end(); ++it){
		delete *it;
	}
	m_items.clear();
}

Item* Container::getItem(uint32_t slot)
{
	if(slot >= m_capacity){
		// TODO (mips_act#3#): Handle error...
		return NULL;
	}

	uint32_t count = 0;
	ItemList::iterator it;
	for(it = m_items.begin(); it != m_items.end(); ++it){
		if(count == slot){
			return *it;
		}
		else{
			++count;
		}
	}
	return NULL;
}

bool Container::addItem(Item* item)
{
	if(m_items.size() == m_capacity){
		// TODO (mips_act#3#): Handle error...
		return false;
	}
	m_items.push_front(item);
	return true;
}

bool Container::removeItem(uint32_t slot)
{
	if(slot >= m_capacity){
		// TODO (mips_act#3#): Handle error...
		return false;
	}

	uint32_t count = 0;
	ItemList::iterator it;
	for(it = m_items.begin(); it != m_items.end(); ++it){
		if(count == slot){
			delete *it;
			m_items.erase(it);
			return true;
		}
		else{
			++count;
		}
	}
	return false;
}

bool Container::updateItem(uint32_t slot, Item* newitem)
{
	if(slot >= m_capacity){
		// TODO (mips_act#3#): Handle error...
		return false;
	}

	uint32_t count = 0;
	ItemList::iterator it;
	for(it = m_items.begin(); it != m_items.end(); ++it){
		if(count == slot){
			delete *it;
			m_items.insert(it, newitem);
			m_items.erase(it);
			return true;
		}
		else{
			++count;
		}
	}
	return false;
}


//**********Containers**************

Containers::Containers()
{
	for(uint32_t i = 0; i < MAX_ALLOWED_CONTAINERS; ++i){
		m_containers[i] = NULL;
	}
	m_tradeContainer = NULL;
}

Containers::~Containers()
{
	clear();
}

int32_t Containers::getFreeContainerSlot()
{
	for(uint32_t i = 0; i < MAX_ALLOWED_CONTAINERS; ++i){
		if(m_containers[i] == NULL){
			return i;
		}
	}
	return MAX_ALLOWED_CONTAINERS - 1;
}

Container* Containers::getContainer(uint32_t id)
{
	if(id >= MAX_ALLOWED_CONTAINERS){
		// TODO (mips_act#3#): Handle error...
		return NULL;
	}

	return m_containers[id];
}

Container* Containers::createContainer(uint32_t id)
{
	if(id >= MAX_ALLOWED_CONTAINERS){
		// TODO (mips_act#3#): Handle error...
		return NULL;
	}

	if(m_containers[id] != NULL){
		// TODO (mips_act#3#): Handle error...?
		delete m_containers[id];
	}
	Container* container = new Container;
	m_containers[id] = container;
	return container;
}

bool Containers::deleteContainer(uint32_t id)
{
	if(id >= MAX_ALLOWED_CONTAINERS){
		// TODO (mips_act#3#): Handle error...
		return false;
	}
	delete m_containers[id];
	m_containers[id] = NULL;
	return true;
}

Container* Containers::newTradeContainer()
{
	if(m_tradeContainer){
		// TODO (mips_act#3#): Handle error...
		delete m_tradeContainer;
	}
	m_tradeContainer = new Container();
	return m_tradeContainer;
}

void Containers::closeTradeContainer()
{
	delete m_tradeContainer;
	m_tradeContainer = NULL;
}

void Containers::clear()
{
	for(uint32_t i = 0; i < MAX_ALLOWED_CONTAINERS; ++i){
		delete m_containers[i];
		m_containers[i] = NULL;
	}
	delete m_tradeContainer;
	m_tradeContainer = NULL;
}
