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

#ifndef __UI_ITEMMOVE_H
#define __UI_ITEMMOVE_H

#include <GLICT/container.h>
#include <GLICT/panel.h>
#include <GLICT/textbox.h>
#include <GLICT/button.h>
#include <GLICT/window.h>

#include "../gamecontent/item.h"

//TODO (nfries88) actually implement this and make sure it works.

class winItemMove_t {
public:
	glictWindow window;		// 228x115
	glictScrollbar sbCt;	// 59 48 155x12
	glictPanel lblTxt;		// 59 17 143x24
	glictPanel pnlItem;		// 15 15 32x32

	glictPanel pnlSep;		// 10 80 210x2
	glictButton btnOk;		// 123 89 43x20
	glictButton btnCancel;	// 176 89 43x20

	const Item* item;
	const Position& fromPos;
	const Position& toPos;
	Item* dispItem;			// item to display in the window

	winItemMove_t(const Item* _item, const Position& from, const Position& to)
		: item(_item), fromPos(from), toPos(to)
	{
		dispItem = Item::CreateItem(item->getID(), item->getCount());

		window.SetCaption("Move Objects");
		window.SetWidth(228);
		window.SetHeight(115);

		window.AddObject(&sbCt);
		sbCt.SetPos(59, 48);
		sbCt.SetWidth(155);
		sbCt.SetHeight(12);
		sbCt.SetValue((int)item->getCount());
		sbCt.SetOnClick(winItemMove_t::onChangeCount);

		window.AddObject(&lblTxt);
		lblTxt.SetCaption("Set the number of \nitems you want to move:");
		lblTxt.SetPos(59, 17);
		lblTxt.SetWidth(143);
		lblTxt.SetHeight(24);

		window.AddObject(&pnlItem);
		pnlItem.SetPos(15, 15);
		pnlItem.SetWidth(32);
		pnlItem.SetHeight(32);
		pnlItem.SetCustomData(this);
		pnlItem.SetOnPaint(winItemMove_t::drawItem);

		window.AddObject(&pnlSep);
		pnlSep.SetPos(10, 80);
		pnlSep.SetWidth(210);
		pnlSep.SetHeight(2);

		btnOk.SetPos(123, 89);
		btnOk.SetWidth(43);
		btnOk.SetHeight(20);
		//btnOk.SetOnClick(winItemMove_t::moveItem);

		btnOk.SetPos(176, 89);
		btnOk.SetWidth(43);
		btnOk.SetHeight(20);
		//btnOk.SetOnClick(winItemMove_t::onCancel);
	}

	static void onChangeCount(glictPos* pos, glictContainer *caller)
	{
		winItemMove_t* witm = (winItemMove_t*)(caller->GetCustomData());
		delete witm->dispItem;
		witm->dispItem = Item::CreateItem(witm->item->getID(), (uint8_t)witm->sbCt.GetValue());
	}

	static void drawItem(glictRect *real, glictRect *clipped, glictContainer *caller)
	{
		winItemMove_t* witm = (winItemMove_t*)(caller->GetCustomData());
		witm->dispItem->Blit((int)real->left, (int)real->top);
	}
};

#endif
