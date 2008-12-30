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
#include "../gm_gameworld.h"
#include "../net/protocolgame.h"
#if defined(HAVE_LIBINTL_H)
    #include <libintl.h>
#else
    #define gettext(x) (x)
#endif
winShop_t::winShop_t()
{
    window.SetCaption(gettext("NPC Trade"));
    window.SetWidth(168);
    window.SetHeight(141);

    #if (GLICT_APIREV >= 76)
	window.AddTitlebarObject(&closebtn);
	closebtn.SetCaption("x");
	closebtn.SetWidth(12);
	closebtn.SetHeight(12);
	closebtn.SetPos(150 + 10 - 12, 2);
	closebtn.SetCustomData(this);
	closebtn.SetOnClick(OnClose);

    #else
    #warning For titlebar objects (such as close buttons) to work properly, you need GLICT APIREV 76+
    #endif




    window.AddObject(&btnBuy);
    btnBuy.SetPos(74, 4);
    btnBuy.SetWidth(43);
    btnBuy.SetHeight(20);
    btnBuy.SetFont("minifont");
    btnBuy.SetCaption(gettext("Buy"));
    btnBuy.SetHold(true);
    btnBuy.SetOnClick(OnBuyClick);
    btnBuy.SetCustomData(this);

    window.AddObject(&btnSell);
    btnSell.SetPos(121, 4);
    btnSell.SetWidth(43);
    btnSell.SetHeight(20);
    btnSell.SetFont("minifont");
    btnSell.SetCaption(gettext("Sell"));
    btnSell.SetHold(false);
    btnSell.SetOnClick(OnSellClick);
    btnSell.SetCustomData(this);

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
    sbCt.SetValue(1);
    sbCt.SetMin(1);
    sbCt.SetMax(50);
    sbCt.SetOnClick(winShop_t::OnChangeCount);
    sbCt.SetCustomData(this);

    window.AddObject(&pnlAmountLeft);
    window.AddObject(&pnlAmountRight);
    pnlAmountLeft.SetPos(4,96);
    pnlAmountLeft.SetWidth(55);
    pnlAmountLeft.SetHeight(12);
    pnlAmountLeft.SetCaption(gettext("Amount:"));
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
    pnlPriceLeft.SetCaption(gettext("Price:"));
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
    pnlMoneyLeft.SetCaption(gettext("Money:"));
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
    btnOk.SetCaption(gettext("Ok"));
    btnOk.SetOnClick(&winShop_t::OnOkClick);
    btnOk.SetCustomData(this);

    selling = false;

    dispItem = NULL;
    currentpnl = NULL;


}

winShop_t::~winShop_t()
{
    if (lsiBuy.size() || lsiSell.size())
        destroyList();
}

void winShop_t::drawObject(glictRect *real, glictRect *clipped, glictContainer *caller)
{
    winShop_t* ws = (winShop_t*)(caller->GetCustomData());
    if (ws->dispItem) {
        ws->dispItem->Blit((int)real->left, (int)real->top);
        if (ws->dispItem->getCount()>1) {
            std::stringstream s;
            s << (int)ws->dispItem->getCount();
            g_engine->drawText(s.str().c_str(), "gamefont", (int)real->right - g_engine->sizeText(s.str().c_str(), "gamefont") - 2, (int)real->bottom - 12, TEXTCOLOR_WHITE);
        }
    }
}

void winShop_t::destroyList()
{
    for (std::list<glictPanel*>::iterator it = lsiBuy.begin(); it != lsiBuy.end(); it++) {
        #if (GLICT_APIREV >= 71)
        lstBuy.RemoveObject(*it);
        lstBuy.DelayedRemove();
        #else
        #warning Stuff wont work ok till you upgrade to GLICT APIREV >= 71
        #endif

        bool unique=true;
        for (std::list<glictPanel*>::iterator its = lsiSell.begin(); its != lsiSell.end(); its++) {
            if (((ShopItem*)(*its)->GetCustomData()) == ((ShopItem*)(*it)->GetCustomData())) {unique=false; break;}
        }
        if (unique)
            delete (ShopItem*)((*it)->GetCustomData());
        delete (*it);
    }
    lsiBuy.clear();

    for (std::list<glictPanel*>::iterator it = lsiSell.begin(); it != lsiSell.end(); it++) {
        #if (GLICT_APIREV >= 71)
        lstSell.RemoveObject(*it);
        lstSell.DelayedRemove();
        #else
        #warning Stuff wont work ok till you upgrade to GLICT APIREV >= 71
        #endif

        delete (ShopItem*)((*it)->GetCustomData());
        delete (*it);
    }
    lsiSell.clear();


    if (dispItem)
        delete dispItem;
    dispItem = NULL;

    currentpnl = NULL;
}



