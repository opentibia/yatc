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

#ifndef __YATC_SHOP_H
#define __YATC_SHOP_H

#include <string>

class ShopItem {
public:
    ShopItem() {}
	ShopItem(const std::string& iname, uint16_t id, uint8_t type, uint32_t buy, uint32_t sell)
		: name(iname), itemid(id), subtype(type), buyprice(buy), sellprice(sell), extra(NULL) {}
	~ShopItem(){}

	std::string getName() const { return name; }
	uint16_t getItemId() const { return itemid; }
	uint8_t getSubType() const { return subtype; }
	uint32_t getBuyPrice() const { return buyprice; }
	uint32_t getSellPrice() const { return sellprice; }

	void setExtraData(void* _extra) { extra = _extra; }
	void*getExtraData() const { return extra; }

private:
	std::string name;
	uint16_t itemid;
	uint8_t subtype;
	uint32_t buyprice;
	uint32_t sellprice;
	void* extra;
};

#endif
