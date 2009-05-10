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
//#include "vip.h"
//#include "battlewindow.h"
#include <GLICT/panel.h>
#include <GLICT/button.h>

class sbvlPanel_t
{
public:
	glictPanel panel; //172x30

	glictButton btnSkills;	//8, 6; 34x20
	glictButton btnBattle;	//45, 6; 34x20
	glictButton btnVIP;		//82, 6; 32x20
	glictButton btnLogout;	//124, 6; 44x20

	winSkills_t winSkills;
	//winBattle_t winBattle;
	//winVIP_t winVIP;

	sbvlPanel_t()
	{
		panel.SetWidth(172);
		panel.SetHeight(30);
		#if (GLICT_APIREV >= 98)
		panel.SetDraggable(true);
		panel.SetSkin(&g_skin.background);
		panel.SetFocusable(true);
		#else
		#warning svblPanel_t is not properly operated until you upgrade to GLICT 98+
		#endif

		panel.AddObject(&btnSkills);
		btnSkills.SetCaption("Skills");
		btnSkills.SetCustomData(this);
		btnSkills.SetWidth(35);
		btnSkills.SetHeight(20);
		btnSkills.SetPos(8, 6);
		btnSkills.SetOnClick(btnSkills_onClick);
		btnSkills.SetFont("minifont");

		/*
		panel.AddObject(&btnBattle);
		btnBattle.SetCaption("Battle");
		btnBattle.SetCustomData(this);
		btnBattle.SetWidth(35);
		btnBattle.SetHeight(20);
		btnBattle.SetPos(45, 6);
		btnBattle.SetOnClick(btnBattle_onClick);
		btnBattle.SetFont("minifont");

		panel.AddObject(&btnVIP);
		btnVIP.SetCaption("VIP");
		btnVIP.SetCustomData(this);
		btnVIP.SetWidth(35);
		btnVIP.SetHeight(20);
		btnVIP.SetPos(82, 6);
		btnVIP.SetOnClick(btnVIP_onClick);
		btnVIP.SetFont("minifont");
		*/


		panel.AddObject(&btnLogout);
		btnLogout.SetCaption("Logout");
		btnLogout.SetCustomData(this);
		btnLogout.SetWidth(45);
		btnLogout.SetHeight(20);
		btnLogout.SetPos(124, 6);
		btnLogout.SetOnClick(onClick_Logout);
		btnLogout.SetFont("minifont");

		winSkills.window.SetVisible(false);
		winSkills.controller = this;
		//winBattle.window.SetVisible(false);
		//winBattle.controller = this;
		//winVIP.window.SetVisible(false);
		//winVIP.controller = this;
	}

	void openSkills();
	void closeSkills();
	/*
	void openBattle();
	void closeBattle();
	void openVIP();
	void closeVIP();
	*/

	static void onClick_Logout(glictPos* relmousepos, glictContainer* callerclass);
	static void btnSkills_onClick(glictPos* relmousepos, glictContainer* callerclass);
	/*
	static void btnBattle_onClick(glictPos* relmousepos, glictContainer* callerclass);
	static void btnVIP_onClick(glictPos* relmousepos, glictContainer* callerclass);
	*/
};

#endif //__UI_SBVLPANEL_H

