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
#include "../gamecontent/creature.h"
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
	#if (GLICT_APIREV >= 98)
		panel.SetDraggable(true);
		panel.SetSkin(&g_skin.background);
		panel.SetFocusable(true);
	#else
		#warning Inventory is not properly drawn until you upgrade to GLICT 98+
		panel.SetCaption(gettext("Inventory"));
	#endif

	panel.AddObject(&btnStop);
	btnStop.SetCaption(gettext("Stop"));
	btnStop.SetFont("minifont");
	//btnStop.SetOnClick(&pnlInventory_t::onClick_Stop);

	panel.AddObject(&btnCollapse);
	btnCollapse.SetCaption("");
	btnCollapse.SetWidth(12);
	btnCollapse.SetHeight(12);
	btnCollapse.SetPos(8, 3);
	btnCollapse.SetCustomData(this);

	panel.AddObject(&btnQuests);
	btnQuests.SetCaption(gettext("Quests"));
	btnQuests.SetWidth(50);
	btnQuests.SetHeight(20);
	btnQuests.SetPos(120, 104);
	btnQuests.SetFont("minifont");
	//btnQuests.SetOnClick(&pnlInventory_t::onClick_Quests);

	panel.AddObject(&btnOptions);
	btnOptions.SetFont("minifont");
	btnOptions.SetOnClick(&pnlInventory_t::onClick_Options);

	panel.AddObject(&btnHelp);
	//btnHelp.SetOnClick(&pnlInventory_t::onClick_Help);

	panel.AddObject(&pnlSoul);
	pnlSoul.SetPos(8, 128);
	pnlSoul.SetHeight(19);
	pnlSoul.SetWidth(32);
	pnlSoul.SetSkin(&g_skin.inv);
	pnlSoul.SetOnPaint(pnlInventory_t::paintSoul);

	panel.AddObject(&pnlCap);
	pnlCap.SetPos(83, 128);
	pnlCap.SetHeight(20);
	pnlCap.SetWidth(32);
	pnlCap.SetSkin(&g_skin.inv);
	pnlCap.SetOnPaint(pnlInventory_t::paintCap);

	panel.AddObject(&pnlCapSoulCombo);
	pnlCapSoulCombo.SetPos(22, 3);
	pnlCapSoulCombo.SetHeight(42);
	pnlCapSoulCombo.SetWidth(32);
	pnlCapSoulCombo.SetSkin(&g_skin.inv);
	//pnlSoul.SetOnPaint(pnlInventory_t::paintCapSoulCombo);

	// TODO: fix this to the proper skin (background should match capacity and soul)
	panel.AddObject(&pnlIcons);
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
	chcBattleMode.setItemSize(20, 20);
	chcBattleMode.setPadding(0, 0);
	for(int i = 0; i != 3; ++i)
	{
		battleModes[i] = chcBattleMode.addItem("", onSetFightModes, (void*)(i+1));
		battleModes[i]->btn.SetSkin(&g_skin.graphicbtn[BUTTON_FULLATK+i]);
		battleModes[i]->btn.SetHighlightSkin(&g_skin.graphicbth[BUTTON_FULLATK+i]);
	}
	chcBattleMode.setSelected(battleModes[options.battlemode-1]);

	glictPanel* cGrid = chcChase.getGrid();
	panel.AddObject(cGrid);
	chcChase.setItemSize(20, 20);
	chcChase.setPadding(0,0);
	for(int i = 0; i != 2; ++i)
	{
		chaseModes[i] = chcChase.addItem("", onSetChase, (void*)(i));
		chaseModes[i]->btn.SetSkin(&g_skin.graphicbtn[BUTTON_NOCHASE+i]);
		chaseModes[i]->btn.SetHighlightSkin(&g_skin.graphicbth[BUTTON_NOCHASE+i]);
	}
	chcChase.setSelected(chaseModes[options.chasemode]);

	panel.AddObject(&btnSafeMode);
	btnSafeMode.SetCaption("");
	btnSafeMode.SetWidth(20);
	btnSafeMode.SetHeight(20);
	btnSafeMode.SetOnClick(onSetSafeMode);
	btnSafeMode.SetSkin(&g_skin.graphicbtn[BUTTON_SAFEMODE]);
	btnSafeMode.SetHighlightSkin(&g_skin.graphicbth[BUTTON_SAFEMODE]);
	btnSafeMode.SetHold(options.safemode);

	if(options.inventoryCollapsed)
		collapse();
	else
		expand();
}

