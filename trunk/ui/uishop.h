#ifndef __UI_SHOP_H
#define __UI_SHOP_H

#include <GLICT/container.h>
#include <GLICT/panel.h>

#include <GLICT/button.h>
#include <GLICT/window.h>
#include <GLICT/list.h>

#include "../gamecontent/item.h"
#include "../gamecontent/shop.h"
#include "../skin.h"

#include <sstream>

class winShop_t {
public:
	glictWindow window;

	glictScrollbar sbCt;

	glictList lstSell;
	glictList lstBuy;

    glictPanel pnlAmountLeft, pnlAmountRight;
	glictPanel pnlPriceLeft, pnlPriceRight;
	glictPanel pnlMoneyLeft, pnlMoneyRight;

	glictPanel pnlItem;

	glictButton btnBuy;
	glictButton btnSell;
    glictButton btnOk;

	std::list<glictPanel*> lsiBuy;
	std::list<glictPanel*> lsiSell;

	ShopItem currentBuyItem;
	ShopItem currentSellItem;

	winShop_t();

	~winShop_t();



	void generateList(const std::list<ShopItem>& list);
    void destroyList();
	void addItem (const ShopItem& itm);

    void rebuildImage();

	static void OnListbox(glictPos* pos, glictContainer *caller);
    static void OnChangeCount(glictPos* pos, glictContainer *caller);

    static void drawObject(glictRect *real, glictRect *clipped, glictContainer *caller);


    Item* dispItem;
};

#endif
