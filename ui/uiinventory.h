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


#ifndef __UI_INVENTORY_H
#define __UI_INVENTORY_H

#include <GLICT/container.h>
#include <GLICT/panel.h>
#include <GLICT/textbox.h>
#include <GLICT/button.h>
#include <GLICT/window.h>
#include <GLICT/globals.h>

#include "../skin.h"
#if defined(HAVE_LIBINTL_H)
    #include <libintl.h>
#else
    #define gettext(x) (x)
#endif

#include "../popup.h"
#include "itempanel.h"
#include "../choicegrid.h"

class pnlInventory_t {
public:
	pnlInventory_t();
	~pnlInventory_t();

#if (GLICT_APIREV >= 98)
    glictPanel panel; // 170x34
#elif _MSC_VER
	__declspec(deprecated) glictWindow panel;
#else
	glictWindow panel __attribute__((deprecated));
#endif

	glictPanel pnlSoul;
	glictPanel pnlCap;
	glictPanel pnlCapSoulCombo;
	glictPanel pnlIcons;

	ChoiceGrid::Item* battleModes[3];
	ChoiceGrid::Item* chaseModes[2];

	ChoiceGrid chcBattleMode;
	ChoiceGrid chcChase;
	glictButton btnSafeMode;

	ItemPanel* pnlItem[10];

	glictButton btnStop;
	glictButton btnQuests;
	glictButton btnOptions;
	glictButton btnHelp;

	glictButton btnCollapse;

	void collapse();
	void expand();


	static void onClick_Options(glictPos* relmousepos, glictContainer* callerclass);
	static void onClick_Collapse(glictPos* relmousepos, glictContainer* callerclass);
	static void onClick_Expand(glictPos* relmousepos, glictContainer* callerclass);

	static void paintCap(glictRect *real, glictRect *clipped, glictContainer *caller);
	static void paintSoul(glictRect *real, glictRect *clipped, glictContainer *caller);
	static void paintIcons(glictRect *real, glictRect *clipped, glictContainer *caller);

	static void onSetFightModes(ChoiceGrid::Item *parent, ChoiceGrid::Item *olditem);
	static void onSetChase(ChoiceGrid::Item *parent, ChoiceGrid::Item *olditem);
	static void onSetSafeMode(glictPos* relmousepos, glictContainer* callerclass);
};


#endif
