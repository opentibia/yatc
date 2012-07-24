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

#ifndef __UI_SBVLPANEL_H
#define __UI_SBVLPANEL_H

#include "skills.h"
#include "vipwindow.h"
#include "battlewindow.h"
#include <GLICT/panel.h>
#include <GLICT/button.h>
#if defined(HAVE_CONFIG_H)
    #include "../config.h"
#endif
#if defined(HAVE_LIBINTL_H)
    #include <libintl.h>
#else
    #define gettext(x) (x)
#endif
class sbvlPanel_t
{
public:
	glictPanel panel; //172x30

	glictButton btnSkills;	//8, 6; 34x20
	glictButton btnBattle;	//45, 6; 34x20
	glictButton btnVIP;		//82, 6; 32x20
	glictButton btnLogout;	//124, 6; 44x20

	winSkills_t winSkills;
	winBattle_t winBattle;
	winVIP_t winVIP;

	sbvlPanel_t();

	void openSkills();
	void closeSkills();
	void openBattle();
	void closeBattle();
	void openVIP();
	void closeVIP();

	static void onClick_Logout(glictPos* relmousepos, glictContainer* callerclass);
	static void btnSkills_onClick(glictPos* relmousepos, glictContainer* callerclass);
	static void btnBattle_onClick(glictPos* relmousepos, glictContainer* callerclass);
	static void btnVIP_onClick(glictPos* relmousepos, glictContainer* callerclass);
};

#endif //__UI_SBVLPANEL_H

