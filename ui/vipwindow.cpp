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

#if defined(HAVE_LIBINTL_H)
    #include <libintl.h>
#else
    #define gettext(x) (x)
#endif

#include "vipwindow.h"
#include "sbvlpanel.h"
#include "../gamecontent/viplist.h"

winVIP_t::winVIP_t()
{
    window.SetWidth(160);
    window.SetHeight(GetDefaultHeight());
    window.SetCaption(gettext("VIP"));
    window.AddObject(&container);
    container.SetWidth(160);
    container.SetHeight(GetDefaultHeight());
    container.SetVirtualSize(160, 270);
    container.SetBGActiveness(false);

    const std::map<uint32_t, VipEntry>& entries = VipList::getInstance().getEntries();
    for (std::map<uint32_t, VipEntry>::const_iterator it = entries.begin(); it != entries.end(); it++)
    {
        addVIP(it->first);
    }
}
winVIP_t::~winVIP_t()
{
}


void winVIP_t::addVIP(uint32_t id)
{
    const std::map<uint32_t, VipEntry>& entries = VipList::getInstance().getEntries();
    std::map<uint32_t, VipEntry>::const_iterator it;

    it = entries.find(id);
    if (it == entries.end()) // if its unknown to gamecontent layer, then we cant add it either
        return;

    bool isonline = it->second.isOnline();

    glictPanel *p=m_entries[id]=new glictPanel;
    p->SetOnClick(OnListbox);
    p->SetBGActiveness(false);
    p->SetCaption(it->second.getName() + (isonline ? " *" : ""));
    p->SetCustomData(this);
    p->SetBGColor(.4,.4,.4,1.);
    if (isonline)
        p->SetCaptionColor(0,1,0);
    else
        p->SetCaptionColor(1,0,0);
    container.AddObject(p);
}
void winVIP_t::updateVIP(uint32_t id)
{
    const std::map<uint32_t, VipEntry>& entries = VipList::getInstance().getEntries();
    std::map<uint32_t, VipEntry>::const_iterator it;

    it = entries.find(id);
    if (it == entries.end()) // if its unknown to gamecontent layer, then we cant work with it either
        return;

    if (m_entries.find(id) == m_entries.end())
    {
        addVIP(id);
        return;
    }

    bool isonline = it->second.isOnline();

    glictPanel *p=m_entries[id];
    p->SetBGActiveness(false);
    p->SetCaption(it->second.getName() + (isonline ? " *" : ""));
    p->SetCustomData(this);
    p->SetBGColor(.4,.4,.4,1.);
    if (isonline)
        p->SetCaptionColor(0,1,0);
    else
        p->SetCaptionColor(1,0,0);
}
void winVIP_t::removeVIP(uint32_t id)
{
    glictPanel *p=m_entries[id];
    p->RemoveObject(p);
    //VipList::getInstance().removeEntry(id);
}

float winVIP_t::GetDefaultHeight()
{
    return 40.F;
}

void winVIP_t::OnClose()
{
	controller->closeVIP();
}


void winVIP_t::OnListbox(glictPos* pos, glictContainer *caller)
{
    std::map<uint32_t, glictPanel*>::iterator it;

    winVIP_t *wvip = (winVIP_t*)(caller->GetCustomData());

    for (it = wvip->m_entries.begin(); it != wvip->m_entries.end(); it++)
    {
        it->second->SetBGActiveness(false);
    }

    ((glictPanel*)caller)->SetBGActiveness(true);
}
