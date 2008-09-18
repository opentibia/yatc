#ifndef __YATC_SHOP_H
#define __YATC_SHOP_H

#include <string>

class ShopItem {
public:
    ShopItem() {}
	ShopItem(const std::string& iname, uint16_t id, uint8_t charges, uint32_t buy, uint32_t sell)
		: name(iname), itemid(id), runecharges(charges), buyprice(buy), sellprice(sell) {}
	~ShopItem(){}

	std::string getName() const { return name; }
	uint16_t getItemId() const { return itemid; }
	uint8_t getCharges() const { return runecharges; }
	uint32_t getBuyPrice() const { return buyprice; }
	uint32_t getSellPrice() const { return sellprice; }

	void setExtraData(void* _extra) { extra = _extra; }
	void*getExtraData() const { return extra; }

private:
	std::string name;
	uint16_t itemid;
	uint8_t runecharges;
	uint32_t buyprice;
	uint32_t sellprice;
	void* extra;
};

#endif
