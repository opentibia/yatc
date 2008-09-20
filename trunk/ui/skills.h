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

#ifndef __SKILLS_H
#define __SKILLS_H

#include <GLICT/window.h>
#include <GLICT/progressbar.h>
#include <sstream>
#include <iomanip>
#include "../engine.h"
#include "../gamecontent/globalvars.h"
#include "../util.h"
class winSkills_t {
public:
	// FIXME (ivucica#4#) This window is not a 1:1 match, and among other things, we need to modify GLICT's window to make it such (no icon support, no left alignment support, no minimize and close support ...)
	glictWindow window;
	glictPanel container;

	glictPanel lblExperienceLeft, lblExperienceRight;
	glictPanel lblLevelLeft, lblLevelRight;
	glictProgressBar pbExperience;
	////
	glictPanel lblHPLeft, lblHPRight;
	glictPanel lblMPLeft, lblMPRight;
	glictPanel lblSPLeft, lblSPRight;
	glictPanel lblCapLeft, lblCapRight;
	glictPanel lblStaLeft, lblStaRight;
	glictProgressBar pbStamina;
	glictPanel lblMagicLevelLeft, lblMagicLevelRight;
	glictProgressBar pbMagicLevel;

	glictPanel pnlSeparator;

	glictPanel lblSkillLeft[7], lblSkillRight[7];
	glictProgressBar pbSkill[7];

