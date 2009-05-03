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

#include "uicontainer.h"
#include "../gm_gameworld.h"
#include "../net/protocolgame.h"
#include "itempanel.h"


extern int g_lastmousebutton;

winContainer_t::winContainer_t(Container* _container, uint32_t cid) {
    container = _container;
    containerId = cid;

    window.SetCaption(container->getName());
    window.SetHeight(GetDefaultHeight());

    // icon on titlebar
    // support added to glict svn 76+
    // this needs to be relocated manually
    #if (GLICT_APIREV >= 76)
	itemIcon = Item::CreateItem(container->getItemId(), 1);

	window.AddTitlebarObject(&pnlIcon);
	pnlIcon.SetWidth(12);
	pnlIcon.SetHeight(12);
	pnlIcon.SetPos(2, 2);
	pnlIcon.SetCustomData(itemIcon);
	pnlIcon.SetOnPaint(containerIconOnPaint);
	pnlIcon.SetBGActiveness(false);
    #else
    #warning For titlebar objects (such as close buttons) to work properly, you need GLICT APIREV 76+
    #endif

    // we're adding a winpanel object.
    // sadly, glictWindow does not have virtualsize handling implemented.
    // glictPanel does have virtual size; we'll add subpanels into glictPanel
    // which'll be of the same size as the window itself, and the only object
    // on the window.
    window.AddObject(&winpanel);
    winpanel.SetBGActiveness(false);
    winpanel.SetPos(0,0);
    winpanel.SetWidth(150 + 10); // same size as for window
    winpanel.SetHeight(GetDefaultHeight());
    // virtual size is always to display all rows
    winpanel.SetVirtualSize(160, 4 + (36*ceil(container->getCapacity()/4.)));

    for(uint32_t i = 0; i != container->getCapacity(); ++i)
    {
        glictPanel* panel = new ItemPanel(container, i, Position(0xFFFF, containerId | 0x40, i));
        panel->SetPos(5 + ((i % 4) * 36), 4 + (std::floor(i / 4.0f) * 36));

        panel->SetSkin(&g_skin.inv);
        winpanel.AddObject(panel);
        pnlItems.push_back(panel);
    }
}

winContainer_t::~winContainer_t()
{
	for(PanelList::iterator it = pnlItems.begin(); it != pnlItems.end(); ++it)
	{
		delete (*it);
	}

	pnlItems.clear();
	#if (GLICT_APIREV >= 76)
	delete itemIcon;
	#endif
}

float winContainer_t::GetDefaultHeight()
{
	return (MIN(4+36*3, 4 + (36*ceil(container->getCapacity()/4.))));
}

void winContainer_t::OnClose()
{
    GM_Gameworld *gw = ((GM_Gameworld*)g_game);
    gw->m_protocol->sendCloseContainer(containerId);
}

void winContainer_t::containerIconOnPaint(glictRect *real, glictRect *clipped, glictContainer *caller)
{
	Item* item = (Item*)caller->GetCustomData();
	g_engine->resetClipping();
	ObjectType* t = Objects::getInstance()->getItemType(item->getID());
	item->Blit((int)real->left, (int)real->top, 12./(32.f * MAX(t->height, t->width)));
}
