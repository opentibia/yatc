#ifndef __YATC_SHOP_H
#define __YATC_SHOP_H

#include <string>

class ShopItem {
public:
	ShopItem(const std::string& iname, uint16_t id, uint8_t charges, uint32_t buy, uint32_t sell)
		: name(iname), itemid(id), runecharges(charges), buyprice(buy), sellprice(sell) {}
	~ShopItem(){}

	std::string getName() { return name; }
	uint16_t getItemId() { return itemid; }
	uint8_t getCharges() { return runecharges; }
	uint32_t getBuyPrice() { return buyprice; }
	uint32_t getSellPrice() { return sellprice; }

private:
	std::string name;
	uint16_t itemid;
	uint8_t runecharges;
	uint32_t buyprice;
	uint32_t sellprice;
};

#endif