void winShop_t::generateList(const std::list<ShopItem>& list)
{
    destroyList();
    for (std::list<ShopItem>::const_iterator it = list.begin(); it != list.end(); it++) {
        addItemBuy(*it);
        addItemSell(*it);
    }
    lstBuy.Focus(NULL);

    rebuildImage();
}

void winShop_t::addItemBuy (const ShopItem& itm)
{
    if (!itm.getBuyPrice())
        return;
    std::stringstream s;

    s << itm.getName() << ": " << itm.getBuyPrice() << gettext(" gold");

    glictPanel *pnl = new glictPanel;
    ShopItem *data = new ShopItem(itm);

    data->setExtraData(this);

    pnl->SetBGActiveness(false);
    pnl->SetBGColor(.4,.4,.4,1.);
    pnl->SetOnClick(winShop_t::OnListbox);
    pnl->SetCustomData(data);
    pnl->SetCaption(s.str());
    pnl->SetFont("aafont");
    pnl->SetFocusable(false);
    #if GLICT_APIREV < 68
    #warning Aesthetic corrections on listboxes wont work without GLICT of apirev 68+
    #else
    pnl->SetTextOffset(4,2);
    #endif

    lsiBuy.push_back(pnl);
    lstBuy.AddObject(pnl);

    if (lsiBuy.size() == 1) {
        OnListbox(NULL, pnl);
    }
}




void winShop_t::addItemSell (const ShopItem& itm)
{
    if (!itm.getSellPrice()) return;
    std::stringstream s;

    s << itm.getName() << ": " << itm.getSellPrice() << gettext(" gold");

    glictPanel *pnl = new glictPanel;
    ShopItem *data = new ShopItem(itm);

    data->setExtraData(this);

    pnl->SetBGActiveness(false);
    pnl->SetBGColor(.4,.4,.4,1.);
    pnl->SetOnClick(winShop_t::OnListbox);
    pnl->SetCustomData(data);
    pnl->SetCaption(s.str());
    pnl->SetFont("aafont");
    pnl->SetFocusable(false);
    #if GLICT_APIREV < 68
    #warning Aesthetic corrections on listboxes wont work without GLICT of apirev 68+
    #else
    pnl->SetTextOffset(4,2);
    #endif

    lsiSell.push_back(pnl);
    lstSell.AddObject(pnl);

    if (lsiSell.size() == 1) {
        OnListbox(NULL, pnl);
    }
}




void winShop_t::OnListbox(glictPos* pos, glictContainer *caller)
{
    if (!caller)
        return;
    winShop_t *wc = (winShop_t*) (((ShopItem*)caller->GetCustomData())->getExtraData());
    for (std::list<glictPanel*>::iterator it = wc->lsiBuy.begin(); it != wc->lsiBuy.end(); it++) {
        (*it)->SetBGActiveness(false);
    }
    for (std::list<glictPanel*>::iterator it = wc->lsiSell.begin(); it != wc->lsiSell.end(); it++) {
        (*it)->SetBGActiveness(false);
    }
    ((glictPanel*)caller)->SetBGActiveness(true);
    wc->currentBuyItem = *((ShopItem*)(caller->GetCustomData()));
    wc->currentSellItem = *((ShopItem*)(caller->GetCustomData()));

    wc->currentpnl = (glictPanel*)caller;

    wc->rebuildImage();
    wc->sbCt.SetMin(1);
    if (wc->cash){
        if (!wc->selling) {
            if (wc->cash/wc->currentBuyItem.getBuyPrice()>1)
                wc->sbCt.SetMax(wc->cash/wc->currentBuyItem.getBuyPrice());
            else
                wc->sbCt.SetMax(30);
        }
        else
            wc->sbCt.SetMax(30);
    } else {
        wc->sbCt.SetMax(30);
    }
}

void winShop_t::OnChangeCount(glictPos* pos, glictContainer *caller)
{
    winShop_t *wc = (winShop_t*) (caller->GetCustomData());

    wc->rebuildImage();
}

