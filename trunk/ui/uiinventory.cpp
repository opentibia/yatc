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
	pnlIcons.SetHeight(13);
	pnlIcons.SetWidth(108);
	//pnlIcons.SetSkin(&g_skin.txt);
	pnlIcons.SetOnPaint(pnlInventory_t::paintIcons);
	//pnlIcons.SetBGActiveness(false);


	for (int i = 0; i < 10; i++) {
		pnlItem[i] = new ItemPanel(&Inventory::getInstance(), i+1, Position(0xFFFF, i+1, 0), 0);
		panel.AddObject(pnlItem[i]);
		pnlItem[i]->SetPos(posInvSlots[i].x, posInvSlots[i].y);
		pnlItem[i]->setUIBackground(96 + 32 * (posInvSlots[i].uisprite % 5), 32 * (posInvSlots[i].uisprite / 5));
	}

	// battlemode
	glictPanel* bGrid = chcBattleMode.getGrid();
	panel.AddObject(bGrid);
	bGrid->SetPos(124, 17);
	chcBattleMode.setItemSize(20, 20);
	chcBattleMode.setPadding(0, 0);
	chcBattleMode.setRows(1);
	for(int i = 0; i != 3; ++i)
	{
		battleModes[i] = chcBattleMode.addItem("", onSetFightModes, (void*)(i+1));
		battleModes[i]->btn.SetSkin(&g_skin.graphicbtn[BUTTON_FULLATK+i]);
		battleModes[i]->btn.SetHighlightSkin(&g_skin.graphicbth[BUTTON_FULLATK+i]);
	}
	chcBattleMode.setSelected(battleModes[options.battlemode-1]);

	glictPanel* cGrid = chcChase.getGrid();
	panel.AddObject(cGrid);
	cGrid->SetPos(147, 17);
	chcChase.setItemSize(20, 20);
	chcChase.setPadding(0,0);
	chcChase.setRows(1);
	for(int i = 0; i != 2; ++i)
	{
		chaseModes[i] = chcChase.addItem("", onSetChase, (void*)(i));
		chaseModes[i]->btn.SetSkin(&g_skin.graphicbtn[BUTTON_NOCHASE+i]);
		chaseModes[i]->btn.SetHighlightSkin(&g_skin.graphicbth[BUTTON_NOCHASE+i]);
	}
	chcChase.setSelected(chaseModes[options.chasemode]);

	panel.AddObject(&btnSafeMode);
	btnSafeMode.SetCaption("");
	btnSafeMode.SetPos(147, 57);
	btnSafeMode.SetWidth(20);
	btnSafeMode.SetHeight(20);
	btnSafeMode.SetOnClick(onSetSafeMode);
	btnSafeMode.SetSkin(&g_skin.graphicbtn[BUTTON_SAFEMODE]);
	btnSafeMode.SetHighlightSkin(&g_skin.graphicbth[BUTTON_SAFEMODE]);
	btnSafeMode.SetHold(options.safemode);
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
	// FIXME: this probably isn't the best way to do the background, but it's a good temporary solution.
	g_engine->getUISprite()->Blit((int)real->left, (int)real->top, 98, 240, caller->GetWidth(), caller->GetHeight());
	uint32_t icons = GlobalVariables::getPlayerIcons();
	// TODO (nfries88): make sure these are ordered and separated as they would be in CipSoft's client
	uint32_t posx = 2;

	if(icons & ICON_POISON)
	{
		g_engine->getUISprite()->Blit((int)real->left+posx, (int)real->top+2, 279, 32, 9, 9);
		posx += 10;
	}
	if(icons & ICON_BURN)
	{
		g_engine->getUISprite()->Blit((int)real->left+posx, (int)real->top+2, 288, 32, 9, 9);
		posx += 10;
	}
	if(icons & ICON_ENERGY)
	{
		g_engine->getUISprite()->Blit((int)real->left+posx, (int)real->top+2, 297, 32, 9, 9);
		posx += 10;
	}
	if(icons & ICON_SWORDS)
	{
		g_engine->getUISprite()->Blit((int)real->left+posx, (int)real->top+2, 306, 32, 9, 9);
		posx += 10;
	}
	if(icons & ICON_DRUNK)
	{
		g_engine->getUISprite()->Blit((int)real->left+posx, (int)real->top+2, 279, 41, 9, 9);
		posx += 10;
	}
	if(icons & ICON_MANASHIELD)
	{
		g_engine->getUISprite()->Blit((int)real->left+posx, (int)real->top+2, 288, 41, 9, 9);
		posx += 10;
	}
	if(icons & ICON_HASTE)
	{
		g_engine->getUISprite()->Blit((int)real->left+posx, (int)real->top+2, 297, 41, 9, 9);
		posx += 10;
	}
	if(icons & ICON_PARALYZE)
	{
		g_engine->getUISprite()->Blit((int)real->left+posx, (int)real->top+2, 306, 41, 9, 9);
		posx += 10;
	}
	if(icons & ICON_DROWNING)
	{
		g_engine->getUISprite()->Blit((int)real->left+posx, (int)real->top+2, 279, 59, 9, 9);
		posx += 10;
	}
	if(icons & ICON_FREEZING)
	{
		g_engine->getUISprite()->Blit((int)real->left+posx, (int)real->top+2, 279, 68, 9, 9);
		posx += 10;
	}

	if(icons & ICON_DAZZLED)
	{
		g_engine->getUISprite()->Blit((int)real->left+posx, (int)real->top+2, 279, 77, 9, 9);
		posx += 10;
	}
	if(icons & ICON_CURSED)
	{
		g_engine->getUISprite()->Blit((int)real->left+posx, (int)real->top+2, 279, 86, 9, 9);
		posx += 10;
	}
	if(icons & ICON_PARTY_BUFF)
	{
		g_engine->getUISprite()->Blit((int)real->left+posx, (int)real->top+2, 307, 148, 9, 9);
		posx += 10;
	}
}

void pnlInventory_t::onSetFightModes(ChoiceGrid::Item *parent, ChoiceGrid::Item *olditem)
{
	options.battlemode = (int)parent->data;
	((GM_Gameworld*)g_game)->m_protocol->sendFightModes(options.battlemode, options.chasemode, options.safemode);
	options.Save();
}
void pnlInventory_t::onSetChase(ChoiceGrid::Item *parent, ChoiceGrid::Item *olditem)
{
	options.chasemode = (int)parent->data;
	((GM_Gameworld*)g_game)->m_protocol->sendFightModes(options.battlemode, options.chasemode, options.safemode);
	options.Save();
}
void pnlInventory_t::onSetSafeMode(glictPos* relmousepos, glictContainer* callerclass)
{
	options.safemode = !options.safemode;
	((GM_Gameworld*)g_game)->m_protocol->sendFightModes(options.battlemode, options.chasemode, options.safemode);
	options.Save();
	((glictButton*)callerclass)->SetHold(options.safemode);
}
