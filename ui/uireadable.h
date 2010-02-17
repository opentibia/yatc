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


#ifndef __UI_READABLE_H__
#define __UI_READABLE_H__

#include <GLICT/window.h>
#include <GLICT/textbox.h>
#include <GLICT/button.h>
#include <GLICT/panel.h>

#include "../skin.h"
#include "../gamecontent/item.h"

#if defined(HAVE_LIBINTL_H)
	#include<libintl.h>
#else
	#define gettext(x) (x)
#endif

class winReadable_t{
public:
    glictWindow window;
    glictPanel pnlText;
    glictTextbox txtText;
    glictPanel lblLabel;
    glictPanel pnlItem;
    glictPanel pnlSep;
    glictButton btnOk;

    glictPanel lblEdit; // dunno what to do with this yet...

    uint16_t windowID;

    Item* dispItem;

    winReadable_t(){
        dispItem = NULL;
        windowID = 0;

        window.SetCaption(gettext("Show Text"));
        window.SetWidth(286);
        window.SetHeight(278);

        window.AddObject(&pnlItem);
        pnlItem.SetPos(15, 15);
        pnlItem.SetWidth(32);
        pnlItem.SetHeight(32);
        pnlItem.SetOnPaint(winReadable_t::onRenderItem);
        pnlItem.SetBGActiveness(false);
        pnlItem.SetCustomData(NULL);

        window.AddObject(&lblLabel);
        lblLabel.SetPos(58, 36);
        lblLabel.SetWidth(136);
        lblLabel.SetHeight(12);
        lblLabel.SetCaption(gettext("You read the following."));
        lblLabel.SetBGActiveness(false);

        window.AddObject(&pnlText);
        pnlText.SetPos(15, 75);
        pnlText.SetWidth(250);
        pnlText.SetHeight(132);
        pnlText.SetVirtualSize(250, 133);
        pnlText.SetBGActiveness(false);

        pnlText.AddObject(&txtText);
        txtText.SetPos(0, 0);
        txtText.SetWidth(250);
        txtText.SetHeight(133);

        window.AddObject(&pnlSep);
        pnlSep.SetPos(8, 226);
        pnlSep.SetWidth(262);
        pnlSep.SetHeight(2);
        pnlSep.SetSkin(&g_skin.txt);

        window.AddObject(&btnOk);
        btnOk.SetPos(226, 236);
        btnOk.SetWidth(43);
        btnOk.SetHeight(20);
        btnOk.SetOnClick(winReadable_t::onClose);
        btnOk.SetCustomData(this);
        btnOk.SetCaption(gettext("Ok"));
    }
    ~winReadable_t(){
        if(dispItem)
            delete dispItem;
    }

    void open(uint16_t id, uint16_t wid, const std::string& text, const std::string& lastEditor, const std::string& lastEditDate){
        if(dispItem)
            delete dispItem;
        dispItem = Item::CreateItem(id, 1);
        pnlItem.SetCustomData((void*)dispItem);
        txtText.SetCaption(text);
        txtText.SetHeight(glictFontNumberOfLines(text.c_str()) * 12);
        pnlText.SetVirtualSize(250, glictFontNumberOfLines(text.c_str()) * 12);
        windowID = wid;

        window.SetVisible(true);
    }
    static void onClose(glictPos* pos, glictContainer* caller);
    static void onRenderItem(glictRect *real, glictRect *clipped, glictContainer *caller) {
        Item* dispItem = (Item*)caller->GetCustomData();
        if(dispItem)
            dispItem->Blit((int)real->left, (int)real->top);
    }
};

#endif
