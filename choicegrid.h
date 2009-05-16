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


#ifndef __CHOICES_H
#define __CHOICES_H

#include <string>
#include <GLICT/panel.h>
#include <GLICT/button.h>
class ChoiceGrid
{
public:
    struct Item;
    typedef void (*ItemCallback_t)(Item *parent, Item *olditem);
    typedef void (*Callback_t)(ChoiceGrid* parent, Item* item, Item* olditem);

    struct Item {
        std::string txt;

        glictButton btn;
        ItemCallback_t cb;
        void* data;
        class ChoiceGrid* parent;
    };

    ChoiceGrid();
    ~ChoiceGrid();
    ChoiceGrid::Item* addItem(const std::string &txt, ItemCallback_t cb, void* data=NULL, bool enabled=true);
    void setRows(int rows);
    void setPadding(int w, int h);
    void setItemSize(int w, int h);
    void rebuildGrid();
    void setOnClick(Callback_t func) { cb=func; }
    void setSelected(ChoiceGrid::Item* newitem);
    void setData(void* data) { this->data = data; }
    void setFont(const std::string& font) { this->font = font; }
    glictPanel* getGrid() { return &grid; }

    ChoiceGrid::Item* getSelected() { return selected; }
    const ChoiceGrid::Item* getSelected() const { return selected; }
    void* getData() { return data; }
    int getCount() const { return items.size(); }

private:
    glictPanel grid;
    std::vector<Item*> items;
    int paddingw, paddingh;
    int rows;
    int w, h;
    Callback_t cb;
    Item* selected;
    void* data;
    std::string font;

    static void onClick(glictPos* relmousepos, glictContainer* caller);


};

#endif
