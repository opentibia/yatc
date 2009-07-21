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

#include "popup.h"
#include "skin.h"

Popup::Popup() {
    wantdeath = false;
    // NOTE (nfries88): This isn't the right skin. The right skin is like the windows, but without the header.
    //	good enough as a temporary fix though.
    m_pnl.SetSkin(&g_skin.ptb);
    m_pnl.SetWidth(120);
    m_pnl.SetHeight(10);
    m_pnl.AddObject(&m_list);
    m_list.SetPos(3, 5);
    m_list.SetBGActiveness(false);
    m_maxw = 114; // minimum size is 114
}
Popup::~Popup() {
    for (std::vector<Popup::Item*>::iterator it = items.begin(); it != items.end(); it++) {
        delete *it;
    }
}

void Popup::addItem(const std::string &txt, Callback_t cb, void* data) {
    Popup::Item* pi = new Popup::Item;
    pi->pnl.SetCustomData(pi);
    pi->txt = txt;
    pi->cb = cb;
    pi->data = data;
    if(txt != "-") {
		pi->pnl.SetCaption(txt);
		pi->pnlSep = NULL;
    }
    else {
    	pi->pnlSep = new glictPanel;
    	pi->pnl.AddObject(pi->pnlSep);
    	pi->pnlSep->SetPos(0, 6);
    	pi->pnlSep->SetHeight(2);
    	pi->pnlSep->SetWidth(114);
    	pi->pnlSep->SetSkin(&g_skin.chk);
    	pi->pnlSep->SetFocusable(false);
    }
    pi->pnl.SetBGActiveness(false);
    pi->pnl.SetHeight(14);
    pi->pnl.SetWidth(114);
    pi->pnl.SetFont("gamefont");
    pi->pnl.SetFocusable(false);
    pi->pnl.SetBGColor(.7, .7, .7, 1.);
    pi->parent = this;
    items.push_back(pi);
    m_list.AddObject(&pi->pnl);

    m_list.SetHeight(14*items.size());
    m_pnl.SetHeight(m_list.GetHeight() + 10);
    m_list.SetWidth(114);

    if (glictFontSize(txt.c_str(), "gamefont")>m_maxw)
    {
        m_maxw = int(glictFontSize(txt.c_str(), "gamefont"));
    }

}

void Popup::updateWidths()
{
    m_list.SetWidth(m_maxw);
    for (std::vector<Item*>::iterator it = items.begin(); it != items.end(); it++)
    {
        if ((*it)->pnlSep)
        {
            (*it)->pnlSep->SetWidth(m_maxw);
        }
        (*it)->pnl.SetWidth(m_maxw);
    }
}

void Popup::mouseOver(int x, int y) {
    for (std::vector<Popup::Item*>::iterator it = items.begin(); it != items.end(); it++) {
        Popup::Item* pi = *it;

        pi->pnl.SetBGActiveness(false);
    }
    if (cursorInside(x,y)) {
        y -= int(m_pnl.GetY() + m_list.GetY());
        y /= 14;
        if (y < int(items.size()) && items[y]->txt != "-")
            items[y]->pnl.SetBGActiveness(true);
        else if (items[y]->txt != "-") // separator
            printf("Warning: popup tried to highligh nonexisting listitem\n");
    }
}

void Popup::mouseClick(int x, int y) {
    if (cursorInside(x,y)) {
        y -= int(m_pnl.GetY() + m_list.GetY());
        y /= 14;
        if (y < int(items.size()))
            if (items[y]->cb)
                items[y]->cb(items[y]);
    }
    prepareToDie();
}

bool Popup::cursorInside(int x, int y) {
    return (x >= (m_pnl.GetX() + m_list.GetX()) && x < (m_pnl.GetX() + m_list.GetX()) + m_list.GetWidth() && y >= (m_pnl.GetY() + m_list.GetY()) && y < (m_pnl.GetY() + m_list.GetY()) + m_list.GetHeight());

}
