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
#include "../gm_gameworld.h"
#include "../net/protocolgame.h"
#if defined(HAVE_LIBINTL_H)
    #include <libintl.h>
#else
    #define gettext(x) (x)
#endif

void winOutfit_t::onClose(glictPos* pos, glictContainer *caller) {
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
    btnHead.SetOnClick(onHead);
    btnHead.SetCustomData(this);

    window.AddObject(&btnBody);
    btnBody.SetPos(167,39);
    btnBody.SetWidth(58);
    btnBody.SetHeight(20);
    btnBody.SetCaption(gettext("Primary"));
    btnBody.SetFont("minifont");
    btnBody.SetOnClick(onBody);
    btnBody.SetCustomData(this);

    window.AddObject(&btnLegs);
    btnLegs.SetPos(167,63);
    btnLegs.SetWidth(58);
    btnLegs.SetHeight(20);
    btnLegs.SetCaption(gettext("Secondary"));
    btnLegs.SetFont("minifont");
    btnLegs.SetOnClick(onLegs);
    btnLegs.SetCustomData(this);

    window.AddObject(&btnFeet);
    btnFeet.SetPos(167,87);
    btnFeet.SetWidth(58);
    btnFeet.SetHeight(20);
    btnFeet.SetCaption(gettext("Detail"));
    btnFeet.SetFont("minifont");
    btnFeet.SetOnClick(onFeet);
    btnFeet.SetCustomData(this);

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
    btnOk.SetOnClick(onApply);
    btnOk.SetCustomData(this);

    window.AddObject(&btnCancel);
    btnCancel.SetPos(437,296);
    btnCancel.SetWidth(43);
    btnCancel.SetHeight(20);
    btnCancel.SetCaption(gettext("Cancel"));
    btnCancel.SetFont("minifont");
    btnCancel.SetCustomData(&window);
    btnCancel.SetOnClick(onClose);

    for (int j = 0; j < 7; j++) {
        for (int i = 0; i < 19; i++) {
            glictButton *btn = &btnColor[j*19+i];
            window.AddObject(btn);

            btn->SetPos(229 + i*13,15 + j*13);
            btn->SetWidth(12);
            btn->SetHeight(12);

            btn->SetCaption("");
            btn->SetOnClick(onColor);

            btn->SetCustomData(this);
            #if (GLICT_APIREV < 77)
            #warning To enjoy colors in outfit changing window, you need GLICT APIREV 77. Functionality otherwise ok.
            #endif
            btn->SetOnPaint(winOutfit_t::onBtnPaint);

        }
    }

    // TODO check! current way of deleting this in destructor may be invalid
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
    uint32_t ticks = SDL_GetTicks();
    dispCreature->loadOutfit();
    printf("Loaded creature in %d ticks\n", SDL_GetTicks() - ticks);
}
winOutfit_t::~winOutfit_t(){
    Creatures::getInstance().removeCreature(0xFFFFFF00);
}

void winOutfit_t::onBtnPaint(glictRect *real, glictRect *clipped, glictContainer *caller) {

    winOutfit_t* wo = (winOutfit_t*)caller->GetCustomData();
    int colorid = (glictButton*)caller - &wo->btnColor[0];

    uint32_t col = Creatures::OutfitLookupTable[colorid];
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
    for(std::list<AvailOutfit_t>::iterator it = m_availableOutfits.begin(); it!=m_availableOutfits.end();it++)
        if(current.m_looktype == it->id)
            m_currentOutfit = it;
    rebuildGfx();

    setActivePart(HEAD);
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

void winOutfit_t::setActivePart(winOutfit_t_PARTS wp){
    btnHead.SetHold(wp==HEAD);
    btnBody.SetHold(wp==BODY);
    btnLegs.SetHold(wp==LEGS);
    btnFeet.SetHold(wp==FEET);

    if (wp==HEAD)
        setActiveColor(dispCreature->getOutfit().m_lookhead);
    if (wp==BODY)
        setActiveColor(dispCreature->getOutfit().m_lookbody);
    if (wp==LEGS)
        setActiveColor(dispCreature->getOutfit().m_looklegs);
    if (wp==FEET)
        setActiveColor(dispCreature->getOutfit().m_lookfeet);

    m_currentPart=wp;

}

void winOutfit_t::setActiveColor(uint8_t color){
    for(int i=0;i<19*7;i++){
        btnColor[i].SetHold(color==i);
    }
}

void winOutfit_t::onColor(glictPos* pos, glictContainer *caller){
    winOutfit_t* wo = (winOutfit_t*)caller->GetCustomData();
    int colorid = (glictButton*)caller - &wo->btnColor[0];
    wo->setActiveColor(colorid);
    winOutfit_t_PARTS wp = wo->m_currentPart;
    if(wp==HEAD)
        wo->dispCreature->getOutfit().m_lookhead = colorid;
    if(wp==BODY)
        wo->dispCreature->getOutfit().m_lookbody = colorid;
    if(wp==LEGS)
        wo->dispCreature->getOutfit().m_looklegs = colorid;
    if(wp==FEET)
        wo->dispCreature->getOutfit().m_lookfeet = colorid;
    wo->rebuildGfx();
}

void winOutfit_t::onHead(glictPos* pos, glictContainer *caller){
    winOutfit_t* wo = (winOutfit_t*)caller->GetCustomData();
    wo->setActivePart(HEAD);
}
void winOutfit_t::onBody(glictPos* pos, glictContainer *caller){
    winOutfit_t* wo = (winOutfit_t*)caller->GetCustomData();
    wo->setActivePart(BODY);
}
void winOutfit_t::onLegs(glictPos* pos, glictContainer *caller){
    winOutfit_t* wo = (winOutfit_t*)caller->GetCustomData();
    wo->setActivePart(LEGS);
}
void winOutfit_t::onFeet(glictPos* pos, glictContainer *caller){
    winOutfit_t* wo = (winOutfit_t*)caller->GetCustomData();
    wo->setActivePart(FEET);
}

void winOutfit_t::onApply(glictPos* pos, glictContainer *caller) {
    winOutfit_t* wo = (winOutfit_t*)caller->GetCustomData();
    GM_Gameworld *gw = ((GM_Gameworld*)g_game);
    wo->window.SetVisible(false);
    gw->m_protocol->sendSetOutfit(
        wo->dispCreature->getOutfit().m_looktype,
        wo->dispCreature->getOutfit().m_lookhead,
        wo->dispCreature->getOutfit().m_lookbody,
        wo->dispCreature->getOutfit().m_looklegs,
        wo->dispCreature->getOutfit().m_lookfeet);

}
