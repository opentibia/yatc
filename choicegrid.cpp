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

#include "choicegrid.h"
ChoiceGrid::ChoiceGrid()
{
    paddingw=0; paddingh=0;
    rows=1;
    w=100; h=20;
    selected=NULL;
    cb=NULL;
    grid.SetBGActiveness(false);
    font = "minifont";
}
ChoiceGrid::~ChoiceGrid()
{
}

ChoiceGrid::Item* ChoiceGrid::addItem(const std::string &txt, ChoiceGrid::ItemCallback_t cb, void* data, bool enabled)
{
    ChoiceGrid::Item* pi = new ChoiceGrid::Item;
    pi->btn.SetCustomData(pi);
    pi->txt = txt;
    pi->cb = cb;
    pi->data = data;
    pi->btn.SetCaption(txt);
    pi->btn.SetHeight(h);
    pi->btn.SetWidth(w);
    pi->btn.SetFont(font.c_str());
    pi->btn.SetFocusable(false);
    pi->btn.SetBGColor(.4, .4, .4, 1.);
    pi->btn.SetOnClick(ChoiceGrid::onClick);
    pi->btn.SetCustomData(pi);
    pi->btn.SetEnabled(enabled);
    pi->parent = this;

    pi->btn.SetPos((items.size()%rows)*(w+paddingw) + paddingw, (items.size()/rows)*(h+paddingh) + paddingh);


    items.push_back(pi);

    grid.AddObject(&pi->btn);
    grid.SetWidth((w+paddingw)*rows + paddingw);
    grid.SetHeight((h+paddingh)*(items.size()/rows + 1) + paddingh);
    grid.SetBGColor(.1, .1, .1,1);
    return pi;
}


void ChoiceGrid::setRows(int rows)
{
    this->rows = rows;
}
void ChoiceGrid::setPadding(int w, int h)
{
    paddingw = w; paddingh = h;
}
void ChoiceGrid::setItemSize(int w, int h)
{
    this->w = w; this->h = h;
}
void ChoiceGrid::onClick(glictPos* relmousepos, glictContainer* caller)
{
    ChoiceGrid::Item* it = (ChoiceGrid::Item*)caller->GetCustomData();
    ChoiceGrid::Item* oldit=it->parent->selected;
    if (oldit) {
        oldit->btn.SetHold(false);
    }
    it->parent->selected = it;
    it->btn.SetHold(true);
    if (it->cb)
    {
        it->cb(it,oldit);
    }
    if (it->parent->cb)
    {
        it->parent->cb(it->parent, it,oldit);
    }
}
void ChoiceGrid::setSelected(ChoiceGrid::Item* newitem)
{
    if(selected) selected->btn.SetHold(false);
    selected=newitem;
    if(selected) selected->btn.SetHold(true);
}