pnlInventory_t::~pnlInventory_t()
{
	for(int i = 0; i < 10; i++){
		delete pnlItem[i];
		pnlItem[i] = NULL;
	}
}

void pnlInventory_t::collapse()
{
	// TODO (nfries88): implement this as cleanly as possible
	panel.SetHeight(64);
	panel.SetWidth(170);

	btnCollapse.SetSkin(&g_skin.graphicbtn[BUTTON_EXPAND_WINDOW]);
	btnCollapse.SetHighlightSkin(&g_skin.graphicbth[BUTTON_EXPAND_WINDOW]);
	btnCollapse.SetOnClick(&pnlInventory_t::onClick_Expand);

	btnQuests.SetVisible(false);

	btnOptions.SetSkin(&g_skin.graphicbtn[BUTTON_OPTIONSMINI]);
	btnOptions.SetHighlightSkin(&g_skin.graphicbth[BUTTON_OPTIONSMINI]);
	btnOptions.SetWidth(20);
	btnOptions.SetHeight(20);
	btnOptions.SetPos(125, 27);
	btnOptions.SetCaption("");

	btnStop.SetPos(125, 3);
	btnStop.SetWidth(42);
	btnStop.SetHeight(20);

	btnHelp.SetSkin(&g_skin.graphicbtn[BUTTON_HELPMINI]);
	btnHelp.SetHighlightSkin(&g_skin.graphicbth[BUTTON_HELPMINI]);
	btnHelp.SetCaption("");
	btnHelp.SetWidth(20);
	btnHelp.SetHeight(20);
	btnHelp.SetPos(147, 27);
	btnHelp.SetFont("minifont");

	glictPanel* bGrid = chcBattleMode.getGrid();
	bGrid->SetPos(57, 3);
	chcBattleMode.setRows(3);

	glictPanel* cGrid = chcChase.getGrid();
	cGrid->SetPos(57, 25);
	chcChase.setRows(3);

	pnlSoul.SetVisible(false);
	pnlCap.SetVisible(false);
	pnlCapSoulCombo.SetVisible(true);
	pnlIcons.SetPos(8, 48);

	btnSafeMode.SetPos(97, 27);

	for (int i = 0; i < 10; i++) {
		pnlItem[i]->SetVisible(false);
	}
}

void pnlInventory_t::expand()
{
	panel.SetHeight(170);
	panel.SetWidth(170);

	btnCollapse.SetSkin(&g_skin.graphicbtn[BUTTON_COLLAPSE_WINDOW]);
	btnCollapse.SetHighlightSkin(&g_skin.graphicbth[BUTTON_COLLAPSE_WINDOW]);
	btnCollapse.SetOnClick(&pnlInventory_t::onClick_Collapse);

	btnQuests.SetVisible(true);

	btnOptions.SetSkin(&g_skin.btnn);
	btnOptions.SetHighlightSkin(&g_skin.btnh);
	btnOptions.SetCaption(gettext("Options"));
	btnOptions.SetWidth(50);
	btnOptions.SetHeight(20);
	btnOptions.SetPos(120, 126);

	btnStop.SetPos(120, 82);
	btnStop.SetWidth(50);
	btnStop.SetHeight(20);

	btnHelp.SetSkin(&g_skin.btnn);
	btnHelp.SetHighlightSkin(&g_skin.btnh);
	btnHelp.SetCaption(gettext("Help"));
	btnHelp.SetWidth(50);
	btnHelp.SetHeight(20);
	btnHelp.SetPos(120, 148);
	btnHelp.SetFont("minifont");

	glictPanel* bGrid = chcBattleMode.getGrid();
	bGrid->SetPos(124, 17);
	chcBattleMode.setRows(1);

	glictPanel* cGrid = chcChase.getGrid();
	cGrid->SetPos(147, 17);
	chcChase.setRows(1);

	pnlSoul.SetVisible(true);
	pnlCap.SetVisible(true);
	pnlCapSoulCombo.SetVisible(false);
	pnlIcons.SetPos(8, 152);

	btnSafeMode.SetPos(147, 57);

	for (int i = 0; i < 10; i++) {
		pnlItem[i]->SetVisible(true);
	}
}

