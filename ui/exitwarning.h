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

#ifndef __UI_EXITWARNING_H
#define __UI_EXITWARNING_H

#include <GLICT/window.h>
#include <GLICT/panel.h>
#include <GLICT/button.h>

class exitWarning_t
{
public:
	glictWindow window;

	glictPanel lblText;

	glictPanel pnlSep;

	glictButton btnLogout;
	glictButton btnExit;
	glictButton btnCancel;

	exitWarning_t()
	{
		window.SetCaption("Warning");
		window.SetWidth(520);
		window.SetHeight(100);

		window.AddObject(&lblText);
		lblText.SetPos(13, 10);
		lblText.SetWidth(500);
		lblText.SetHeight(44);
		lblText.SetBGActiveness(false);
		lblText.SetCaption("If you shut down the program, your character might stay in the game.\n"
			"Click on \"Logout\" to ensure that your character leaves the game properly.\n"
			"Click on \"Exit\" if you want to exit the program without logging out your character.");
		lblText.SetFont("aafont");

		window.AddObject(&pnlSep);
		pnlSep.SetPos(9, 55);
		pnlSep.SetWidth(500);
		pnlSep.SetHeight(2);
		pnlSep.SetSkin(&g_skin.chk);

		window.AddObject(&btnLogout);
		btnLogout.SetCaption("Logout");
		btnLogout.SetFont("minifont");
		btnLogout.SetPos(405, 70);
		btnLogout.SetWidth(44);
		btnLogout.SetHeight(20);
		btnLogout.SetOnClick(&btnLogout_onClick);
		btnLogout.SetCustomData(this);

		window.AddObject(&btnExit);
		btnExit.SetCaption("Exit");
		btnExit.SetFont("minifont");
		btnExit.SetPos(350, 70);
		btnExit.SetWidth(44);
		btnExit.SetHeight(20);
		btnExit.SetOnClick(&btnExit_onClick);

		window.AddObject(&btnCancel);
		btnCancel.SetCaption("Cancel");
		btnCancel.SetFont("minifont");
		btnCancel.SetPos(460, 70);
		btnCancel.SetWidth(44);
		btnCancel.SetHeight(20);
		btnCancel.SetOnClick(&btnCancel_onClick);
		btnCancel.SetCustomData(this);
	}

	~exitWarning_t(){}

	static void btnLogout_onClick(glictPos* relmousepos, glictContainer* callerclass);
	static void btnExit_onClick(glictPos* relmousepos, glictContainer* callerclass);
	static void btnCancel_onClick(glictPos* relmousepos, glictContainer* callerclass)
	{
		exitWarning_t* exitwarn = ((exitWarning_t*)callerclass->GetCustomData());
		exitwarn->window.SetVisible(false);
	}
};

#endif
