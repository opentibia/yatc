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
    m_pnl.AddObject(&list);
    list.SetPos(3, 5);
    list.SetBGActiveness(false);
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
    list.AddObject(&pi->pnl);

    list.SetHeight(14*items.size());
    m_pnl.SetHeight(list.GetHeight() + 10);
    list.SetWidth(114);

}

void Popup::mouseOver(float x, float y) {
    for (std::vector<Popup::Item*>::iterator it = items.begin(); it != items.end(); it++) {
        Popup::Item* pi = *it;

        pi->pnl.SetBGActiveness(false);
    }
    if (cursorInside(x,y)) {
        y -= (m_pnl.GetY() + list.GetY());
        y /= 14;
        if (y < items.size() && items[y]->txt != "-")
            items[y]->pnl.SetBGActiveness(true);
        else if (items[y]->txt != "-") // separator
            printf("Warning: popup tried to highligh nonexisting listitem\n");
    }

}

void Popup::mouseClick(float x, float y) {
    if (cursorInside(x,y)) {
        y -= (m_pnl.GetY() + list.GetY());
        y /= 14;
        if (y < items.size())
            if (items[y]->cb)
                items[y]->cb(items[y]);
    }
    prepareToDie();
}

bool Popup::cursorInside(float x, float y) {
    return (x >= (m_pnl.GetX() + list.GetX()) && x < (m_pnl.GetX() + list.GetX()) + list.GetWidth() && y >= (m_pnl.GetY() + list.GetY()) && y < (m_pnl.GetY() + list.GetY()) + list.GetHeight());

}
