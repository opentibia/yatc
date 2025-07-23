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

#include <iomanip>
#include "skills.h"
#include "sbvlpanel.h"
#include "../skin.h"

winSkills_t::winSkills_t()
{
    controller = NULL;

    window.SetWidth(160);
    window.SetHeight(GetDefaultHeight());
    window.SetCaption(gettext("Skills"));
    window.AddObject(&container);
    container.SetWidth(160);
    container.SetHeight(GetDefaultHeight());
    container.SetVirtualSize(160, 270);
    container.SetBGActiveness(false);


    container.AddObject(&lblExperienceLeft);
    container.AddObject(&lblExperienceRight);
    lblExperienceLeft.SetPos(5, 5);
    lblExperienceLeft.SetWidth(160);
    lblExperienceLeft.SetHeight(12);
    lblExperienceLeft.SetBGActiveness(false);
    lblExperienceLeft.SetCaption(gettext("Experience"));

    lblExperienceRight.SetPos(160-20, 5);
    lblExperienceRight.SetWidth(10);
    lblExperienceRight.SetHeight(12);
    lblExperienceRight.SetBGActiveness(false);
    lblExperienceRight.SetCaption("0");


    container.AddObject(&lblLevelLeft);
    container.AddObject(&lblLevelRight);
    lblLevelLeft.SetPos(5, 15);
    lblLevelLeft.SetWidth(160);
    lblLevelLeft.SetHeight(12);
    lblLevelLeft.SetBGActiveness(false);
    lblLevelLeft.SetCaption(gettext("Level"));

    lblLevelRight.SetPos(160 - 20, 15);
    lblLevelRight.SetWidth(10);
    lblLevelRight.SetHeight(12);
    lblLevelRight.SetBGActiveness(false);
    lblLevelRight.SetCaption("0");


    container.AddObject(&pbExperience);

    pbExperience.SetPos(5, 25);
    pbExperience.SetWidth(140);
    pbExperience.SetHeight(5);
    pbExperience.SetValue(40);
    #if GLICT_APIREV >= 105
    pbExperience.SetBGActiveness(false);
    pbExperience.SetBorderColor(glictColor(0, 0, 0, 1));
    pbExperience.SetFGColor(glictColor(.9,.2,.1,1.));
    #else
#if _MSC_VER
	#pragma message ("For visual improvements to skill bars, update to glict rev 105 or higher")
#else
    #warning For visual improvements to skill bars, update to glict rev 105 or higher
#endif
    #endif



    ////////////////////////////////

    container.AddObject(&lblHPLeft);
    container.AddObject(&lblHPRight);
    lblHPLeft.SetPos(5, 35);
    lblHPLeft.SetWidth(160);
    lblHPLeft.SetHeight(12);
    lblHPLeft.SetBGActiveness(false);
    lblHPLeft.SetCaption(gettext("Hit Points"));

    lblHPRight.SetPos(160 - 20, 35);
    lblHPRight.SetWidth(10);
    lblHPRight.SetHeight(12);
    lblHPRight.SetBGActiveness(false);
    lblHPRight.SetCaption("0");


    container.AddObject(&lblMPLeft);
    container.AddObject(&lblMPRight);
    lblMPLeft.SetPos(5, 45);
    lblMPLeft.SetWidth(160);
    lblMPLeft.SetHeight(12);
    lblMPLeft.SetBGActiveness(false);
    lblMPLeft.SetCaption(gettext("Mana"));

    lblMPRight.SetPos(160 - 20, 45);
    lblMPRight.SetWidth(10);
    lblMPRight.SetHeight(12);
    lblMPRight.SetBGActiveness(false);
    lblMPRight.SetCaption("0");


    container.AddObject(&lblSPLeft);
    container.AddObject(&lblSPRight);
    lblSPLeft.SetPos(5, 55);
    lblSPLeft.SetWidth(160);
    lblSPLeft.SetHeight(12);
    lblSPLeft.SetBGActiveness(false);
    lblSPLeft.SetCaption(gettext("Soul Points"));

    lblSPRight.SetPos(160 - 20, 55);
    lblSPRight.SetWidth(10);
    lblSPRight.SetHeight(12);
    lblSPRight.SetBGActiveness(false);
    lblSPRight.SetCaption("0");


    container.AddObject(&lblCapLeft);
    container.AddObject(&lblCapRight);
    lblCapLeft.SetPos(5, 65);
    lblCapLeft.SetWidth(160);
    lblCapLeft.SetHeight(12);
    lblCapLeft.SetBGActiveness(false);
    lblCapLeft.SetCaption(gettext("Cap"));

    lblCapRight.SetPos(160 - 20, 65);
    lblCapRight.SetWidth(10);
    lblCapRight.SetHeight(12);
    lblCapRight.SetBGActiveness(false);
    lblCapRight.SetCaption("0");


    container.AddObject(&lblStaLeft);
    container.AddObject(&lblStaRight);
    lblStaLeft.SetPos(5, 75);
    lblStaLeft.SetWidth(160);
    lblStaLeft.SetHeight(12);
    lblStaLeft.SetBGActiveness(false);
    lblStaLeft.SetCaption(gettext("Stamina"));

    lblStaRight.SetPos(160 - 20, 75);
    lblStaRight.SetWidth(10);
    lblStaRight.SetHeight(12);
    lblStaRight.SetBGActiveness(false);
    lblStaRight.SetCaption("0");


    container.AddObject(&pbStamina);

    pbStamina.SetPos(5, 85);
    pbStamina.SetWidth(140);
    pbStamina.SetHeight(5);
    pbStamina.SetValue(40);
    #if GLICT_APIREV >= 105
    pbStamina.SetBGActiveness(false);
    pbStamina.SetBorderColor(glictColor(0, 0, 0, 1));
    #else
#if _MSC_VER
	#pragma message ("GLICT too old, get 105 or newer for visual improvement on some progressbars")
#else
	#warning GLICT too old, get 105 or newer for visual improvement on some progressbars
#endif      
    #endif


    container.AddObject(&lblMagicLevelLeft);
    container.AddObject(&lblMagicLevelRight);
    lblMagicLevelLeft.SetPos(5, 95);
    lblMagicLevelLeft.SetWidth(160);
    lblMagicLevelLeft.SetHeight(12);
    lblMagicLevelLeft.SetBGActiveness(false);
    lblMagicLevelLeft.SetCaption(gettext("Magic Level"));

    lblMagicLevelRight.SetPos(160 - 20, 95);
    lblMagicLevelRight.SetWidth(10);
    lblMagicLevelRight.SetHeight(12);
    lblMagicLevelRight.SetBGActiveness(false);
    lblMagicLevelRight.SetCaption("0");


    container.AddObject(&pbMagicLevel);

    pbMagicLevel.SetPos(5, 105);
    pbMagicLevel.SetWidth(140);
    pbMagicLevel.SetHeight(5);
    pbMagicLevel.SetValue(40);
    #if GLICT_APIREV >= 105
    pbMagicLevel.SetBGActiveness(false);
    pbMagicLevel.SetBorderColor(glictColor(0, 0, 0, 1));
    pbMagicLevel.SetFGColor(glictColor(.9,.2,.1,1.));
    #else
#if _MSC_VER
	#pragma message ("For visual improvements to skill bars, update to glict rev 105 or higher")
#else
    #warning For visual improvements to skill bars, update to glict rev 105 or higher
#endif
    #endif




    container.AddObject(&pnlSeparator);
    pnlSeparator.SetPos(5,115);
    pnlSeparator.SetWidth(140);
    pnlSeparator.SetHeight(2);
    pnlSeparator.SetBGColor(.7, .7, .7, 1.);
    pnlSeparator.SetSkin(&g_skin.chk);




    for (int i = 0; i < 7; i++) {
        container.AddObject(&lblSkillLeft[i]);
        container.AddObject(&lblSkillRight[i]);
        lblSkillLeft[i].SetPos(5, 125 + i * 20);
        lblSkillLeft[i].SetWidth(160);
        lblSkillLeft[i].SetHeight(12);
        lblSkillLeft[i].SetBGActiveness(false);
        lblSkillLeft[i].SetCaption(std::string(gettext("Skill ")) + yatc_itoa(i) );

        lblSkillRight[i].SetPos(160 - 20, 125 + i*20);
        lblSkillRight[i].SetWidth(10);
        lblSkillRight[i].SetHeight(12);
        lblSkillRight[i].SetBGActiveness(false);
        lblSkillRight[i].SetCaption("0");


        container.AddObject(&pbSkill[i]);

        pbSkill[i].SetPos(5, 135 + i*20);
        pbSkill[i].SetWidth(140);
        pbSkill[i].SetHeight(5);
        pbSkill[i].SetValue(40);
        #if GLICT_APIREV >= 105
        pbSkill[i].SetBGActiveness(false);
        pbSkill[i].SetBorderColor(glictColor(0, 0, 0, 1));
        #else
#if _MSC_VER
	#pragma message ("For visual improvements to skill bars, update to glict rev 105 or higher")
#else
    #warning For visual improvements to skill bars, update to glict rev 105 or higher
#endif
        #endif
    }
    lblSkillLeft[0].SetCaption(gettext("Fist Fighting"));
    lblSkillLeft[1].SetCaption(gettext("Club Fighting"));
    lblSkillLeft[2].SetCaption(gettext("Sword Fighting"));
    lblSkillLeft[3].SetCaption(gettext("Axe Fighting"));
    lblSkillLeft[4].SetCaption(gettext("Distance Fighting"));
    lblSkillLeft[5].SetCaption(gettext("Shielding"));
    lblSkillLeft[6].SetCaption(gettext("Fishing"));

    updateSelf();
}


