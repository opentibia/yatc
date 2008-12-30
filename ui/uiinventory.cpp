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

#include <sstream>
#include "uiinventory.h"
#include "../gm_gameworld.h"
#include "../gamecontent/enums.h"
#include "../gamecontent/item.h"
#include "../gamecontent/inventory.h"
#include "../net/protocolgame.h"
#include "../popup.h"

extern int g_lastmousebutton;

void pnlInventory_t::inventoryItemOnPaint(glictRect *real, glictRect *clipped, glictContainer *caller)
{
    GM_Gameworld* gw = (GM_Gameworld*)g_game;

	slots_t slot = (slots_t)((glictPanel*)caller - (glictPanel*)caller->GetCustomData() + 1); // customdata stores the address of pnlItem[0], and caller is pnlItem[slot]...

    if(ptrx > clipped->left && ptrx < clipped->right && ptry > clipped->top && ptry < clipped->bottom && gw->isDragging())
        g_engine->drawRectangleLines(clipped->left, clipped->top, clipped->right-clipped->left, clipped->bottom-clipped->top, oRGBA(255,255,255,255));

	Item* item=Inventory::getInstance().getItem(slot);
	if (item){
		item->Blit((int)real->left, (int)real->top, 1.f); // TODO (ivucica#5#) if item is not 32x32, scale appropriately
	} else {
		int int_slot;
		switch (slot) {
		default:
		case SLOT_NECKLACE:
			int_slot = 0;
			break;
		case SLOT_HEAD:
			int_slot = 1;
			break;
		case SLOT_BACKPACK:
			int_slot = 2;
			break;
		case SLOT_LEFT:
			int_slot = 3;
			break;
		case SLOT_RIGHT:
			int_slot = 4;
			break;
		case SLOT_ARMOR:
			int_slot = 5;
			break;
		case SLOT_LEGS:
			int_slot = 6;
			break;
		case SLOT_RING:
			int_slot = 7;
			break;
		case SLOT_AMMO:
			int_slot = 8;
			break;
		case SLOT_FEET:
			int_slot = 9;
			break;
		}

		g_engine->getUISprite()->Blit((int)real->left, (int)real->top, 96 + 32 * (int_slot % 5), 32 * (int_slot / 5), 32, 32);

	}

	if (0) {
		std::stringstream s, count;

		if (item)
			if (item->getCount()>1)
				count << (int)item->getCount() << "x";

		s << "Slot " << (int)slot << "\n" << count.str();


		g_engine->drawText(s.str().c_str(), "minifont", (int)real->left + 2, (int)real->top + 2, TEXTCOLOR_WHITE);
	} else if (item)
        if (item->getCount()>1) {
            std::stringstream s;
            s << (int)item->getCount();
            g_engine->drawText(s.str().c_str(), "gamefont", (int)real->right - g_engine->sizeText(s.str().c_str(), "gamefont") - 2, (int)real->bottom - 12, TEXTCOLOR_WHITE);
        }

}
void pnlInventory_t::inventoryItemOnClick(glictPos *relmousepos,
	glictContainer* callerclass)
{
    GM_Gameworld* gameclass = (GM_Gameworld*)g_game;
    slots_t slotid = (slots_t)((glictPanel*)callerclass -
        (glictPanel*)callerclass->GetCustomData() + 1);

    if (g_lastmousebutton == SDL_BUTTON_RIGHT) {
        gameclass->performPopup(pnlInventory_t::inventoryItemMakePopup,&gameclass->pnlInventory,(void*)slotid);
        return;
    }

    Position p(0xFFFF, slotid, 0);
    if(slotid >= 0 && slotid <= 10)
    {
        Item* item = Inventory::getInstance().getItem(slotid);

        if (gameclass->isExtendedUsing()) {
            gameclass->performExtendedUse(p, item, 0);
        } else
        if(SDL_GetModState() & KMOD_CTRL && item)
            if (!item->isExtendedUseable()) {
                gameclass->m_protocol->sendUseItem (p, item->getID(), 0);
            } else {
                gameclass->beginExtendedUse(item, 0, p);
            }
		else
		if(SDL_GetModState() & KMOD_SHIFT && item)
            gameclass->m_protocol->sendLookItem(p, item->getID(), 0);
	}
}

