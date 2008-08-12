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


#ifndef __UI_HEALTH_H
#define __UI_HEALTH_H

#include <GLICT/container.h>
#include <GLICT/panel.h>
#include <GLICT/textbox.h>
#include <GLICT/button.h>
#include <GLICT/window.h>
#include <math.h>
#include "engine.h"
#include "skin.h"

class pnlHealth_t {
public:
    glictWindow panel; // 170x34

    glictPanel pnlHealth; // 220,76 12x11 => 7,5 12x11
    glictPanel pnlMana; // 220,87 12x11 => 7,19 12x11
    // empty 96,64 90x11
    glictPanel pnlHealthBar; // 96,75 90x11 => 25,6 90x11
    glictPanel pnlManaBar; // 96 86 90x11 => 25,20 90x11

    glictPanel lblHealth;
    glictPanel lblMana;


	pnlHealth_t() {

        panel.SetCaption("Health");
		panel.SetWidth(170);
		panel.SetHeight(34);

		panel.AddObject(&pnlHealth);
		pnlHealth.SetPos(7,5);
		pnlHealth.SetWidth(12); pnlHealth.SetHeight(11);
		pnlHealth.SetOnPaint(paintHealth);
		pnlHealth.SetCustomData(this);

		panel.AddObject(&pnlHealthBar);
		pnlHealthBar.SetPos(25,6);
		pnlHealthBar.SetWidth(90); pnlHealthBar.SetHeight(11);
        pnlHealthBar.SetOnPaint(paintHealthBar);
        pnlHealthBar.SetCustomData(this);



        panel.AddObject(&lblHealth);
        lblHealth.SetPos(140, 6);
        lblHealth.SetWidth(50); lblHealth.SetHeight(12);
        lblHealth.SetBGActiveness(false);


		panel.AddObject(&pnlMana);
		pnlMana.SetPos(7,19);
		pnlMana.SetWidth(12); pnlMana.SetHeight(11);
		pnlMana.SetOnPaint(paintMana);
		pnlMana.SetCustomData(this);

		panel.AddObject(&pnlManaBar);
		pnlManaBar.SetPos(25,20);
		pnlManaBar.SetWidth(90); pnlManaBar.SetHeight(11);
		pnlManaBar.SetOnPaint(paintManaBar);
		pnlManaBar.SetCustomData(this);

        panel.AddObject(&lblMana);
        lblMana.SetPos(140, 20);
        lblMana.SetWidth(50); lblMana.SetHeight(12);
        lblMana.SetBGActiveness(false);

	}

    static void paintHealth(glictRect *real, glictRect *clipped, glictContainer *caller){
        //pnlHealth_t* ph = ((pnlHealth_t*)caller->GetCustomData());
        g_engine->getUISprite()->Blit((int)real->left, (int)real->top, 220,76, 12,11);
    }
    static void paintMana(glictRect *real, glictRect *clipped, glictContainer *caller){
        //pnlHealth_t* ph = ((pnlHealth_t*)caller->GetCustomData());
        g_engine->getUISprite()->Blit((int)real->left, (int)real->top, 220,87, 12,11);
    }
    static void paintHealthBar(glictRect *real, glictRect *clipped, glictContainer *caller){
        float health = ((float)GlobalVariables::getPlayerStat(STAT_HEALTH))/GlobalVariables::getPlayerStat(STAT_HEALTH_MAX);
        //pnlHealth_t* ph = ((pnlHealth_t*)caller->GetCustomData());

        g_engine->getUISprite()->Blit((int)real->left, (int)real->top, 96,75, 90 * health,11);
        g_engine->getUISprite()->Blit(floor((int)real->left + 90*health), (int)real->top, floor(96 + 90*health),64, ceil(90 * (1-health)),11);
    }
    static void paintManaBar(glictRect *real, glictRect *clipped, glictContainer *caller){
        float mana = ((float)GlobalVariables::getPlayerStat(STAT_MANA))/GlobalVariables::getPlayerStat(STAT_MANA_MAX);
        //pnlHealth_t* ph = ((pnlHealth_t*)caller->GetCustomData());
        g_engine->getUISprite()->Blit((int)real->left, (int)real->top, 96, 86, 90*mana,11);
        g_engine->getUISprite()->Blit(floor((int)real->left + 90*mana), (int)real->top, floor(96 + 90*mana),64, ceil(90 * (1-mana)),11);
    }


    void updateSelf() {
        std::stringstream s;

        s.str("");
		s << GlobalVariables::getPlayerStat(STAT_HEALTH);
		lblHealth.SetCaption(s.str());

		s.str("");
		s << GlobalVariables::getPlayerStat(STAT_MANA);
		lblMana.SetCaption(s.str());
    }


	static void inventoryItemOnPaint(glictRect *real, glictRect *clipped, glictContainer *caller);
	static void inventoryItemOnClick(glictPos *relmousepos, glictContainer* callerclass);


};


#endif

