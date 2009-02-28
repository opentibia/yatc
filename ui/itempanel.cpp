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

#include "itempanel.h"
#include "../gamecontent/itemcontainer.h"
#include "../skin.h"
#include "../gm_gameworld.h"
#include "../net/protocolgame.h"
#include "../options.h"

extern int g_lastmousebutton;

ItemPanel::ItemPanel(ItemContainer* container, uint32_t slot, const Position& pos)
{
	SetHeight(32);
	SetWidth(32);
	SetBGColor(.1,.1,.1,1);
	SetCaption("");
	SetOnPaint(ItemPanel::itemPanelOnPaint);
	SetOnClick(ItemPanel::itemPanelOnClick);
	SetOnMouseUp(ItemPanel::itemPanelOnMouseUp);
	SetOnMouseDown(ItemPanel::itemPanelOnMouseDown);
	SetSkin(&g_skin.inv);

	m_uiBack = false;
	m_container = container;
	m_slot = slot;
	m_pos = pos;
}

ItemPanel::~ItemPanel()
{
	//
}

Item* ItemPanel::getItem()
{
	return m_container->getItem(m_slot);
}

void ItemPanel::itemPanelOnPaint(glictRect *real, glictRect *clipped, glictContainer *caller)
{
	GM_Gameworld *gw = (GM_Gameworld*)g_game;
	ItemPanel* _this = (ItemPanel*)caller;

    if(ptrx > clipped->left && ptrx < clipped->right && ptry > clipped->top && ptry < clipped->bottom && gw->isDragging())
		g_engine->drawRectangleLines(clipped->left, clipped->top, clipped->right-clipped->left, clipped->bottom-clipped->top, oRGBA(255,255,255,255));

	Item* item = _this->getItem();
	if(item){
		item->Blit((int)real->left, (int)real->top, 1.f);
	}
	else if(_this->m_uiBack){
		g_engine->getUISprite()->Blit((int)real->left, (int)real->top, _this->m_uiBackx, _this->m_uiBacky, 32, 32);
	}
}

void ItemPanel::itemPanelOnClick(glictPos* relmousepos, glictContainer* callerclass)
{
	GM_Gameworld* gameclass = (GM_Gameworld*)g_game;
	ItemPanel* _this = (ItemPanel*)callerclass;
	Item* item = _this->getItem();
	if(!item)
		return;

    if(g_lastmousebutton == SDL_BUTTON_LEFT){
		if(gameclass->isExtendedUsing()){
			gameclass->performExtendedUse(_this->m_pos, item, 0);
		}
		else{
			if(SDL_GetModState() & KMOD_SHIFT){
				gameclass->m_protocol->sendLookItem(_this->m_pos, item->getID(), 0);
			}
			else if(SDL_GetModState() & KMOD_CTRL){
				if(options.classiccontrol == 0){
					if(!item->isExtendedUseable()) {
						gameclass->m_protocol->sendUseItem(_this->m_pos, item->getID(), 0);
					}
					else{
						gameclass->beginExtendedUse(item, 0, _this->m_pos);
					}
				}
				else{
					gameclass->performPopup(ItemPanel::itemPanelMakePopup, _this, NULL);
				}
			}
		}
	}
	else if(g_lastmousebutton == SDL_BUTTON_RIGHT){
		if(SDL_GetModState() & KMOD_SHIFT){
			gameclass->m_protocol->sendLookItem(_this->m_pos, item->getID(), 0);
		}
		else{
			gameclass->performPopup(ItemPanel::itemPanelMakePopup, _this, NULL);
		}
    }

}

void ItemPanel::itemPanelOnMouseDown(glictPos* relmousepos, glictContainer* callerclass)
{
	GM_Gameworld *gw = (GM_Gameworld*)g_game;
	ItemPanel* _this = (ItemPanel*)callerclass;
	Item* item = _this->getItem();
	if(!item){
		return;
	}

	if(g_lastmousebutton == SDL_BUTTON_LEFT){
		gw->startDrag(_this->m_pos, 0, item->getID(), item->getCount());
    }
    else if(g_lastmousebutton == SDL_BUTTON_RIGHT && options.classiccontrol == 1 &&
			!((SDL_GetModState() & KMOD_CTRL) || (SDL_GetModState() & KMOD_SHIFT))){
		if(!item->isExtendedUseable()) {
			gw->m_protocol->sendUseItem(_this->m_pos, item->getID(), 0);
		}
		else{
			gw->beginExtendedUse(item, 0, _this->m_pos);
		}
		gw->doNotShowPopup();
    }
}

void ItemPanel::itemPanelOnMouseUp(glictPos* relmousepos, glictContainer* callerclass)
{
	GM_Gameworld *gw = (GM_Gameworld*)g_game;
	ItemPanel* _this = (ItemPanel*)callerclass;

	if(g_lastmousebutton == SDL_BUTTON_LEFT){
		if(gw->isDragging()){
			gw->dragComplete(_this->m_pos);
		}
	}
}

void ItemPanel::itemPanelMakePopup(Popup* popup, void* owner, void* arg)
{
	ItemPanel* _this = (ItemPanel*)owner;
    //GM_Gameworld *gw = (GM_Gameworld*)g_game;
    Item* item = _this->getItem();

    if(!item){
        popup->prepareToDie();
        return;
    }

    std::stringstream look,use,trade;
    look << gettext("Look") << " (Shift)";
    if(item->isExtendedUseable()){
        use << gettext("Use with...") << " (Ctrl)";
    }
    else{
        if(Objects::getInstance()->getItemType(item->getID())->container){
            use << gettext("Open") << " (Ctrl)";
        }
        else{
            use << gettext("Use") << " (Ctrl)";
        }
    }

    trade << gettext("Trade with") << " ...";
    popup->addItem(look.str(), onLookAt, (void*)_this);
    popup->addItem(use.str(), onUse, (void*)_this);
    popup->addItem("-", NULL, NULL);
    popup->addItem(trade.str(), onTrade, (void*)_this);
}

void ItemPanel::onLookAt(Popup::Item* menuentry)
{
	GM_Gameworld *gw = (GM_Gameworld*)g_game;
	ItemPanel* _this = (ItemPanel*)menuentry->data;

	Item* item = _this->getItem();
	if(item){
		gw->m_protocol->sendLookItem(_this->m_pos, item->getID(), 0);
	}
}

void ItemPanel::onUse(Popup::Item* menuentry)
{
	GM_Gameworld *gw = (GM_Gameworld*)g_game;
	ItemPanel* _this = (ItemPanel*)menuentry->data;

    Item* item = _this->getItem();
    if(item){
		if(!item->isExtendedUseable()){
			gw->m_protocol->sendUseItem(_this->m_pos, item->getID(), 0);
		}
		else{
			gw->beginExtendedUse(item, 0, _this->m_pos);
		}
    }
}

void ItemPanel::onTrade(Popup::Item* menuentry)
{
	GM_Gameworld *gw = (GM_Gameworld*)g_game;
    ItemPanel* _this = (ItemPanel*)menuentry->data;

    Item* item = _this->getItem();
    if(item){
		gw->beginTrade(item, 0, _this->m_pos);
    }
}
