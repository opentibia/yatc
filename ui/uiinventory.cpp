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
#include "../options.h"
#include "../engine.h"

extern int g_lastmousebutton;


pnlInventory_t::pnlInventory_t()
{
	struct slotPos{ int x, y, uisprite; };
	static slotPos posInvSlots[] = {
		{45, 3, 1},  // helmet
		{8, 17, 0},  // necklace
		{83, 17, 2}, // backpack
		{45, 40, 5}, // armor
		{83, 54, 4}, // right weapon
		{8, 54, 3},  // left weapon
		{45, 77, 6}, // legs
		{45, 115, 9},// feet
		{8, 91, 7},  // ring
		{83, 91, 8} // ammo
	};

	panel.SetHeight(170);
	panel.SetWidth(150);
	#if (GLICT_APIREV >= 98)
		panel.SetDraggable(true);
		panel.SetSkin(&g_skin.background);
		panel.SetFocusable(true);
	#else
		#warning Inventory is not properly drawn until you upgrade to GLICT 98+
		panel.SetCaption(gettext("Inventory"));
	#endif

	panel.AddObject(&btnOptions);
	btnOptions.SetCaption(gettext("Options"));
	btnOptions.SetWidth(50);
	btnOptions.SetHeight(20);
	btnOptions.SetPos(120, 126);
	btnOptions.SetFont("minifont");
	btnOptions.SetOnClick(&pnlInventory_t::onClick_Options);

	panel.AddObject(&pnlSoul);
	pnlSoul.SetPos(8, 128);
	pnlSoul.SetHeight(19);
	pnlSoul.SetWidth(32);
	pnlSoul.SetSkin(&g_skin.inv);
	pnlSoul.SetOnPaint(pnlInventory_t::paintSoul);
	//pnlSoul.SetBGActiveness(false);

	panel.AddObject(&pnlCap);
	pnlCap.SetPos(83, 128);
	pnlCap.SetHeight(20);
	pnlCap.SetWidth(32);
	pnlCap.SetSkin(&g_skin.inv);
	pnlCap.SetOnPaint(pnlInventory_t::paintCap);
	//pnlCap.SetBGActiveness(false);

	// TODO: fix this to the proper skin (background should match capacity and soul)
	panel.AddObject(&pnlIcons);
	pnlIcons.SetPos(8, 152);
	pnlIcons.SetHeight(20);
	pnlIcons.SetWidth(105);
	pnlIcons.SetSkin(&g_skin.txt);
	pnlIcons.SetOnPaint(pnlInventory_t::paintIcons);
	//pnlIcons.SetBGActiveness(false);


	for (int i = 0; i < 10; i++) {
		pnlItem[i] = new ItemPanel(&Inventory::getInstance(), i+1, Position(0xFFFF, i+1, 0), 0);
		panel.AddObject(pnlItem[i]);
		pnlItem[i]->SetPos(posInvSlots[i].x, posInvSlots[i].y);
		pnlItem[i]->setUIBackground(96 + 32 * (posInvSlots[i].uisprite % 5), 32 * (posInvSlots[i].uisprite / 5));
	}
}

pnlInventory_t::~pnlInventory_t()
{
	for(int i = 0; i < 10; i++){
		delete pnlItem[i];
		pnlItem[i] = NULL;
	}
}

void pnlInventory_t::paintCap(glictRect *real, glictRect *clipped, glictContainer *caller)
{
	std::string text = yatc_itoa(GlobalVariables::getPlayerStat(STAT_CAPACITY));
	g_engine->drawText("Cap:", "minifont", (int)real->left + 6, (int)real->top);
	float len = g_engine->sizeText(text.c_str(), "aafont");
	float width = real->right - real->left;
	if(len > width)
		len = width;
	int diff = std::floor((width - len) / 2.f);
	g_engine->drawText(text.c_str(), "aafont", (int)real->left + diff, (int)real->top + 10);
}
void pnlInventory_t::paintSoul(glictRect *real, glictRect *clipped, glictContainer *caller)
{
	std::string text = yatc_itoa(GlobalVariables::getPlayerStat(STAT_SOUL));
	g_engine->drawText("Soul:", "minifont", (int)real->left + 6, (int)real->top);
	float len = g_engine->sizeText(text.c_str(), "aafont");
	float width = real->right - real->left;
	if(len > width)
		len = width;
	int diff = std::floor((width - len) / 2.f);
	g_engine->drawText(text.c_str(), "aafont", (int)real->left + diff, (int)real->top + 10);
}
void pnlInventory_t::paintIcons(glictRect *real, glictRect *clipped, glictContainer *caller)
{
}
