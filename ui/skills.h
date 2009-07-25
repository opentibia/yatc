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

#if defined(HAVE_CONFIG_H)
    #include "../config.h"
#endif
#if defined(HAVE_LIBINTL_H)
    #include <libintl.h>
#else
    #define gettext(x) (x)
#endif

#include "stackpanel.h"

class sbvlPanel_t;

class winSkills_t : public yatcStackPanelWindow {
public:
	// FIXME (ivucica#4#) This window is not a 1:1 match, and among other things, we need to modify GLICT's window to make it such (no icon support, no left alignment support)
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

	sbvlPanel_t* controller;

    // functions:
	winSkills_t();
	void updateSelf();
	virtual void OnClose();
	virtual float GetDefaultHeight(){ return 270.F; }

};
#endif
