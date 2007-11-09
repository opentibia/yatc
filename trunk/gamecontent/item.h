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

#ifndef __YATC_ITEM_H__
#define __YATC_ITEM_H__

#include "itemui.h"
#include "thing.h"
#include "objects.h"

class Item : public Thing, public ItemUI
{
public:
	static Item* CreateItem(const uint16_t type, const uint8_t _count = 0);
	virtual ~Item(){}

	virtual uint32_t getID() const {return m_id;}
	uint8_t getCount() const {return m_count;}

	bool isAlwaysOnTop() const { return m_it->alwaysOnTop; }
	uint32_t getAlwaysOnTopLevel() const { return m_it->alwaysOnTopOrder; }
	bool isGroundTile() const { return m_it->ground; }

	bool isRune() const { return m_it->rune; }
	bool isStackable() const { return m_it->stackable; }
	bool isSplash() const { return m_it->splash; }
	bool isFluidContainer() const { return m_it->fluidContainer; }

	const ObjectType* getObjectType() const {return m_it;}

	virtual uint32_t getOrder() const {return getAlwaysOnTopLevel();}

	virtual Item* getItem() {return this;}
	virtual const Item* getItem() const {return this;}

protected:

	Item(uint16_t id, uint8_t count);
	const ObjectType* m_it;
	uint16_t m_id;
	uint8_t m_count;
};

#endif
