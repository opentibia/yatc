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
#include "../stackpanel.h"

#include <sstream>

class winShop_t: public yatcStackPanelWindow {
public:
	glictScrollbar sbCt;

	glictList lstSell;
	glictList lstBuy;

    glictPanel pnlAmountLeft, pnlAmountRight;
	glictPanel pnlPriceLeft, pnlPriceRight;
	glictPanel pnlMoneyLeft, pnlMoneyRight;

	glictPanel pnlItem;

	glictButton btnOk;
	glictButton btnBuy;
	glictButton btnSell;

	std::list<glictPanel*> lsiBuy;
	std::list<glictPanel*> lsiSell;

	ShopItem currentBuyItem;
	ShopItem currentSellItem;

    glictPanel *currentpnl;
    uint32_t cash;

	winShop_t();
	virtual ~winShop_t();


	void generateList(const std::list<ShopItem>& list);
    void destroyList();
	void addItemBuy (const ShopItem& itm);
	void addItemSell (const ShopItem& itm);
	void setCash(uint32_t newplayercash);

    void rebuildImage();

	void OnClose();

	float GetDefaultHeight(){ return 141; };

	static void OnListbox(glictPos* pos, glictContainer *caller);
    static void OnChangeCount(glictPos* pos, glictContainer *caller);
    static void OnBuyClick(glictPos* pos, glictContainer *caller);
    static void OnSellClick(glictPos* pos, glictContainer *caller);
    static void OnOkClick(glictPos* pos, glictContainer *caller);

    static void drawObject(glictRect *real, glictRect *clipped, glictContainer *caller);

    bool selling;
    Item* dispItem;
};

#endif
