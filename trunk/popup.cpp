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
Popup::Popup() {
    wantdeath = false;
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
    pi->pnl.SetCaption(txt == "-" ? "____________" : txt);
    pi->pnl.SetBGActiveness(false);
    pi->pnl.SetHeight(14);
    pi->pnl.SetWidth(150);
    pi->pnl.SetFont("gamefont");
    pi->pnl.SetFocusable(false);
    pi->pnl.SetBGColor(.4, .4, .4, 1.);
    pi->parent = this;
    items.push_back(pi);
    list.AddObject(&pi->pnl);

    list.SetHeight(14*items.size());
    list.SetWidth(150);
    list.SetBGColor(.1, .1, .1,1);

}



void Popup::mouseOver(float x, float y) {
    for (std::vector<Popup::Item*>::iterator it = items.begin(); it != items.end(); it++) {
        Popup::Item* pi = *it;

        pi->pnl.SetBGActiveness(false);
    }
    if (cursorInside(x,y)) {
        y -= list.GetY();
        y /= 14;
        if (y < items.size() && items[y]->txt != "-")
            items[y]->pnl.SetBGActiveness(true);
        else if (items[y]->txt != "-") // separator
            printf("Warning: popup tried to highligh nonexisting listitem\n");
    }

}

void Popup::mouseClick(float x, float y) {
    if (cursorInside(x,y)) {
        y -= list.GetY();
        y /= 14;
        if (y < items.size())
            if (items[y]->cb)
                items[y]->cb(items[y]);
    }
    prepareToDie();
}

bool Popup::cursorInside(float x, float y) {
    return (x >= list.GetX() && x < list.GetX() + list.GetWidth() && y >= list.GetY() && y < list.GetY() + list.GetHeight());

}
