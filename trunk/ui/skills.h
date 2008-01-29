#ifndef __SKILLS_H
#define __SKILLS_H

#include <GLICT/window.h>
#include <GLICT/progressbar.h>
#include <sstream>
#include "../engine.h"
#include "../gamecontent/globalvars.h"
class winSkills_t {
public:
	// FIXME (ivucica#4#) This window is not a 1:1 match, and among other things, we need to modify GLICT's window to make it such (no icon support, no left alignment support, no minimize and close support ...)
	glictWindow window;
	glictPanel container;

	glictPanel pnlExperienceLeft, pnlExperienceRight;
	glictPanel pnlLevelLeft, pnlLevelRight;
	glictProgressBar pbExperience;
	winSkills_t() {
		window.SetWidth(150);
		window.SetHeight(150);
		window.SetCaption("Skills");
		window.AddObject(&container);
		container.SetWidth(150);
		container.SetHeight(150);
		container.SetVirtualSize(150, 1000);
		container.SetBGActiveness(false);


		container.AddObject(&pnlExperienceLeft);
		container.AddObject(&pnlExperienceRight);
		pnlExperienceLeft.SetPos(5, 5);
		pnlExperienceLeft.SetWidth(150);
		pnlExperienceLeft.SetHeight(12);
		pnlExperienceLeft.SetBGActiveness(false);
		pnlExperienceLeft.SetCaption("Experience");

		pnlExperienceRight.SetPos(150-20, 5);
		pnlExperienceRight.SetWidth(10);
		pnlExperienceRight.SetHeight(12);
		pnlExperienceRight.SetBGActiveness(false);
		pnlExperienceRight.SetCaption("0");


		container.AddObject(&pnlLevelLeft);
		container.AddObject(&pnlLevelRight);
		pnlLevelLeft.SetPos(5, 15);
		pnlLevelLeft.SetWidth(150);
		pnlLevelLeft.SetHeight(12);
		pnlLevelLeft.SetBGActiveness(false);
		pnlLevelLeft.SetCaption("Level");

		pnlLevelRight.SetPos(150 - 20, 15);
		pnlLevelRight.SetWidth(10);
		pnlLevelRight.SetHeight(12);
		pnlLevelRight.SetBGActiveness(false);
		pnlLevelRight.SetCaption("0");


		#if (GLICT_APIREV < 63)
		#warning You should upgrade to GLICT apirev 63 or greater to have progressbar in skills window work correctly.
		#endif
		container.AddObject(&pbExperience);

		pbExperience.SetPos(5, 25);
		pbExperience.SetWidth(130);
		pbExperience.SetHeight(5);
		pbExperience.SetValue(40);


		updateSelf();
	}

	void updateSelf() {
		std::stringstream s("");

		// FIXME (ivucica#4#) insert comma after every three characters
		s << GlobalVariables::getPlayerStat(STAT_EXPERIENCE);
		pnlExperienceRight.SetPos(150 - 12 - g_engine->sizeText(s.str().c_str(), "system"), 5);
		pnlExperienceRight.SetCaption(s.str());

		s.str("");
		s << GlobalVariables::getPlayerSkill(SKILL_LEVEL, SKILL_ATTR_LEVEL);
		pnlLevelRight.SetPos(150 - 12 - g_engine->sizeText(s.str().c_str(), "system"), 5);
		pnlLevelRight.SetCaption(s.str());

		pbExperience.SetValue(GlobalVariables::getPlayerSkill(SKILL_LEVEL, SKILL_ATTR_PERCENT));
	}

};
#endif
