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

#ifndef __POPUP_H
#define __POPUP_H

// A few words about this class.
//
// We need a generic menu popup. The idea is pretty simple and
// similar to what we do with regards to resolutions in
// graphic options.
//
// We'll have a glictList which we'll populate with menu options.
// Each menu option will be a glictPanel. We'll handle mouseclick
// and mousemove in both glictList and glictPanel. Upon mouseclick
// we'll call the callback specified when the calling GameMode was
// creating the popup.
//
// On the other hand, mousemove will be used for producing rollover
// effect. It will be handled on listbox too, for unselecting
// all menu items. Unselection will also be produced by moving the
// mouse out of the popup; this will be caused by the GameMode.
//
// So, let's get crackin', then :)

#include <string>
#include <vector>
#include <GLICT/panel.h>
#include <GLICT/list.h>


typedef glictPanel popupItemGUIType;
class Popup {

public:
    struct Item;
    typedef void (*Callback_t)(struct Item *parent);

    struct Item {
        std::string txt;

        popupItemGUIType pnl;
        Callback_t cb;
        void* data;
        class Popup* parent;
    };

    Popup();
    ~Popup();

    void addItem(const std::string &txt, Callback_t cb, void* data=NULL);
    void mouseOver(float x, float y);
    //static void onClick(glictPos* pos, glictContainer *caller);
    void mouseClick(float x, float y);


    glictList* getGlictList() { return &list; }

    void prepareToDie() { wantdeath = true; }
    bool wantsDeath() const { return wantdeath; }
    bool cursorInside(float x, float y);

    inline void setOwner(void* owner) { this->owner = owner; }
    inline void*getOwner() const      { return owner; }

private:
    glictList list;
    std::vector<Item*> items;
    bool wantdeath;
    void* owner;
};

#endif