void pnlInventory_t::paintCap(glictRect *real, glictRect *clipped, glictContainer *caller)
{
	std::string text = yatc_itoa(GlobalVariables::getPlayerStat(STAT_CAPACITY));
	g_engine->drawText("Cap:", "minifont", (int)real->left + 6, (int)real->top + 1);
	float len = g_engine->sizeText(text.c_str(), "aafont");
	float width = real->right - real->left;
	if(len > width)
		len = width;
	int diff = std::floor((width - len) / 2.f);
	g_engine->drawText(text.c_str(), "aafont", (int)real->left + diff, (int)real->top + 9);
}
void pnlInventory_t::paintSoul(glictRect *real, glictRect *clipped, glictContainer *caller)
{
	std::string text = yatc_itoa(GlobalVariables::getPlayerStat(STAT_SOUL));
	g_engine->drawText("Soul:", "minifont", (int)real->left + 6, (int)real->top + 1);
	float len = g_engine->sizeText(text.c_str(), "aafont");
	float width = real->right - real->left;
	if(len > width)
		len = width;
	int diff = std::floor((width - len) / 2.f);
	g_engine->drawText(text.c_str(), "aafont", (int)real->left + diff, (int)real->top + 9);
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
	if(icons & ICON_PZBLOCK)
	{
		g_engine->getUISprite()->Blit((int)real->left+posx, (int)real->top+2, 310, 191, 9, 9);
		posx += 10;
	}
	if(icons & ICON_PZ)
	{
		g_engine->getUISprite()->Blit((int)real->left+posx, (int)real->top+2, 310, 182, 9, 9);
		posx += 10;
	}

	Creature* player = Creatures::getInstance().getPlayer();
	if (!player)
		return;
	switch(player->getSkull()){
		case SKULL_GREEN:
			g_engine->getUISprite()->Blit((int)real->left+posx, (int)real->top+2, 279, 50, 9, 9);
			posx += 10;
			break;
		case SKULL_YELLOW:
			g_engine->getUISprite()->Blit((int)real->left+posx, (int)real->top+2, 288, 50, 9, 9);
			posx += 10;
			break;
		case SKULL_WHITE:
			g_engine->getUISprite()->Blit((int)real->left+posx, (int)real->top+2, 297, 50, 9, 9);
			posx += 10;
			break;
		case SKULL_RED:
			g_engine->getUISprite()->Blit((int)real->left+posx, (int)real->top+2, 306, 50, 9, 9);
			posx += 10;
			break;
		case SKULL_BLACK:
			g_engine->getUISprite()->Blit((int)real->left+posx, (int)real->top+2, 342, 200, 9, 9);
			posx += 10;
			break;
		default: break;
	}

	if(player->getEmblem() != EMBLEM_NONE) { // our player shouldn't have other emblem than green
		g_engine->getUISprite()->Blit((int)real->left+posx, (int)real->top+2, 251, 218, 9, 9);
		posx += 10;
	}
}

void pnlInventory_t::onSetFightModes(ChoiceGrid::Item *parent, ChoiceGrid::Item *olditem)
{
	options.battlemode = VOIDP2INT(parent->data);
	((GM_Gameworld*)g_game)->m_protocol->sendFightModes(options.battlemode, options.chasemode, options.safemode);
	options.Save();
}
void pnlInventory_t::onSetChase(ChoiceGrid::Item *parent, ChoiceGrid::Item *olditem)
{
	options.chasemode = VOIDP2INT(parent->data);
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


void pnlInventory_t::onClick_Collapse(glictPos* relmousepos, glictContainer* callerclass)
{
	pnlInventory_t* pnlInv = (pnlInventory_t*)callerclass->GetCustomData();
	pnlInv->collapse();
	((GM_Gameworld*)g_game)->updateRightSide();

	options.inventoryCollapsed = true;
	options.Save();
}
void pnlInventory_t::onClick_Expand(glictPos* relmousepos, glictContainer* callerclass)
{
	pnlInventory_t* pnlInv = (pnlInventory_t*)callerclass->GetCustomData();
	pnlInv->expand();
	((GM_Gameworld*)g_game)->updateRightSide();

	options.inventoryCollapsed = false;
	options.Save();
}
