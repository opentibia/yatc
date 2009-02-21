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


#ifndef __UI_ITEM_PANEL_H
#define __UI_ITEM_PANEL_H


#include <GLICT/panel.h>
#include "../gamecontent/position.h"
#include "../popup.h"

class ItemContainer;
class Item;

class ItemPanel : public glictPanel
{
public:
	ItemPanel(ItemContainer* container, uint32_t slot, const Position& pos);
	virtual ~ItemPanel();

	void setUIBackground(int x, int y)
	{
		m_uiBackx = x;
		m_uiBacky = y;
		m_uiBack = true;
	}

protected:

	static void itemPanelOnPaint(glictRect *real, glictRect *clipped, glictContainer *caller);
	static void itemPanelOnClick(glictPos* relmousepos, glictContainer* callerclass);
    static void itemPanelOnMouseDown(glictPos* relmousepos, glictContainer* callerclass);
	static void itemPanelOnMouseUp(glictPos* relmousepos, glictContainer* callerclass);
    static void itemPanelMakePopup(Popup* popup, void* owner, void* arg);

    static void onLookAt(Popup::Item*);
    static void onUse(Popup::Item*);
    static void onTrade(Popup::Item*);

	Item* getItem();

private:
	int m_uiBackx, m_uiBacky;
	bool m_uiBack;

	ItemContainer* m_container;
	uint32_t m_slot;
	Position m_pos;
};


#endif
