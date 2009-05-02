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

#ifndef __UI_OPTIONSCONSOLE_H
#define __UI_OPTIONSCONSOLE_H

#if defined(HAVE_LIBINTL_H)
    #include <libintl.h>
#else
    #define gettext(x) (x)
#endif

#include <GLICT/window.h>
#include <GLICT/button.h>
#include "../skin.h"
#include "../options.h"
#include "checkbox.h"

class winOptionsConsole_t {
public:


	glictWindow window;

	// 14 15, 261 22
	uiCheckbox chkInfoMsgs;

	// 14 50, 261 22
	uiCheckbox chkEventMsgs;

	// 14 85
	uiCheckbox chkStatusMsgs;

	// 14 120
	uiCheckbox chkTimestamps;

	// 14 155
	uiCheckbox chkLevels;

    // 14 190
    uiCheckbox chkPrivateMsgs;


	// 9 232, 270 2
	glictPanel pnlSeparator;


	glictButton btnHelp;// 131 244, 40 17
	glictButton btnOk; // 185 244
	glictButton btnCancel; // 237 244
	winOptionsConsole_t () {

		window.SetVisible(false);
		window.SetHeight(276);
		window.SetWidth(296);
		window.SetCaption(gettext("Console Options"));
		window.SetBGColor(.4, .4, .4, 1.);



        window.AddObject(&chkInfoMsgs.pnlPanel);
		chkInfoMsgs.SetPos(14,16);
		chkInfoMsgs.SetWidth(261);
		chkInfoMsgs.SetHeight(22);
		chkInfoMsgs.SetCaption(gettext("Show Info Messages in Console"));

        window.AddObject(&chkEventMsgs.pnlPanel);
		chkEventMsgs.SetPos(14,50);
		chkEventMsgs.SetWidth(261);
		chkEventMsgs.SetHeight(22);
		chkEventMsgs.SetCaption(gettext("Show Event Messages in Console"));

        window.AddObject(&chkStatusMsgs.pnlPanel);
		chkStatusMsgs.SetPos(14,85);
		chkStatusMsgs.SetWidth(261);
		chkStatusMsgs.SetHeight(22);
		chkStatusMsgs.SetCaption(gettext("Show Status Messages in Console"));

        window.AddObject(&chkTimestamps.pnlPanel);
		chkTimestamps.SetPos(14,120);
		chkTimestamps.SetWidth(261);
		chkTimestamps.SetHeight(22);
		chkTimestamps.SetCaption(gettext("Show Timestamps in Console"));

        window.AddObject(&chkLevels.pnlPanel);
		chkLevels.SetPos(14,155);
		chkLevels.SetWidth(261);
		chkLevels.SetHeight(22);
		chkLevels.SetCaption(gettext("Show Levels in Console"));

        window.AddObject(&chkPrivateMsgs.pnlPanel);
		chkPrivateMsgs.SetPos(14,190);
		chkPrivateMsgs.SetWidth(261);
		chkPrivateMsgs.SetHeight(22);
		chkPrivateMsgs.SetCaption(gettext("Show Private Messages in Game Window"));

		window.AddObject(&pnlSeparator);
		pnlSeparator.SetPos(9, 232);
		pnlSeparator.SetWidth(270);
		pnlSeparator.SetHeight(2);
		pnlSeparator.SetBGColor(.2,.2,.2,1.);
		pnlSeparator.SetSkin(&g_skin.chk);

		window.AddObject(&btnHelp);
		btnHelp.SetPos(131, 244);
		btnHelp.SetWidth(41);
		btnHelp.SetHeight(17);
		btnHelp.SetCaption(gettext("Help"));
		btnHelp.SetFont("minifont",8);

		window.AddObject(&btnOk);
		btnOk.SetPos(185, 244);
		btnOk.SetWidth(41);
		btnOk.SetHeight(17);
		btnOk.SetCaption(gettext("Ok"));
		btnOk.SetFont("minifont",8);

		window.AddObject(&btnCancel);
		btnCancel.SetPos(237, 244);
		btnCancel.SetWidth(41);
		btnCancel.SetHeight(17);
		btnCancel.SetCaption(gettext("Cancel"));
		btnCancel.SetFont("minifont",8);
	}


	void Init() {
	    chkInfoMsgs.SetValue(options.infomsgs);
	    chkEventMsgs.SetValue(options.eventmsgs);
	    chkStatusMsgs.SetValue(options.statusmsgs);
	    chkTimestamps.SetValue(options.timestamps);
	    chkLevels.SetValue(options.levels);
		chkPrivateMsgs.SetValue(options.privatemsgs);
	}

	void Store() {
		options.infomsgs = chkInfoMsgs.GetValue();
		options.eventmsgs = chkEventMsgs.GetValue();
		options.statusmsgs = chkStatusMsgs.GetValue();
		options.timestamps = chkTimestamps.GetValue();
		options.levels = chkLevels.GetValue();
		options.privatemsgs = chkPrivateMsgs.GetValue();
		options.Save();
	}


};


#endif

