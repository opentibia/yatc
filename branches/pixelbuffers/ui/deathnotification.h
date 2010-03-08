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

#ifndef __UI_DEATHNOTIFICATION_H
#define __UI_DEATHNOTIFICATION_H

#include <GLICT/window.h>
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
#include "../util.h"
#include "../product.h"
class deathNotice_t
{
public:
	glictWindow window;

	glictPanel lblText;

	glictPanel pnlSep;

	glictButton btnOk;
	glictButton btnCancel;

	deathNotice_t()
	{
		window.SetCaption(gettext("You are dead"));
		window.SetWidth(340);
		window.SetHeight(140);

		window.AddObject(&lblText);
		lblText.SetPos(13, 10);
		lblText.SetWidth(315);
		lblText.SetHeight(70);
		lblText.SetBGActiveness(false);
		lblText.SetCaption(
            str_replace("$$PRODUCTSHORT$$", PRODUCTSHORT,
                gettext(    "OMG! Some creep just killed ya, you noob.\n"
                            "But don't be sad, you crying baby. You can just respawn if\n"
                            "you give up some of your stuff. \n"
                            "\n"
                            "Go click that 'Ok' button if you dare and resume your journeys in $$PRODUCTSHORT$$!"
                )
            )
        );
        lblText.SetFont("aafont");

		window.AddObject(&pnlSep);
		pnlSep.SetPos(9, 100);
		pnlSep.SetWidth(330);
		pnlSep.SetHeight(2);
		pnlSep.SetSkin(&g_skin.chk);

		window.AddObject(&btnOk);
		btnOk.SetCaption(gettext("Ok"));
		btnOk.SetFont("minifont");
		btnOk.SetPos(225, 110);
		btnOk.SetWidth(44);
		btnOk.SetHeight(20);
		btnOk.SetOnClick(&btnOk_onClick);
		btnOk.SetCustomData(this);

		window.AddObject(&btnCancel);
		btnCancel.SetCaption(gettext("Cancel"));
		btnCancel.SetFont("minifont");
		btnCancel.SetPos(280, 110);
		btnCancel.SetWidth(44);
		btnCancel.SetHeight(20);
		btnCancel.SetOnClick(&btnCancel_onClick);
		btnCancel.SetCustomData(this);
	}

	~deathNotice_t(){}

	static void btnOk_onClick(glictPos* relmousepos, glictContainer* callerclass);
	static void btnCancel_onClick(glictPos* relmousepos, glictContainer* callerclass)
	{
		deathNotice_t* deathnote = ((deathNotice_t*)callerclass->GetCustomData());
		deathnote->window.SetVisible(false);
	}
};

#endif
