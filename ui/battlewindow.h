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

#ifndef __UI_BATTLEWINDOW_H
#define __UI_BATTLEWINDOW_H

#include <GLICT/panel.h>
#include <GLICT/progressbar.h>

#include "stackpanel.h"

#include "../util.h"
#if defined(HAVE_LIBINTL_H)
    #include <libintl.h>
#else
    #define gettext(x) (x)
#endif

class sbvlPanel_t;

class winBattle_t : public yatcStackPanelWindow
{
public:
	struct BattleEntry
	{
		uint32_t creatureId;
		glictPanel pnl;
		glictProgressBar pbHealth;
	};

	std::vector<BattleEntry*> entries;
	glictPanel container;
	glictList list;

	sbvlPanel_t* controller;

	winBattle_t()
	{
		window.SetWidth(160);
		window.SetHeight(GetDefaultHeight());
		window.SetCaption(gettext("Battle"));
		window.AddObject(&container);
		container.SetPos(0, 0);
		container.SetWidth(160);
		container.SetHeight(GetDefaultHeight());
		container.SetVirtualSize(160, 270);
		container.SetBGActiveness(false);
		container.SetFocusable(false);
		container.AddObject(&list);
		list.SetPos(0, 0);
		list.SetWidth(160);
		list.SetHeight(GetDefaultHeight());
		list.SetBGActiveness(false);
		list.SetFocusable(false);
		list.SetVisible(true);
		list.SetForcedHeight(0); // idk what to set it to and dont want to have to rebuild after every change

		entries.clear();
	}
	~winBattle_t(){}

	void add(uint32_t id);
	void update(uint32_t id);
	void remove(uint32_t id);
	void refreshVisibility();

	virtual float GetDefaultHeight(){ return 40.F; }
	virtual void OnClose();

	static void paintEntry(glictRect *real, glictRect *clipped, glictContainer *caller);
	static void clickEntry(glictPos* relmousepos, glictContainer* callerclass);
};

#endif //__UI_BATTLEWINDOW_H

