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

#include "uishop.h"

winShop_t::winShop_t() {
    window.SetCaption("NPC Trade");
    window.SetWidth(168);
    window.SetHeight(141);

    window.AddObject(&btnBuy);
    btnBuy.SetPos(74, 4);
    btnBuy.SetWidth(43);
    btnBuy.SetHeight(20);
    btnBuy.SetFont("minifont");
    btnBuy.SetCaption("Buy");
    btnBuy.SetHold(true);

    window.AddObject(&btnSell);
    btnSell.SetPos(121, 4);
    btnSell.SetWidth(43);
    btnSell.SetHeight(20);
    btnSell.SetFont("minifont");
    btnSell.SetCaption("Sell");
    btnSell.SetHold(false);

    window.AddObject(&lstBuy);
    lstBuy.SetPos(0,28);
    lstBuy.SetWidth(168);
    lstBuy.SetHeight(48);
    lstBuy.SetBGColor(.1,.1,.1,1.);
    lstBuy.SetVisible(true);

    window.AddObject(&lstSell);
    lstSell.SetPos(0,28);
    lstSell.SetWidth(168);
    lstSell.SetHeight(48);
    lstSell.SetBGColor(.1,.1,.1,1.);
    lstSell.SetVisible(false);

    window.AddObject(&sbCt);
    sbCt.SetPos(4,80);
    sbCt.SetWidth(113);
    sbCt.SetHeight(12);
    sbCt.SetOnClick(winShop_t::OnChangeCount);
    sbCt.SetCustomData(this);

    window.AddObject(&pnlAmountLeft);
    window.AddObject(&pnlAmountRight);
    pnlAmountLeft.SetPos(4,96);
    pnlAmountLeft.SetWidth(55);
    pnlAmountLeft.SetHeight(12);
    pnlAmountLeft.SetCaption("Amount:");
    pnlAmountLeft.SetBGActiveness(false);
    pnlAmountRight.SetPos(118,96); // 118 minus whatever amount should be in there
    pnlAmountRight.SetWidth(0);
    pnlAmountRight.SetHeight(12);
    pnlAmountRight.SetBGActiveness(false);

    window.AddObject(&pnlPriceLeft);
    window.AddObject(&pnlPriceRight);
    pnlPriceLeft.SetPos(4,112);
    pnlPriceLeft.SetWidth(55);
    pnlPriceLeft.SetHeight(12);
    pnlPriceLeft.SetCaption("Price:");
    pnlPriceLeft.SetBGActiveness(false);
    pnlPriceRight.SetPos(118,112); // 118 minus whatever amount should be in there
    pnlPriceRight.SetWidth(0);
    pnlPriceRight.SetHeight(12);
    pnlPriceRight.SetBGActiveness(false);

    window.AddObject(&pnlMoneyLeft);
    window.AddObject(&pnlMoneyRight);
    pnlMoneyLeft.SetPos(4,128);
    pnlMoneyLeft.SetWidth(55);
    pnlMoneyLeft.SetHeight(12);
    pnlMoneyLeft.SetCaption("Money:");
    pnlMoneyLeft.SetBGActiveness(false);
    pnlMoneyRight.SetPos(118,128); // 118 minus whatever amount should be in there
    pnlMoneyRight.SetWidth(0);
    pnlMoneyRight.SetHeight(12);
    pnlMoneyRight.SetBGActiveness(false);

    window.AddObject(&pnlItem);
    pnlItem.SetPos(130,80);
    pnlItem.SetWidth(34);
    pnlItem.SetHeight(34);
    pnlItem.SetOnPaint(&winShop_t::drawObject);
    pnlItem.SetBGColor(.1,.1,.1,1);
    pnlItem.SetSkin(&g_skin.inv);
    pnlItem.SetCustomData(this);

    window.AddObject(&btnOk);
    btnOk.SetPos(121,118);
    btnOk.SetWidth(43);
    btnOk.SetHeight(20);
    btnOk.SetFont("minifont");
    btnOk.SetCaption("Ok");

    dispItem = NULL;


}

winShop_t::~winShop_t() {
    if (lsiBuy.size())
        destroyList();
}

void winShop_t::drawObject(glictRect *real, glictRect *clipped, glictContainer *caller) {
    winShop_t* ws = (winShop_t*)(caller->GetCustomData());
    ws->dispItem->Blit((int)real->left, (int)real->top);
}

void winShop_t::destroyList() {
    for (std::list<glictPanel*>::iterator it = lsiBuy.begin(); it != lsiBuy.end(); it++) {
        #if (GLICT_APIREV >= 71)
        lstBuy.RemoveObject(*it);
        lstBuy.DelayedRemove();
        #else
        #warning Stuff wont work ok till you upgrade to GLICT APIREV < 71
        #endif

        delete (ShopItem*)((*it)->GetCustomData());
        delete (*it);
    }
    lsiBuy.clear();
    if (dispItem)
        delete dispItem;
    dispItem = NULL;
}



void winShop_t::generateList(const std::list<ShopItem>& list) {
    destroyList();
    for (std::list<ShopItem>::const_iterator it = list.begin(); it != list.end(); it++) {
        addItem(*it);
    }
    lstBuy.Focus(NULL);
}

void winShop_t::addItem (const ShopItem& itm) {
    std::stringstream s;

    s << itm.getName() << ": " << itm.getBuyPrice() << " gold";
    // FIXME (ivucica#5#) change "res" into "pnl"
    glictPanel *res = new glictPanel;
    ShopItem *data = new ShopItem(itm);

    data->setExtraData(this);

    res->SetBGActiveness(false);
    res->SetBGColor(.4,.4,.4,1.);
    res->SetOnClick(winShop_t::OnListbox);
    res->SetCustomData(data);
    res->SetCaption(s.str());
    res->SetFont("aafont");
    res->SetFocusable(false);
    #if GLICT_APIREV < 68
    #warning Aesthetic corrections on listboxes wont work without GLICT of apirev 68+
    #else
    res->SetTextOffset(4,2);
    #endif

    lsiBuy.push_back(res);
    lstBuy.AddObject(res);

    if (lsiBuy.size() == 1) {
        OnListbox(NULL, res);
    }
}


void winShop_t::OnListbox(glictPos* pos, glictContainer *caller) {
    winShop_t *wc = (winShop_t*) (((ShopItem*)caller->GetCustomData())->getExtraData());
    for (std::list<glictPanel*>::iterator it = wc->lsiBuy.begin(); it != wc->lsiBuy.end(); it++) {
        (*it)->SetBGActiveness(false);
    }
    ((glictPanel*)caller)->SetBGActiveness(true);
    wc->currentBuyItem = *((ShopItem*)(caller->GetCustomData()));

    wc->rebuildImage();
}

void winShop_t::OnChangeCount(glictPos* pos, glictContainer *caller) {
    winShop_t *wc = (winShop_t*) (caller->GetCustomData());
    wc->rebuildImage();
}

void winShop_t::rebuildImage() {
    if (dispItem)
        delete dispItem;

    dispItem = Item::CreateItem(currentBuyItem.getItemId(), (uint8_t)sbCt.GetValue());
}
