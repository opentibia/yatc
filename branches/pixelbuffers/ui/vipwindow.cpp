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


#ifdef HAVE_CONFIG_H
	#include "../config.h"
#endif
#if defined(HAVE_LIBINTL_H)
    #include <libintl.h>
#else
    #define gettext(x) (x)
#endif

#include "../gm_gameworld.h"
#include "vipwindow.h"
#include "sbvlpanel.h"
#include "../gamecontent/viplist.h"
#include "../gamecontent/creature.h"
#include "../clipboard.h"
#include "../net/protocolgame.h"
#include "../skin.h"

extern int g_lastmousebutton;
extern yatcClipboard g_clipboard;

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

    // TODO (Kilouco): Shouldn't be highlighten when selected.
    // TODO (Kilouco): Should add Padding for contacts.

    glictPanel *p=m_entries[id]=new glictPanel;
    p->SetOnClick(OnListbox);
    p->SetBGActiveness(false);
    p->SetCaption(it->second.getName());
    p->SetCustomData(this);
    p->SetBGColor(.4,.4,.4,1.);
    if (isonline)
        p->SetCaptionColor(0.3725f,0.9686f,0.3725f);
        //p->SetCaptionColor(0,1,0);
    else {
        if(options.hideofflineVIP)
            p->SetVisible(false);
        else
            p->SetCaptionColor(0.9686f,0.3725f,0.3725f);
            //p->SetCaptionColor(1,0,0);
    }
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
    p->SetCaption(it->second.getName());
    p->SetCustomData(this);
    p->SetBGColor(.4,.4,.4,1.);
    if (isonline)
        p->SetCaptionColor(0.3725f,0.9686f,0.3725f);
        //p->SetCaptionColor(0,1,0);
    else {
        if(options.hideofflineVIP)
            p->SetVisible(false);
        else
            p->SetCaptionColor(0.9686f,0.3725f,0.3725f);
            //p->SetCaptionColor(1,0,0);
    }
}
void winVIP_t::removeVIP(uint32_t id)
{
    glictPanel *p=m_entries[id];
    container.RemoveObject(p);
    if (m_entries.find(id) != m_entries.end())
        m_entries.erase(m_entries.find(id));

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
    GM_Gameworld* gw = (GM_Gameworld*)g_game;
    winVIP_t *wvip = (winVIP_t*)(caller->GetCustomData());

    std::map<uint32_t, glictPanel*>::iterator it;

    for (it = wvip->m_entries.begin(); it != wvip->m_entries.end(); it++)
    {
        it->second->SetBGActiveness(false);
    }
    ((glictPanel*)caller)->SetBGActiveness(true);

    if (g_lastmousebutton == SDL_BUTTON_RIGHT)
    {
        gw->performPopup(makeVIPPopup, (void*)wvip, (void*)caller);
    }
}

void winVIP_t::makeVIPPopup(Popup* popup, void* owner, void* arg)
{
	winVIP_t* wvip = (winVIP_t*)owner;
	glictPanel* pnl = (glictPanel*)arg;

	std::stringstream s;

	uint32_t creatureid=0;
	for(std::map<uint32_t, glictPanel*>::iterator it = wvip->m_entries.begin(); it != wvip->m_entries.end(); it++)
	{
	    if (it->second == pnl)
            creatureid = it->first;
	}
	wvip->selectedcreature=creatureid;

    s.str("");
    s << gettext("Edit ") + pnl->GetCaption();
    popup->addItem(s.str(), onEditVIP, (void*)pnl->GetCaption().c_str());

    s.str("");
    s << gettext("Remove ") + pnl->GetCaption();
    popup->addItem(s.str(), onRemoveVIP, wvip);

    s.str("");
    s << gettext("Message to ") + pnl->GetCaption();
    popup->addItem(s.str(), onMessageTo, (void*)wvip->selectedcreature);

    popup->addItem("-",NULL,NULL);

    s.str("");
    s << gettext("Add new VIP");
    popup->addItem(s.str(), onAddVIP);

    s.str("");
    s << gettext("Sort by name");
    popup->addItem(s.str(), onUnimplemented);

    s.str("");
    s << gettext("Sort by type");
    popup->addItem(s.str(), onUnimplemented);

    s.str("");
    s << gettext("Sort by status");
    popup->addItem(s.str(), onUnimplemented);

    s.str("");
    if(!options.hideofflineVIP)
        s << gettext("Hide offline VIPs");
    else
        s << gettext("Show offline VIPs");
    popup->addItem(s.str(), onHideOfflineVIPs, wvip);

    popup->addItem("-",NULL,NULL);

    s.str("");
    s << gettext("Copy name");
    popup->addItem(s.str(), onCopyName, (void*)wvip->selectedcreature);

}

