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

#include <sstream>
#include "../gamecontent/creature.h"
#include "uioutfit.h"
#include "../skin.h"
#ifndef __APPLE__
    #include <libintl.h>
#else
    #define gettext(x) (x)
#endif

static void __uioutfit__closeme(glictPos* pos, glictContainer *caller) { // REMOVE ME
    ((glictWindow*)caller->GetCustomData())->SetVisible(false);
}


winOutfit_t::winOutfit_t() {
    window.SetCaption(gettext("Set Outfit"));
    window.SetWidth(490);
    window.SetHeight(323);

    window.AddObject(&pnlOutfit);
    pnlOutfit.SetPos(14,15);
    pnlOutfit.SetWidth(140);
    pnlOutfit.SetHeight(140);
    pnlOutfit.SetOnPaint(onGfxPaint);
    pnlOutfit.SetCustomData(this);
    pnlOutfit.SetSkin(&g_skin.inv);
    //pnlOutfit.SetBGActiveness(false);

    window.AddObject(&btnHead);
    btnHead.SetPos(167,15);
    btnHead.SetWidth(58);
    btnHead.SetHeight(20);
    btnHead.SetCaption(gettext("Head"));
    btnHead.SetFont("minifont");

    window.AddObject(&btnPrimary);
    btnPrimary.SetPos(167,39);
    btnPrimary.SetWidth(58);
    btnPrimary.SetHeight(20);
    btnPrimary.SetCaption(gettext("Primary"));
    btnPrimary.SetFont("minifont");

    window.AddObject(&btnSecondary);
    btnSecondary.SetPos(167,63);
    btnSecondary.SetWidth(58);
    btnSecondary.SetHeight(20);
    btnSecondary.SetCaption(gettext("Secondary"));
    btnSecondary.SetFont("minifont");

    window.AddObject(&btnDetail);
    btnDetail.SetPos(167,87);
    btnDetail.SetWidth(58);
    btnDetail.SetHeight(20);
    btnDetail.SetCaption(gettext("Detail"));
    btnDetail.SetFont("minifont");

    window.AddObject(&lblInstructions);
    lblInstructions.SetPos(167,120);
    lblInstructions.SetCaption(
        // the following text is not in-game original
        // it was INTENTIONALLY not reproduced verbatim
        // to prevent copyright infringement
        gettext("Pick an outfit and choose colors for parts of\n"
        "your character's avatar.\n"
        "Addons are activated by ticking the appropriate box.\n"
        "To activate particular addon, premium players can\n"
        "solve various quests."));
    lblInstructions.SetWidth(310);
    lblInstructions.SetHeight(90);
    lblInstructions.SetBGActiveness(false);

    window.AddObject(&btnLeft);
    btnLeft.SetPos(14,159);
    btnLeft.SetWidth(20);
    btnLeft.SetHeight(20);
    btnLeft.SetCaption("<"); // FIXME (ivucica#4#): graphic should be extracted from .pic and displayed here
    btnLeft.SetOnClick(onPrev);
    btnLeft.SetCustomData(this);

    window.AddObject(&btnRight);
    btnRight.SetPos(134,159);
    btnRight.SetWidth(20);
    btnRight.SetHeight(20);
    btnRight.SetCaption(">"); // FIXME (ivucica#4#): graphic should be extracted from .pic and displayed here
    btnRight.SetOnClick(onNext);
    btnRight.SetCustomData(this);

    window.AddObject(&lblName);
    lblName.SetPos(38,159);
    lblName.SetWidth(92);
    lblName.SetHeight(20);
    lblName.SetCaption("?");
    lblName.SetHold(true);

    for (int i = 0; i < 3; i++) {
        std::stringstream s;
        s << gettext("Addon ") << (i+1);
        window.AddObject(&chkAddon[i].pnlPanel);
        chkAddon[i].SetPos(14,192 + 26*i);
        chkAddon[i].SetWidth(140);
        chkAddon[i].SetHeight(22);
        chkAddon[i].SetCaption(s.str());
    }

    window.AddObject(&pnlSeparator);
    pnlSeparator.SetPos(9,286);
    pnlSeparator.SetWidth(471);
    pnlSeparator.SetHeight(2);

    window.AddObject(&btnOk);
    btnOk.SetPos(384,296);
    btnOk.SetWidth(43);
    btnOk.SetHeight(20);
    btnOk.SetCaption(gettext("Ok"));
    btnOk.SetFont("minifont");
    btnOk.SetEnabled(false);

    window.AddObject(&btnCancel);
    btnCancel.SetPos(437,296);
    btnCancel.SetWidth(43);
    btnCancel.SetHeight(20);
    btnCancel.SetCaption(gettext("Cancel"));
    btnCancel.SetFont("minifont");
    // temporary:
    btnCancel.SetCustomData(&window);
    btnCancel.SetOnClick(__uioutfit__closeme);
    // end temporary

    for (int j = 0; j < 7; j++) {
        for (int i = 0; i < 19; i++) {
            glictButton *btn = &btnColor[j*19+i];
            window.AddObject(btn);

            btn->SetPos(229 + i*13,15 + j*13);
            btn->SetWidth(12);
            btn->SetHeight(12);

            btn->SetCaption("");

            btn->SetCustomData((void*)(j*19+i));
            #if (GLICT_APIREV < 77)
            #warning To enjoy colors in outfit changing window, you need GLICT APIREV 77. Functionality otherwise ok.
            #endif
            btn->SetOnPaint(winOutfit_t::onBtnPaint);

        }
    }

    // FIXME we don't delete this in destructor
    // FIXME is ffffff00 really a satisfying id that won't be used by the game itself?
    dispCreature = Creatures::getInstance().addCreature(0xFFFFFF00);

    dispCreature->getOutfit().m_looktype = 128;
    dispCreature->getOutfit().m_lookbody = 40;
    dispCreature->getOutfit().m_looklegs = 40;
    dispCreature->getOutfit().m_lookhead = 40;
    dispCreature->getOutfit().m_lookfeet = 40;
    dispCreature->getOutfit().m_lookitem = 0;
    dispCreature->getOutfit().m_addons = 0;

    dispCreature->setLookDir((Direction)2);
    dispCreature->setTurnDir((Direction)2);

    printf("Configured outfit. LOADING GFX.\n");

    dispCreature->loadOutfit();
}


