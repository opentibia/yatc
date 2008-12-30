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

#ifndef __UI_OPTIONSGENERAL_H
#define __UI_OPTIONSGENERAL_H

#include "../skin.h"
#include "checkbox.h"
#if defined(HAVE_LIBINTL_H)
    #include <libintl.h>
#else
    #define gettext(x) (x)
#endif
class winOptionsGeneral_t {
public:


	glictWindow window;

	// 16 16, 198 20
	uiCheckbox chkClassicControl;

	// 16 49, 198 20
	uiCheckbox chkAutoChase;

	// 16 82
	uiCheckbox chkShowHints;

	// 16 115
	uiCheckbox chkShowNames;

	// 16 148
	uiCheckbox chkShowTextEffects;

	// 10 189, 210 2
	glictPanel pnlSeparator;


	glictButton btnHelp;// 72 200, 40 17
	glictButton btnOk; // 125 200
	glictButton btnCancel; // 178 200
	winOptionsGeneral_t () {

		window.SetVisible(false);
		window.SetHeight(236);
		window.SetWidth(228);
		window.SetCaption(gettext("General Options"));
		window.SetBGColor(.4, .4, .4, 1.);

		window.AddObject(&chkClassicControl.pnlPanel);
		chkClassicControl.SetPos(16,16);
		chkClassicControl.SetWidth(198);
		chkClassicControl.SetHeight(20);
		chkClassicControl.SetCaption(gettext("Classic Control"));


		window.AddObject(&chkAutoChase.pnlPanel);
		chkAutoChase.SetPos(16,49);
		chkAutoChase.SetWidth(198);
		chkAutoChase.SetHeight(20);
		chkAutoChase.SetCaption(gettext("Auto Chase Off"));

		window.AddObject(&chkShowHints.pnlPanel);
		chkShowHints.SetPos(16,82);
		chkShowHints.SetWidth(198);
		chkShowHints.SetHeight(20);
		chkShowHints.SetCaption(gettext("Show Hints"));

		window.AddObject(&chkShowNames.pnlPanel);
		chkShowNames.SetPos(16,115);
		chkShowNames.SetWidth(198);
		chkShowNames.SetHeight(20);
		chkShowNames.SetCaption(gettext("Show Names of Creatures"));

		window.AddObject(&chkShowTextEffects.pnlPanel);
		chkShowTextEffects.SetPos(16, 148);
		chkShowTextEffects.SetWidth(198);
		chkShowTextEffects.SetHeight(20);
		chkShowTextEffects.SetCaption(gettext("Show Textual Effects"));

		window.AddObject(&pnlSeparator);
		pnlSeparator.SetPos(10, 189);
		pnlSeparator.SetWidth(210);
		pnlSeparator.SetHeight(2);
		pnlSeparator.SetBGColor(.2,.2,.2,1.);
		pnlSeparator.SetSkin(&g_skin.chk);

		window.AddObject(&btnHelp);
		btnHelp.SetPos(72, 200);
		btnHelp.SetWidth(41);
		btnHelp.SetHeight(17);
		btnHelp.SetCaption(gettext("Help"));
		btnHelp.SetFont("minifont",8);

		window.AddObject(&btnOk);
		btnOk.SetPos(125, 200);
		btnOk.SetWidth(41);
		btnOk.SetHeight(17);
		btnOk.SetCaption(gettext("Ok"));
		btnOk.SetFont("minifont",8);

		window.AddObject(&btnCancel);
		btnCancel.SetPos(178, 200);
		btnCancel.SetWidth(41);
		btnCancel.SetHeight(17);
		btnCancel.SetCaption(gettext("Cancel"));
		btnCancel.SetFont("minifont",8);
	}


	void Init() {
		chkAutoChase.SetValue(options.autochase);
		chkShowHints.SetValue(options.showhints);
		chkShowTextEffects.SetValue(options.showtexteffects);
		chkClassicControl.SetValue(options.classiccontrol);
		chkShowNames.SetValue(options.shownames);
	}

	void Store() {
		options.autochase = chkAutoChase.GetValue();
		options.showhints = chkShowHints.GetValue();
        options.showtexteffects = chkShowTextEffects.GetValue();
		options.classiccontrol = chkClassicControl.GetValue();
		options.shownames = chkShowNames.GetValue();
		options.Save();
	}


};


#endif