void winVIP_t::onUnimplemented(Popup::Item *parent)
{
	GM_Gameworld* gw = (GM_Gameworld*)g_game;

    gw->msgBox(gettext("This functionality is not yet finished"), "TODO");
}

void winVIP_t::onAddVIP(Popup::Item *parent)
{
    winVIP_t* wvip = (winVIP_t*)(parent->parent->getOwner());
    wvip->winAdd.launch();
}
void winVIP_t::onEditVIP(Popup::Item* parent)
{
    winVIP_t* wvip = (winVIP_t*)(parent->parent->getOwner());
    wvip->winEdit.launch(std::string((char*)parent->data));
}

void winVIP_t::onRemoveVIP(Popup::Item *parent)
{
	GM_Gameworld* gw = (GM_Gameworld*)g_game;

	winVIP_t* wvip = (winVIP_t*)parent->data;
	printf("Removing %d\n", wvip->selectedcreature),
    wvip->removeVIP(wvip->selectedcreature);
    VipList::getInstance().removeEntry(wvip->selectedcreature);
    gw->m_protocol->sendRemVip(wvip->selectedcreature);
}

void winVIP_t::onHideOfflineVIPs(Popup::Item *parent)
{
    winVIP_t* wvip = (winVIP_t*)parent->data;
    std::map<uint32_t, glictPanel*>::iterator eit = wvip->m_entries.begin();

    options.hideofflineVIP = !options.hideofflineVIP;
    if(options.hideofflineVIP){
        for(; eit != wvip->m_entries.end(); eit++){
            if(!(VipList::getInstance().entryIsOnline(eit->first)))
                (eit->second)->SetVisible(false);
        }
    }
    else {
        for(; eit != wvip->m_entries.end(); eit++){
            if(!(VipList::getInstance().entryIsOnline(eit->first)))
                (eit->second)->SetVisible(true);
        }
    }
}

void winVIP_t::onMessageTo(Popup::Item *parent)
{
	VipEntry vip = VipList::getInstance().getEntry((uint32_t)VOIDP2INT(parent->data));
    GM_Gameworld *gw = (GM_Gameworld*)g_game;
    gw->setActiveConsole(gw->findConsole(vip.getName()));
}

void winVIP_t::onCopyName(Popup::Item *parent)
{
    VipEntry vip = VipList::getInstance().getEntry((uint32_t)VOIDP2INT(parent->data));
    g_clipboard.setText(vip.getName());
}

winAddVIP_t::winAddVIP_t()
{
    window.SetWidth(236);
    window.SetHeight(105);
    window.SetCaption(gettext("Add to VIP list"));

    window.AddObject(&lblName);
    lblName.SetWidth(210);
    lblName.SetHeight(12);
    lblName.SetPos(14, 16);
    lblName.SetFont("aafont");
    lblName.SetCaption(gettext("Please enter a character name:"));
    lblName.SetBGActiveness(false);

    window.AddObject(&name);
    name.SetPos(14, 30);
    name.SetWidth(200);
    name.SetHeight(16);

    window.AddObject(&pnlSep);
    pnlSep.SetPos(10, 68);
    pnlSep.SetWidth(210);
    pnlSep.SetHeight(2);
    pnlSep.SetSkin(&g_skin.txt);

    window.AddObject(&btnAdd);
    btnAdd.SetCaption("Add");
    btnAdd.SetFont("minifont");
    btnAdd.SetPos(124, 76);
    btnAdd.SetWidth(42);
    btnAdd.SetHeight(20);
    btnAdd.SetCustomData(this);
    btnAdd.SetOnClick(winAddVIP_t::onButtonPressed);

    window.AddObject(&btnCancel);
    btnCancel.SetCaption("Cancel");
    btnCancel.SetFont("minifont");
    btnCancel.SetPos(176, 76);
    btnCancel.SetWidth(42);
    btnCancel.SetHeight(20);
    btnCancel.SetCustomData(this);
    btnCancel.SetOnClick(winAddVIP_t::onButtonPressed);
}
winAddVIP_t::~winAddVIP_t()
{
}
void winAddVIP_t::onButtonPressed(glictPos* pos, glictContainer *caller)
{
    winAddVIP_t* wadd = (winAddVIP_t*)caller->GetCustomData();
    GM_Gameworld *gw = (GM_Gameworld*)g_game;
    if(caller == &(wadd->btnAdd)){
        gw->m_protocol->sendAddVip(wadd->name.GetCaption());
    }
    wadd->name.SetCaption("");
    wadd->window.SetVisible(false);
}
void winAddVIP_t::launch()
{
    this->window.SetVisible(true);
    g_game->centerWindow(&this->window);
}

