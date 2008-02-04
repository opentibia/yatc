#ifndef __SKILLS_H
#define __SKILLS_H

#include <GLICT/window.h>
#include <GLICT/progressbar.h>
#include <sstream>
#include <iomanip>
#include "../engine.h"
#include "../gamecontent/globalvars.h"
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
	winSkills_t() {
		window.SetWidth(150);
		window.SetHeight(150);
		window.SetCaption("Skills");
		window.AddObject(&container);
		container.SetWidth(150);
		container.SetHeight(150);
		container.SetVirtualSize(150, 1000);
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


		#if (GLICT_APIREV < 63)
		#warning You should upgrade to GLICT apirev 63 or greater to have progressbar in skills window work correctly.
		#endif
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



		#if (GLICT_APIREV < 63)
		#warning You should upgrade to GLICT apirev 63 or greater to have progressbar in skills window work correctly.
		#endif
		container.AddObject(&pbStamina);

		pbStamina.SetPos(5, 85);
		pbStamina.SetWidth(130);
		pbStamina.SetHeight(5);
		pbStamina.SetValue(40);

		updateSelf();
	}

	void updateSelf() {
		std::stringstream s("");

		// FIXME (ivucica#4#) insert comma after every three characters
		s << GlobalVariables::getPlayerStat(STAT_EXPERIENCE);
		lblExperienceRight.SetPos(150 - 12 - g_engine->sizeText(s.str().c_str(), "system"), 5);
		lblExperienceRight.SetCaption(s.str());

		s.str("");
		s << GlobalVariables::getPlayerSkill(SKILL_LEVEL, SKILL_ATTR_LEVEL);
		lblLevelRight.SetPos(150 - 12 - g_engine->sizeText(s.str().c_str(), "system"), 15);
		lblLevelRight.SetCaption(s.str());

		pbExperience.SetValue(GlobalVariables::getPlayerSkill(SKILL_LEVEL, SKILL_ATTR_PERCENT));
		printf("Experience level: %d\n", GlobalVariables::getPlayerSkill(SKILL_LEVEL, SKILL_ATTR_PERCENT));

		///////////////////


		s.str("");
		s << GlobalVariables::getPlayerStat(STAT_HEALTH_MAX); // TODO (ivucica#1#) check if this should be CURRENT or MAX hp
		lblHPRight.SetPos(150 - 12 - g_engine->sizeText(s.str().c_str(), "system"), 35);
		lblHPRight.SetCaption(s.str());

		s.str("");
		s << GlobalVariables::getPlayerStat(STAT_MANA_MAX); // TODO (ivucica#1#) check if this should be CURRENT or MAX mp
		lblMPRight.SetPos(150 - 12 - g_engine->sizeText(s.str().c_str(), "system"), 45);
		lblMPRight.SetCaption(s.str());

		s.str("");
		s << GlobalVariables::getPlayerStat(STAT_SOUL);
		lblSPRight.SetPos(150 - 12 - g_engine->sizeText(s.str().c_str(), "system"), 55);
		lblSPRight.SetCaption(s.str());

		s.str("");
		s << GlobalVariables::getPlayerStat(STAT_CAPACITY);
		lblCapRight.SetPos(150 - 12 - g_engine->sizeText(s.str().c_str(), "system"), 65);
		lblCapRight.SetCaption(s.str());

		s.str("");
		s << int(GlobalVariables::getPlayerStat(STAT_STAMINA) / 60) << ":" <<  std::setw(2) << std::setfill('0') << GlobalVariables::getPlayerStat(STAT_STAMINA) % 60;
		lblStaRight.SetPos(150 - 12 - g_engine->sizeText(s.str().c_str(), "system"), 75);
		lblStaRight.SetCaption(s.str());

		pbExperience.SetValue(GlobalVariables::getPlayerStat(STAT_STAMINA) / 56*60); /* 56 hours 60 minutes is max stamina so far */

	}

};
#endif
