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

#include "inventory.h"
#include "item.h"

Inventory::Inventory()
{
	for(uint32_t i = SLOT_FIRST; i < SLOT_LAST; ++i){
		m_inventory[i] = NULL;
	}
}

Inventory::~Inventory()
{
	clear();
}

void Inventory::clear()
{
	for(uint32_t i = SLOT_FIRST; i < SLOT_LAST; ++i){
		delete m_inventory[i];
	}
}

Item* Inventory::getItem(uint32_t slot)
{
	if(slot < SLOT_FIRST || slot >= SLOT_LAST){
		// TODO (mips_act#3#): Handle error...
		return NULL;
	}
	return m_inventory[slot];
}

bool Inventory::addItem(uint32_t slot, Item* item)
{
	if(slot < SLOT_FIRST || slot >= SLOT_LAST){
		// TODO (mips_act#3#): Handle error...
		return false;
	}
	if(m_inventory[slot]){
		delete m_inventory[slot];
	}

	m_inventory[slot] = item;
	return true;
}

bool Inventory::removeItem(uint32_t slot)
{
	if(slot < SLOT_FIRST || slot >= SLOT_LAST){
		// TODO (mips_act#3#): Handle error...
		return false;
	}
	delete m_inventory[slot];
	m_inventory[slot] = NULL;
	return true;
}
