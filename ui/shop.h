#ifndef __UI_SHOP_H
#define __UI_SHOP_H

#include <GLICT/container.h>
#include <GLICT/panel.h>
#include <GLICT/textbox.h>
#include <GLICT/button.h>
#include <GLICT/window.h>

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

	glictPanel pnlPrice;
	glictPanel pnlMoney;
	glictPanel pnlItem;

	glictButton btnBuy;
	glictButton btnSell;

	std::list<glictPanel*> lsiBuy;
	std::list<glictPanel*> lsiSell;

	winShop_t()
	{
		// TODO
	}

	~winShop_t(){}
	// TODO
};

#endif