	winSkills_t() {
		window.SetWidth(150);
		window.SetHeight(270);
		window.SetCaption("Skills");
		window.AddObject(&container);
		container.SetWidth(150);
		container.SetHeight(270);
		container.SetVirtualSize(150, 270);
		container.SetBGActiveness(false);


		container.AddObject(&lblExperienceLeft);
		container.AddObject(&lblExperienceRight);
		lblExperienceLeft.SetPos(5, 5);
		lblExperienceLeft.SetWidth(150);
		lblExperienceLeft.SetHeight(12);
		lblExperienceLeft.SetBGActiveness(false);
		lblExperienceLeft.SetCaption("Experience");

		lblExperienceRight.SetPos(150-20, 5);
		lblExperienceRight.SetWidth(10);
		lblExperienceRight.SetHeight(12);
		lblExperienceRight.SetBGActiveness(false);
		lblExperienceRight.SetCaption("0");


		container.AddObject(&lblLevelLeft);
		container.AddObject(&lblLevelRight);
		lblLevelLeft.SetPos(5, 15);
		lblLevelLeft.SetWidth(150);
		lblLevelLeft.SetHeight(12);
		lblLevelLeft.SetBGActiveness(false);
		lblLevelLeft.SetCaption("Level");

		lblLevelRight.SetPos(150 - 20, 15);
		lblLevelRight.SetWidth(10);
		lblLevelRight.SetHeight(12);
		lblLevelRight.SetBGActiveness(false);
		lblLevelRight.SetCaption("0");


		container.AddObject(&pbExperience);

		pbExperience.SetPos(5, 25);
		pbExperience.SetWidth(130);
		pbExperience.SetHeight(5);
		pbExperience.SetValue(40);



		////////////////////////////////

		container.AddObject(&lblHPLeft);
		container.AddObject(&lblHPRight);
		lblHPLeft.SetPos(5, 35);
		lblHPLeft.SetWidth(150);
		lblHPLeft.SetHeight(12);
		lblHPLeft.SetBGActiveness(false);
		lblHPLeft.SetCaption("Hit Points");

		lblHPRight.SetPos(150 - 20, 35);
		lblHPRight.SetWidth(10);
		lblHPRight.SetHeight(12);
		lblHPRight.SetBGActiveness(false);
		lblHPRight.SetCaption("0");


		container.AddObject(&lblMPLeft);
		container.AddObject(&lblMPRight);
		lblMPLeft.SetPos(5, 45);
		lblMPLeft.SetWidth(150);
		lblMPLeft.SetHeight(12);
		lblMPLeft.SetBGActiveness(false);
		lblMPLeft.SetCaption("Mana");

		lblMPRight.SetPos(150 - 20, 45);
		lblMPRight.SetWidth(10);
		lblMPRight.SetHeight(12);
		lblMPRight.SetBGActiveness(false);
		lblMPRight.SetCaption("0");


		container.AddObject(&lblSPLeft);
		container.AddObject(&lblSPRight);
		lblSPLeft.SetPos(5, 55);
		lblSPLeft.SetWidth(150);
		lblSPLeft.SetHeight(12);
		lblSPLeft.SetBGActiveness(false);
		lblSPLeft.SetCaption("Soul Points");

		lblSPRight.SetPos(150 - 20, 55);
		lblSPRight.SetWidth(10);
		lblSPRight.SetHeight(12);
		lblSPRight.SetBGActiveness(false);
		lblSPRight.SetCaption("0");


		container.AddObject(&lblCapLeft);
		container.AddObject(&lblCapRight);
		lblCapLeft.SetPos(5, 65);
		lblCapLeft.SetWidth(150);
		lblCapLeft.SetHeight(12);
		lblCapLeft.SetBGActiveness(false);
		lblCapLeft.SetCaption("Cap");

		lblCapRight.SetPos(150 - 20, 65);
		lblCapRight.SetWidth(10);
		lblCapRight.SetHeight(12);
		lblCapRight.SetBGActiveness(false);
		lblCapRight.SetCaption("0");


		container.AddObject(&lblStaLeft);
		container.AddObject(&lblStaRight);
		lblStaLeft.SetPos(5, 75);
		lblStaLeft.SetWidth(150);
		lblStaLeft.SetHeight(12);
		lblStaLeft.SetBGActiveness(false);
		lblStaLeft.SetCaption("Stamina");

		lblStaRight.SetPos(150 - 20, 75);
		lblStaRight.SetWidth(10);
		lblStaRight.SetHeight(12);
		lblStaRight.SetBGActiveness(false);
		lblStaRight.SetCaption("0");


		container.AddObject(&pbStamina);

		pbStamina.SetPos(5, 85);
		pbStamina.SetWidth(130);
		pbStamina.SetHeight(5);
		pbStamina.SetValue(40);


		container.AddObject(&lblMagicLevelLeft);
		container.AddObject(&lblMagicLevelRight);
		lblMagicLevelLeft.SetPos(5, 95);
		lblMagicLevelLeft.SetWidth(150);
		lblMagicLevelLeft.SetHeight(12);
		lblMagicLevelLeft.SetBGActiveness(false);
		lblMagicLevelLeft.SetCaption("Magic Level");

		lblMagicLevelRight.SetPos(150 - 20, 95);
		lblMagicLevelRight.SetWidth(10);
		lblMagicLevelRight.SetHeight(12);
		lblMagicLevelRight.SetBGActiveness(false);
		lblMagicLevelRight.SetCaption("0");


		container.AddObject(&pbMagicLevel);

		pbMagicLevel.SetPos(5, 105);
		pbMagicLevel.SetWidth(130);
		pbMagicLevel.SetHeight(5);
		pbMagicLevel.SetValue(40);



		container.AddObject(&pnlSeparator);
		pnlSeparator.SetPos(5,115);
		pnlSeparator.SetWidth(130);
		pnlSeparator.SetHeight(2);
		pnlSeparator.SetBGColor(.7, .7, .7, 1.);











		for (int i = 0; i < 7; i++) {
			container.AddObject(&lblSkillLeft[i]);
			container.AddObject(&lblSkillRight[i]);
			lblSkillLeft[i].SetPos(5, 125 + i * 20);
			lblSkillLeft[i].SetWidth(150);
			lblSkillLeft[i].SetHeight(12);
			lblSkillLeft[i].SetBGActiveness(false);
			lblSkillLeft[i].SetCaption(std::string("Skill ") + yatc_itoa(i) );

			lblSkillRight[i].SetPos(150 - 20, 125 + i*20);
			lblSkillRight[i].SetWidth(10);
			lblSkillRight[i].SetHeight(12);
			lblSkillRight[i].SetBGActiveness(false);
			lblSkillRight[i].SetCaption("0");


			container.AddObject(&pbSkill[i]);

			pbSkill[i].SetPos(5, 135 + i*20);
			pbSkill[i].SetWidth(130);
			pbSkill[i].SetHeight(5);
			pbSkill[i].SetValue(40);
		}
		lblSkillLeft[0].SetCaption("Fist Fighting");
		lblSkillLeft[1].SetCaption("Club Fighting");
		lblSkillLeft[2].SetCaption("Sword Fighting");
		lblSkillLeft[3].SetCaption("Axe Fighting");
		lblSkillLeft[4].SetCaption("Distance Fighting");
		lblSkillLeft[5].SetCaption("Shielding");
		lblSkillLeft[6].SetCaption("Fishing");

		updateSelf();
	}

