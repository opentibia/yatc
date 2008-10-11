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

#ifndef _YATC_UIOUTFIT_H
#define _YATC_UIOUTFIT_H

#include <GLICT/panel.h>
#include <GLICT/window.h>
#include <GLICT/button.h>
#include "checkbox.h"
class winOutfit_t
{
public:
    glictWindow window;
    glictPanel pnlOutfit;
    glictButton btnHead, btnPrimary, btnSecondary, btnDetail;
    glictPanel lblInstructions;
    glictButton btnLeft, btnRight;
    glictButton lblName; // only to use "buttondown" skin, and to center the text
    uiCheckbox chkAddon[3];

    glictPanel pnlSeparator;

    glictButton btnOk, btnCancel;

    glictButton btnColor[19*7];

    winOutfit_t();

    void openSelf(const Outfit_t& current, const std::list<AvailOutfit_t>& available);
    void rebuildGfx();


    static void onNext(glictPos* pos, glictContainer *caller);
    static void onPrev(glictPos* pos, glictContainer *caller);


    std::list<AvailOutfit_t> m_availableOutfits;
    std::list<AvailOutfit_t>::iterator m_currentOutfit;

    static void onBtnPaint(glictRect *real, glictRect *clipped, glictContainer *caller);
    static void onGfxPaint(glictRect *real, glictRect *clipped, glictContainer *caller);




    Creature *dispCreature;
};
#endif
