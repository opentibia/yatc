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


#ifndef __YATC_INVENTORY_H__
#define __YATC_INVENTORY_H__

#include "stdint.h"
#include "enums.h"

class Item;

class Inventory{
public:
	~Inventory();

	static Inventory& getInstance() {
		static Inventory instance;
		return instance;
	}

	void clear();

	Item* getItem(uint32_t slot);
	bool addItem(uint32_t slot, Item* item);
	bool removeItem(uint32_t slot);


protected:
	Inventory();

	Item* m_inventory[SLOT_LAST];
};


#endif