	void updateSelf() {
		std::stringstream s("");

		// FIXME (ivucica#4#) insert comma after every three characters
		s << GlobalVariables::getPlayerStat(STAT_EXPERIENCE);
		lblExperienceRight.SetPos(150 - 12 - g_engine->sizeText(s.str().c_str(), "system"), 5);
		lblExperienceRight.SetWidth(g_engine->sizeText(s.str().c_str(), "system"));
		lblExperienceRight.SetCaption(s.str());

		s.str("");
		s << GlobalVariables::getPlayerSkill(SKILL_LEVEL, SKILL_ATTR_LEVEL);
		lblLevelRight.SetPos(150 - 12 - g_engine->sizeText(s.str().c_str(), "system"), 15);
		lblLevelRight.SetWidth(g_engine->sizeText(s.str().c_str(), "system"));
		lblLevelRight.SetCaption(s.str());

		pbExperience.SetValue(GlobalVariables::getPlayerSkill(SKILL_LEVEL, SKILL_ATTR_PERCENT));
		///////////////////


		s.str("");
		s << GlobalVariables::getPlayerStat(STAT_HEALTH_MAX); // TODO (ivucica#1#) check if this should be CURRENT or MAX hp
		lblHPRight.SetPos(150 - 12 - g_engine->sizeText(s.str().c_str(), "system"), 35);
		lblHPRight.SetWidth(g_engine->sizeText(s.str().c_str(), "system"));
		lblHPRight.SetCaption(s.str());

		s.str("");
		s << GlobalVariables::getPlayerStat(STAT_MANA_MAX); // TODO (ivucica#1#) check if this should be CURRENT or MAX mp
		lblMPRight.SetPos(150 - 12 - g_engine->sizeText(s.str().c_str(), "system"), 45);
		lblMPRight.SetWidth(g_engine->sizeText(s.str().c_str(), "system"));
		lblMPRight.SetCaption(s.str());

		s.str("");
		s << GlobalVariables::getPlayerStat(STAT_SOUL);
		lblSPRight.SetPos(150 - 12 - g_engine->sizeText(s.str().c_str(), "system"), 55);
		lblSPRight.SetWidth(g_engine->sizeText(s.str().c_str(), "system"));
		lblSPRight.SetCaption(s.str());

		s.str("");
		s << GlobalVariables::getPlayerStat(STAT_CAPACITY);
		lblCapRight.SetPos(150 - 12 - g_engine->sizeText(s.str().c_str(), "system"), 65);
		lblCapRight.SetWidth(g_engine->sizeText(s.str().c_str(), "system"));
		lblCapRight.SetCaption(s.str());

		s.str("");
		s << int(GlobalVariables::getPlayerStat(STAT_STAMINA) / 60) << ":" <<  std::setw(2) << std::setfill('0') << GlobalVariables::getPlayerStat(STAT_STAMINA) % 60;
		lblStaRight.SetPos(150 - 12 - g_engine->sizeText(s.str().c_str(), "system"), 75);
		lblStaRight.SetWidth(g_engine->sizeText(s.str().c_str(), "system"));
		lblStaRight.SetCaption(s.str());

		pbStamina.SetValue((GlobalVariables::getPlayerStat(STAT_STAMINA) / (56*60.)) * 100.); /* 56 hours 60 minutes is max stamina so far */

		s.str("");
		s << GlobalVariables::getPlayerSkill(SKILL_MAGIC, SKILL_ATTR_LEVEL);
		lblMagicLevelRight.SetPos(150 - 12 - g_engine->sizeText(s.str().c_str(), "system"), 95);
		lblMagicLevelRight.SetWidth(g_engine->sizeText(s.str().c_str(), "system"));
		lblMagicLevelRight.SetCaption(s.str());

		pbMagicLevel.SetValue(GlobalVariables::getPlayerSkill(SKILL_MAGIC, SKILL_ATTR_PERCENT));


		for (int i = 0; i < 7; i++) {
			s.str("");
			s << GlobalVariables::getPlayerSkill((SkillList_t)(SKILL_FIST + i), SKILL_ATTR_LEVEL);
			lblSkillRight[i].SetPos(150 - 12 - g_engine->sizeText(s.str().c_str(), "system"), 125 + i*20);
			lblSkillRight[i].SetWidth(g_engine->sizeText(s.str().c_str(), "system"));
			lblSkillRight[i].SetCaption(s.str());

			pbSkill[i].SetValue(GlobalVariables::getPlayerSkill((SkillList_t)(SKILL_FIST + i), SKILL_ATTR_PERCENT));
		}




	}

};
#endif