winChangeVIP_t::winChangeVIP_t()
{
    window.SetCaption(gettext("Change player in VIP list"));
    window.SetWidth(286);
    window.SetHeight(250);

    window.AddObject(&lblName);
    lblName.SetFont("aafont");
    lblName.SetPos(18, 14);
    lblName.SetWidth(250);
    lblName.SetHeight(12);
    lblName.SetBGActiveness(false);

    window.AddObject(&sepTop);
    sepTop.SetPos(18, 28);
    sepTop.SetWidth(250);
    sepTop.SetHeight(2);
    sepTop.SetSkin(&g_skin.txt);
    sepTop.SetBGColor(.2,.2,.2,1.);

    window.AddObject(&lblType);
    lblType.SetPos(18, 48);
    lblType.SetWidth(85);
    lblType.SetHeight(12);
    lblType.SetFont("aafont");
    lblType.SetCaption(gettext("Select a type:"));
    lblType.SetBGActiveness(false);

    window.AddObject(btnIcons.getGrid());
    btnIcons.getGrid()->SetPos(18,60);
    btnIcons.setItemSize(16,16);
    btnIcons.setPadding(4,4);
    btnIcons.setRows(11);
    btnIcons.setOnClick(NULL);
    btnIcons.setData(this);
    for(int i = 0; i <= 10; i++){
        ChoiceGrid::Item* item = btnIcons.addItem("", NULL, (void*)i);
        item->btn.SetOnPaint(winChangeVIP_t::paintButtonIcon);
        icons.push_back(item);
    }
    btnIcons.setSelected(icons[0]);

    window.AddObject(&lblDesc);
    lblDesc.SetPos(18, 92);
    lblDesc.SetWidth(150);
    lblDesc.SetHeight(12);
    lblDesc.SetFont("aafont");
    lblDesc.SetCaption(gettext("Enter a short description:"));
    lblDesc.SetBGActiveness(false);

    window.AddObject(&desc);
    desc.SetPos(18, 104);
    desc.SetWidth(250);
    desc.SetHeight(48);
    // trick to get a scrollbar to appear but basically do nothing?
    desc.SetVirtualSize(250, 49);

    window.AddObject(&notify.pnlPanel);
    notify.SetCaption("Notify at login");
    notify.SetPos(18, 166);
    notify.SetSize(250, 22);

    window.AddObject(&sepBottom);
    sepBottom.SetPos(18, 208);
    sepBottom.SetWidth(250);
    sepBottom.SetHeight(2);
    sepBottom.SetSkin(&g_skin.txt);
    sepBottom.SetBGColor(.2,.2,.2,1.);

    window.AddObject(&btnOk);
    btnOk.SetCaption("Ok");
    btnOk.SetFont("minifont", 8);
    btnOk.SetPos(178, 218);
    btnOk.SetWidth(44);
    btnOk.SetHeight(20);
    btnOk.SetCustomData(this);
    btnOk.SetOnClick(winChangeVIP_t::onButtonPressed);

    window.AddObject(&btnCancel);
    btnCancel.SetCaption("Cancel");
    btnCancel.SetFont("minifont", 8);
    btnCancel.SetPos(230, 218);
    btnCancel.SetWidth(44);
    btnCancel.SetHeight(20);
    btnCancel.SetCustomData(this);
    btnCancel.SetOnClick(winChangeVIP_t::onButtonPressed);
}
winChangeVIP_t::~winChangeVIP_t()
{
}

void winChangeVIP_t::onButtonPressed(glictPos* pos, glictContainer* caller)
{
    winChangeVIP_t* wch = (winChangeVIP_t*)caller->GetCustomData();
    //GM_Gameworld *gw = (GM_Gameworld*)g_game;
    if(caller == &(wch->btnOk)){
        // TODO
    }
    wch->desc.SetCaption("");
    wch->window.SetVisible(false);
}
void winChangeVIP_t::paintButtonIcon(glictRect *real, glictRect *clipped, glictContainer *caller)
{
    ChoiceGrid::Item* pi = (ChoiceGrid::Item*)caller->GetCustomData();
    int type = VOIDP2INT(pi->data);
    if(type == 0) return;
    int x = 12 * ((type > 5) ? (type - 6) : type-1);
    int y = 12 * (type > 5);

    g_engine->getUISprite()->Blit(real->left+2, real->top+2, (289 + x) ,  (72 + y) , 12, 12);
}

void winChangeVIP_t::launch(std::string name)
{
    lblName.SetCaption(name);
    this->window.SetVisible(true);
    g_game->centerWindow(&this->window);
}