void pnlInventory_t::inventoryItemOnMouseUp(glictPos *relmousepos,
	glictContainer* callerclass)
{
    if (g_lastmousebutton != SDL_BUTTON_LEFT)
        return;
    GM_Gameworld *gw = ((GM_Gameworld*)g_game);
	if (gw->isDragging())
	{
        slots_t slotid = (slots_t)((glictPanel*)callerclass -
			(glictPanel*)callerclass->GetCustomData() + 1);
		if(slotid >= 0 && slotid <= 10) {
            Position dest(0xFFFF, slotid, 0);
            gw->dragComplete(dest);
		}
        gw->dismissDrag();
	}
}

void pnlInventory_t::inventoryItemOnMouseDown(glictPos *relmousepos,
	glictContainer* callerclass)
{
    if (g_lastmousebutton != SDL_BUTTON_LEFT)
        return;
    GM_Gameworld *gw = ((GM_Gameworld*)g_game);

    slots_t slotid = (slots_t)((glictPanel*)callerclass -
        (glictPanel*)callerclass->GetCustomData() + 1);
    if(slotid >= 0 && slotid <= 10) {
        gw->setDragInv(slotid);
    } else {
        gw->setDragInv(SLOT_NONE);
    }
}




void pnlInventory_t::inventoryItemMakePopup(Popup*popup,void*owner,void*arg){
    pnlInventory_t* p = (pnlInventory_t*)owner;
    GM_Gameworld *gw = ((GM_Gameworld*)g_game);
    slots_t slotid = ((slots_t)(VOIDP2INT(arg)));
    Item* item = Inventory::getInstance().getItem(slotid);

    if (!item) {
        popup->prepareToDie();
        return;
    }

    std::stringstream look,use,trade;
    look << gettext("Look") << " (Shift)";
    if (item->isExtendedUseable())
        use << gettext("Use with...") << " (Ctrl)";
    else
        if (Objects::getInstance()->getItemType(item->getID())->container)
            use << gettext("Open") << " (Ctrl)";
        else
            use << gettext("Use") << " (Ctrl)";


    trade << gettext("Trade with") << " ...";
    popup->addItem(look.str(),onLookAt,(void*)slotid);
    popup->addItem(use.str(),onUse,(void*)slotid);
    popup->addItem("-",NULL,NULL);
    popup->addItem(trade.str(),onTrade,(void*)slotid);
}

void pnlInventory_t::onLookAt(Popup::Item* pi){
    GM_Gameworld *gw = ((GM_Gameworld*)g_game);
    slots_t slotid = ((slots_t)(VOIDP2INT(pi->data)));
    Position p(0xFFFF, slotid, 0);
    Item* item = Inventory::getInstance().getItem(slotid);

    gw->m_protocol->sendLookItem(p,item->getID(), 0);
}
void pnlInventory_t::onUse(Popup::Item* pi){
    GM_Gameworld *gw = ((GM_Gameworld*)g_game);
    slots_t slotid = ((slots_t)(VOIDP2INT(pi->data)));
    Position p(0xFFFF, slotid, 0);
    Item* item = Inventory::getInstance().getItem(slotid);

    if (!item->isExtendedUseable()) {
        gw->m_protocol->sendUseItem (p, item->getID(), 0);
    } else {
        gw->beginExtendedUse(item, 0, p);
    }
}
void pnlInventory_t::onTrade(Popup::Item*){
    GM_Gameworld *gw = ((GM_Gameworld*)g_game);
    gw->msgBox(gettext("This functionality is not yet finished"),"TODO");
}