void winSkills_t::updateSelf()
{
    if (!g_engine) // not running under GUI, it seems
        return;

    std::stringstream s("");

    // FIXME (ivucica#4#) insert comma after every three characters
    s << GlobalVariables::getPlayerStat(STAT_EXPERIENCE);
    std::string expstr = s.str();
    size_t pos = expstr.length() % 3;
    if(pos == 0) pos = 3; // to prevent exp from having leading comma (",666")
    while(pos < expstr.length()) {
        expstr.insert(pos, 1, ',');
        pos += 4; // to account for the addition of ','
    }
    lblExperienceRight.SetPos(160 - 12 - g_engine->sizeText(expstr.c_str(), "system"), 5);
    lblExperienceRight.SetWidth(g_engine->sizeText(expstr.c_str(), "system"));
    lblExperienceRight.SetCaption(expstr);

    s.str("");
    s << GlobalVariables::getPlayerSkill(SKILL_LEVEL, SKILL_ATTR_LEVEL);
    lblLevelRight.SetPos(160 - 12 - g_engine->sizeText(s.str().c_str(), "system"), 15);
    lblLevelRight.SetWidth(g_engine->sizeText(s.str().c_str(), "system"));
    lblLevelRight.SetCaption(s.str());

    pbExperience.SetValue(GlobalVariables::getPlayerSkill(SKILL_LEVEL, SKILL_ATTR_PERCENT));
    ///////////////////


    s.str("");
    s << GlobalVariables::getPlayerStat(STAT_HEALTH_MAX); // TODO (ivucica#1#) check if this should be CURRENT or MAX hp
    lblHPRight.SetPos(160 - 12 - g_engine->sizeText(s.str().c_str(), "system"), 35);
    lblHPRight.SetWidth(g_engine->sizeText(s.str().c_str(), "system"));
    lblHPRight.SetCaption(s.str());

    s.str("");
    s << GlobalVariables::getPlayerStat(STAT_MANA_MAX); // TODO (ivucica#1#) check if this should be CURRENT or MAX mp
    lblMPRight.SetPos(160 - 12 - g_engine->sizeText(s.str().c_str(), "system"), 45);
    lblMPRight.SetWidth(g_engine->sizeText(s.str().c_str(), "system"));
    lblMPRight.SetCaption(s.str());

    s.str("");
    s << GlobalVariables::getPlayerStat(STAT_SOUL);
    lblSPRight.SetPos(160 - 12 - g_engine->sizeText(s.str().c_str(), "system"), 55);
    lblSPRight.SetWidth(g_engine->sizeText(s.str().c_str(), "system"));
    lblSPRight.SetCaption(s.str());

    s.str("");
    s << GlobalVariables::getPlayerStat(STAT_CAPACITY);
    lblCapRight.SetPos(160 - 12 - g_engine->sizeText(s.str().c_str(), "system"), 65);
    lblCapRight.SetWidth(g_engine->sizeText(s.str().c_str(), "system"));
    lblCapRight.SetCaption(s.str());

    s.str("");
    s << int(GlobalVariables::getPlayerStat(STAT_STAMINA) / 60) << ":" <<  std::setw(2) << std::setfill('0') << GlobalVariables::getPlayerStat(STAT_STAMINA) % 60;
    lblStaRight.SetPos(160 - 12 - g_engine->sizeText(s.str().c_str(), "system"), 75);
    lblStaRight.SetWidth(g_engine->sizeText(s.str().c_str(), "system"));
    lblStaRight.SetCaption(s.str());

    pbStamina.SetValue((GlobalVariables::getPlayerStat(STAT_STAMINA) / (56*60.)) * 100.); /* 56 hours 60 minutes is max stamina so far */

    s.str("");
    s << GlobalVariables::getPlayerSkill(SKILL_MAGIC, SKILL_ATTR_LEVEL);
    lblMagicLevelRight.SetPos(160 - 12 - g_engine->sizeText(s.str().c_str(), "system"), 95);
    lblMagicLevelRight.SetWidth(g_engine->sizeText(s.str().c_str(), "system"));
    lblMagicLevelRight.SetCaption(s.str());

    pbMagicLevel.SetValue(GlobalVariables::getPlayerSkill(SKILL_MAGIC, SKILL_ATTR_PERCENT));


    for (int i = 0; i < 7; i++) {
        s.str("");
        s << GlobalVariables::getPlayerSkill((SkillList_t)(SKILL_FIST + i), SKILL_ATTR_LEVEL);
        lblSkillRight[i].SetPos(160 - 12 - g_engine->sizeText(s.str().c_str(), "system"), 125 + i*20);
        lblSkillRight[i].SetWidth(g_engine->sizeText(s.str().c_str(), "system"));
        lblSkillRight[i].SetCaption(s.str());

        pbSkill[i].SetValue(GlobalVariables::getPlayerSkill((SkillList_t)(SKILL_FIST + i), SKILL_ATTR_PERCENT));
    }


}

void winSkills_t::OnClose()
{
	controller->closeSkills();
}
