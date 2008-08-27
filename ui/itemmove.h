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

	Position fromPos;
	Position toPos;
	Item* dispItem;			// item to display in the window
	uint16_t itemid;
	uint8_t maxct;
	uint8_t fromStack;

	winItemMove_t()
	{
		initUI();
	}

	winItemMove_t(uint16_t _id, uint8_t _count, Position from, Position to, uint8_t _stack)
		: fromPos(from), toPos(to), itemid(_id), maxct(_count), fromStack(_stack)
	{
		dispItem = Item::CreateItem(_id, _count);

		initUI();

		sbCt.SetValue(_count);
	}

	void initUI()
	{
		window.SetCaption("Move Objects");
		window.SetWidth(228);
		window.SetHeight(115);

		window.AddObject(&sbCt);
		sbCt.SetPos(59, 48);
		sbCt.SetWidth(155);
		sbCt.SetHeight(12);
		sbCt.SetValue(1);
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

		window.AddObject(&btnOk);
		btnOk.SetPos(123, 89);
		btnOk.SetWidth(43);
		btnOk.SetHeight(20);
		btnOk.SetCaption("Ok");
		btnOk.SetCustomData(this);
		btnOk.SetOnClick(winItemMove_t::moveItem);

		window.AddObject(&btnCancel);
		btnCancel.SetPos(176, 89);
		btnCancel.SetWidth(43);
		btnCancel.SetHeight(20);
		btnCancel.SetCaption("Cancel");
		btnOk.SetOnClick(winItemMove_t::onCancel);
	}

	static void onChangeCount(glictPos* pos, glictContainer *caller)
	{
		winItemMove_t* witm = (winItemMove_t*)(caller->GetCustomData());
		if(witm->sbCt.GetValue() <= witm->maxct) {
			if(witm->dispItem != NULL)
				delete witm->dispItem;
			witm->dispItem = Item::CreateItem(witm->itemid, (uint8_t)witm->sbCt.GetValue());
		}
	}

	static void drawItem(glictRect *real, glictRect *clipped, glictContainer *caller)
	{
		winItemMove_t* witm = (winItemMove_t*)(caller->GetCustomData());
		witm->dispItem->Blit((int)real->left, (int)real->top);
	}

	static void moveItem(glictPos* pos, glictContainer *caller);
	static void onCancel(glictPos* pos, glictContainer *caller)
	{
		// do nothing?
	}
};

#endif
