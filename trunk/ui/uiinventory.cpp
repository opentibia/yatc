#include <sstream>
#include "uiinventory.h"
#include "../gm_gameworld.h"
#include "../gamecontent/enums.h"
#include "../gamecontent/item.h"
#include "../gamecontent/inventory.h"
#include "../net/protocolgame.h"
void pnlInventory_t::inventoryItemOnPaint(glictRect *real, glictRect *clipped, glictContainer *caller)
{

	slots_t slot = (slots_t)((glictPanel*)caller - (glictPanel*)caller->GetCustomData() + 1); // customdata stores the address of pnlItem[0], and caller is pnlItem[slot]...


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
	}

}
void pnlInventory_t::inventoryItemOnClick(glictPos *relmousepos,
	glictContainer* callerclass)
{
    GM_Gameworld* gameclass = (GM_Gameworld*)g_game;
    slots_t slotid = (slots_t)((glictPanel*)callerclass -
        (glictPanel*)callerclass->GetCustomData() + 1);
    Position p(0xFFFF, slotid, 0);
    if(slotid >= 0 && slotid <= 10)
    {
        Item* item = Inventory::getInstance().getItem(slotid);
        if(SDL_GetModState() & KMOD_CTRL && item)
            gameclass->m_protocol->sendUseItem(p, item->getID(), 0);
		else
		if(SDL_GetModState() & KMOD_SHIFT && item)
            gameclass->m_protocol->sendLookItem(p, item->getID(), 0);
	}
}

void pnlInventory_t::inventoryItemOnMouseUp(glictPos *relmousepos,
	glictContainer* callerclass)
{
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
    GM_Gameworld *gw = ((GM_Gameworld*)g_game);

    slots_t slotid = (slots_t)((glictPanel*)callerclass -
        (glictPanel*)callerclass->GetCustomData() + 1);
    if(slotid >= 0 && slotid <= 10) {
        gw->setDragInv(slotid);
    } else {
        gw->setDragInv(SLOT_NONE);
    }
}