void winShop_t::rebuildImage()
{
    if (dispItem)
        delete dispItem;

    uint32_t id=0;
    if (!selling)
        id = currentBuyItem.getItemId();
    else
        id = currentSellItem.getItemId();
    if (id)
        dispItem = Item::CreateItem(id, (uint8_t)sbCt.GetValue());
    else
        dispItem = NULL;

    std::stringstream ss;
    int newwidth;

    ss << sbCt.GetValue();
    newwidth = glictFontSize(ss.str().c_str(),"aafont");
    pnlAmountRight.SetCaption(ss.str());
    pnlAmountRight.SetPos(118-newwidth,96);
    pnlAmountRight.SetWidth(newwidth);

    ss.str("");
    if (!selling)
        ss << sbCt.GetValue() * currentBuyItem.getBuyPrice();
    else
        ss << sbCt.GetValue() * currentSellItem.getSellPrice();
    newwidth = glictFontSize(ss.str().c_str(),"aafont");
    pnlPriceRight.SetCaption(ss.str());
    pnlPriceRight.SetPos(118-newwidth,112);
    pnlPriceRight.SetWidth(newwidth);

    ss.str("");
    ss << cash;
    newwidth = glictFontSize(ss.str().c_str(),"aafont");
    pnlMoneyRight.SetCaption(ss.str());
    pnlMoneyRight.SetPos(118-newwidth,128);
    pnlMoneyRight.SetWidth(newwidth);
}

void winShop_t::OnBuyClick(glictPos* pos, glictContainer *caller)
{
    winShop_t *wst = (winShop_t*)caller->GetCustomData();
    wst->selling=false;
    wst->lstSell.SetVisible(false);
    wst->lstBuy.SetVisible(true);
    wst->btnSell.SetHold(false);
    wst->btnBuy.SetHold(true);
    wst->currentBuyItem = ShopItem("",0,0,0,0);
    wst->currentSellItem = ShopItem("",0,0,0,0);
    wst->sbCt.SetValue(1);
    wst->rebuildImage();

    wst->currentpnl = NULL;
    if (wst->lsiBuy.size())
        OnListbox(NULL, *wst->lsiBuy.begin());
}
void winShop_t::OnSellClick(glictPos* pos, glictContainer *caller)
{
    winShop_t *wst = (winShop_t*)caller->GetCustomData();
    wst->selling=true;
    wst->lstSell.SetVisible(true);
    wst->lstBuy.SetVisible(false);
    wst->btnSell.SetHold(true);
    wst->btnBuy.SetHold(false);
    wst->currentBuyItem = ShopItem("",0,0,0,0);
    wst->currentSellItem = ShopItem("",0,0,0,0);
    wst->sbCt.SetValue(1);
    wst->rebuildImage();

    wst->currentpnl = NULL;
    if (wst->lsiSell.size())
        OnListbox(NULL, *wst->lsiSell.begin());
}

void winShop_t::OnOkClick(glictPos* pos, glictContainer *caller)
{
    winShop_t *wst = (winShop_t*)caller->GetCustomData();
    GM_Gameworld *gw = ((GM_Gameworld*)g_game);
    if (!wst->selling) {
        printf("Buying..."); fflush(stdout);
        printf("itemid %d subtype %d count %d\n", wst->currentBuyItem.getItemId(),wst->currentBuyItem.getSubType(),
            /* amount: */wst->sbCt.GetValue());
        gw->m_protocol->sendShopPurchase(
            /* itemid: */wst->currentBuyItem.getItemId(),
            /* subtype: */wst->currentBuyItem.getSubType(),
            /* amount: */wst->sbCt.GetValue(),
            /* ignore cap*/false,
            /* with backpack*/false);

    }
    else {
        printf("Selling..."); fflush(stdout);
        printf("itemid %d subtype %d count %d\n", wst->currentSellItem.getItemId(),wst->currentSellItem.getSubType(),
                /* amount: */wst->sbCt.GetValue());


        gw->m_protocol->sendShopSale(
            /* itemid: */wst->currentSellItem.getItemId(),
            /* subtype: */wst->currentSellItem.getSubType(),
            /* amount: */wst->sbCt.GetValue());
    }
    printf("Done\n");
}

void winShop_t::OnClose(glictPos* pos, glictContainer *caller)
{
    GM_Gameworld *gw = ((GM_Gameworld*)g_game);
	winShop_t* window = (winShop_t*)caller->GetCustomData();

    gw->m_protocol->sendShopClose();
    window->window.SetVisible(false);
}
void winShop_t::setCash(uint32_t newcash)
{
    cash = newcash;
    OnListbox(NULL, currentpnl); // will rebuild image too
}