void winOutfit_t::onBtnPaint(glictRect *real, glictRect *clipped, glictContainer *caller) {
    uint32_t col = Creatures::OutfitLookupTable[(int)(caller->GetCustomData())];
    oRGBA c(((col >> 16) & 0xFF), ((col >> 8) & 0xFF), (col & 0xFF), 255);

    g_engine->drawRectangle(real->left + 2, real->top + 2, 8, 8, c);
}
void winOutfit_t::onGfxPaint(glictRect *real, glictRect *clipped, glictContainer *caller) {
    winOutfit_t* wo = (winOutfit_t*)(caller->GetCustomData());
    if (wo->dispCreature) {
        wo->dispCreature->Blit((int)real->left+64, (int)real->top+64,2);
    }
}


void winOutfit_t::openSelf(const Outfit_t& current, const std::list<AvailOutfit_t>& available){
    // FIXME doesnt take argument "current" into consideration
    dispCreature->getOutfit() = current;
    window.SetVisible(true);
    m_availableOutfits = available;
    m_currentOutfit = m_availableOutfits.begin();
    rebuildGfx();
}

void winOutfit_t::rebuildGfx(){
    lblName.SetCaption(m_currentOutfit->name);
    dispCreature->getOutfit().m_looktype = m_currentOutfit->id;
    dispCreature->unloadGfx();
    dispCreature->loadOutfit();
}

void winOutfit_t::onNext(glictPos* pos, glictContainer *caller){
    winOutfit_t *wo = (winOutfit_t*)caller->GetCustomData();
    wo->m_currentOutfit++;
    if(wo->m_currentOutfit==wo->m_availableOutfits.end())
        wo->m_currentOutfit=wo->m_availableOutfits.begin();
    wo->rebuildGfx();
}
void winOutfit_t::onPrev(glictPos* pos, glictContainer *caller){
    winOutfit_t *wo = (winOutfit_t*)caller->GetCustomData();
    if(wo->m_currentOutfit==wo->m_availableOutfits.begin())
        wo->m_currentOutfit=wo->m_availableOutfits.end();
    wo->m_currentOutfit--;
    wo->rebuildGfx();
}
