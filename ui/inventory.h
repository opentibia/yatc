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

#include "../skin.h"

class pnlInventory_t {
public:
	pnlInventory_t() {
		static int posInvSlots[] = {
            45, 3,  // helmet
            8, 17,  // necklace
            83, 17, // backpack
            45, 40, // armor
            83, 54, // right weapon
            8, 54,  // left weapon
            45, 77, // legs
            45, 115,// feet
            8, 91,  // ring
            83, 91};// hand

        panel.SetCaption("Inventory");
		panel.SetHeight(150);
		panel.SetWidth(150);
		for (int i = 0; i < 10; i++) {
			panel.AddObject(pnlItem+i);
			pnlItem[i].SetPos(posInvSlots[i*2], posInvSlots[i*2+1]);
			pnlItem[i].SetHeight(32);
			pnlItem[i].SetWidth(32);
			pnlItem[i].SetBGColor(.1,.1,.1,1);
			pnlItem[i].SetCaption("");
			pnlItem[i].SetCustomData(pnlItem);
			pnlItem[i].SetOnPaint(pnlInventory_t::inventoryItemOnPaint);
//			pnlItem[i].SetBGActiveness(true);
			pnlItem[i].SetOnClick(pnlInventory_t::inventoryItemOnClick);
			pnlItem[i].SetOnMouseUp(pnlInventory_t::inventoryItemOnMouseUp);
			pnlItem[i].SetOnMouseDown(pnlInventory_t::inventoryItemOnMouseDown);
			pnlItem[i].SetSkin(&g_skin.inv);
		}
	}

	static void inventoryItemOnPaint(glictRect *real, glictRect *clipped, glictContainer *caller);
	static void inventoryItemOnClick(glictPos *relmousepos, glictContainer* callerclass);
	static void inventoryItemOnMouseUp(glictPos *relmousepos, glictContainer* callerclass);
	static void inventoryItemOnMouseDown(glictPos *relmousepos, glictContainer* callerclass);


	/*glictContainer*/glictWindow panel; // it's a container, so it's transparent
	glictPanel pnlItem[10];

};


#endif
