#ifndef __UI_OPTIONS_H
#define __UI_OPTIONS_H

#include "optionsconsole.h"
#include "optionsgeneral.h"
#include "optionsgfxadv.h"
#include "optionsgraphics.h"
#include "optionshotkeys.h"
#include "optionsnetwork.h"

class winOptions_t {
public:

	glictWindow window;

	glictButton btnGeneral; // 15, 16 -- 84 18
	glictButton btnGraphics; // 15, 49
	glictButton btnConsole; // 15, 82
	glictButton btnHotkeys; // 15 115
	glictButton btnNetwork;

	glictPanel pnlGeneral; //112, 14; 142 27
	glictPanel pnlGraphics; // 112 47
	glictPanel pnlConsole; // 112 80
	glictPanel pnlHotkeys; // 112 113
	glictPanel pnlNetwork;

	glictPanel pnlSeparator1; // 13, 147 -- 250 2

	glictButton btnMotd; // 15 165
	glictPanel pnlMotd; // 112 163

	glictPanel pnlSeparator2; // 9 204 -- 260 2

	glictButton btnOk; // 227 215 -- 41 17

	// (nfries88) Moving as much of the options ui stuff into here as possible
	// is this ok? [
	winOptionsGeneral_t winOptionsGeneral;
	winOptionsConsole_t winOptionsConsole;
	winOptionsGraphics_t winOptionsGraphics;
	winOptionsGraphicsAdvanced_t winOptionsGraphicsAdvanced;
  winOptionsHotkeys_t winOptionsHotkeys;
	winOptionsNetwork_t winOptionsNetwork;
	//]

	winOptions_t () {

		window.SetVisible(false);
		window.SetHeight(271);
		window.SetWidth(278);
		window.SetCaption("Options");
		window.SetBGColor(.4, .4, .4, 1.);

		window.AddObject(&btnGeneral);
		btnGeneral.SetPos(15, 16);
		btnGeneral.SetWidth(84);
		btnGeneral.SetHeight(18);
		btnGeneral.SetCaption("General");
		btnGeneral.SetFont("minifont",8);

		window.AddObject(&btnGraphics);
		btnGraphics.SetPos(15, 49);
		btnGraphics.SetWidth(84);
		btnGraphics.SetHeight(18);
		btnGraphics.SetCaption("Graphics");
		btnGraphics.SetFont("minifont",8);

		window.AddObject(&btnConsole);
		btnConsole.SetPos(15, 82);
		btnConsole.SetWidth(84);
		btnConsole.SetHeight(18);
		btnConsole.SetCaption("Console");
		btnConsole.SetFont("minifont",8);

		window.AddObject(&btnHotkeys);
		btnHotkeys.SetPos(15, 115);
		btnHotkeys.SetWidth(84);
		btnHotkeys.SetHeight(18);
		btnHotkeys.SetCaption("Hotkeys");
		btnHotkeys.SetFont("minifont",8);

		window.AddObject(&btnNetwork);
		btnNetwork.SetPos(15, 148);
		btnNetwork.SetWidth(84);
		btnNetwork.SetHeight(18);
		btnNetwork.SetCaption("Network");
		btnNetwork.SetFont("minifont",8);

		window.AddObject(&pnlGeneral);
		pnlGeneral.SetPos(112, 14);
		pnlGeneral.SetWidth(142);
		pnlGeneral.SetHeight(27);
		pnlGeneral.SetCaption("Change general\ngame options");
		pnlGeneral.SetBGActiveness(false);
		pnlGeneral.SetFont("aafont",10);

		window.AddObject(&pnlGraphics);
		pnlGraphics.SetPos(112, 47);
		pnlGraphics.SetWidth(142);
		pnlGraphics.SetHeight(27);
		pnlGraphics.SetCaption("Change graphics and\nperformance settings");
		pnlGraphics.SetBGActiveness(false);
		pnlGraphics.SetFont("aafont");

		window.AddObject(&pnlConsole);
		pnlConsole.SetPos(112, 80);
		pnlConsole.SetWidth(142);
		pnlConsole.SetHeight(27);
		pnlConsole.SetCaption("Customise the console");
		pnlConsole.SetBGActiveness(false);
		pnlConsole.SetFont("aafont");

		window.AddObject(&pnlHotkeys);
		pnlHotkeys.SetPos(112, 113);
		pnlHotkeys.SetWidth(142);
		pnlHotkeys.SetHeight(27);
		pnlHotkeys.SetCaption("Edit your hotkey texts");
		pnlHotkeys.SetBGActiveness(false);
		pnlHotkeys.SetFont("aafont");

		window.AddObject(&pnlNetwork);
		pnlNetwork.SetPos(112, 146);
		pnlNetwork.SetWidth(142);
		pnlNetwork.SetHeight(27);
		pnlNetwork.SetCaption("Configure network options,\nsuch as server address.");
		pnlNetwork.SetBGActiveness(false);
		pnlNetwork.SetFont("aafont");

		window.AddObject(&pnlSeparator1);
		pnlSeparator1.SetPos(13, 176);
		pnlSeparator1.SetWidth(250);
		pnlSeparator1.SetHeight(2);
		pnlSeparator1.SetBGColor(.2,.2,.2,1.);

		window.AddObject(&btnMotd);
		btnMotd.SetPos(15, 198);
		btnMotd.SetWidth(84);
		btnMotd.SetHeight(18);
		btnMotd.SetCaption("Motd");
		btnMotd.SetFont("minifont",8);

		window.AddObject(&pnlMotd);
		pnlMotd.SetPos(112, 196);
		pnlMotd.SetWidth(142);
		pnlMotd.SetHeight(27);
		pnlMotd.SetCaption("Show the most recent\nMessage of the Day");
		pnlMotd.SetBGActiveness(false);
		pnlMotd.SetFont("aafont");

		window.AddObject(&pnlSeparator2);
		pnlSeparator2.SetPos(9, 237);
		pnlSeparator2.SetWidth(260);
		pnlSeparator2.SetHeight(2);
		pnlSeparator2.SetBGColor(.2,.2,.2,1.);


		window.AddObject(&btnOk);
		btnOk.SetPos(227, 248);
		btnOk.SetWidth(41);
		btnOk.SetHeight(17);
		btnOk.SetCaption("Ok");
		btnOk.SetFont("minifont",8);
	}

	void initiateAll(glictContainer* desktop);

    static void btnHelp_OnClick(glictPos* relmousepos, glictContainer* callerclass);

	static void winOptions_btnGeneral_OnClick(glictPos* relmousepos, glictContainer* callerclass);
	static void winOptions_btnConsole_OnClick(glictPos* relmousepos, glictContainer* callerclass);
	static void winOptions_btnGraphics_OnClick(glictPos* relmousepos, glictContainer* callerclass);
	static void winOptions_btnHotkeys_OnClick(glictPos* relmousepos, glictContainer* callerclass);
	static void winOptions_btnNetwork_OnClick(glictPos* relmousepos, glictContainer* callerclass);
	static void winOptions_btnMotd_OnClick(glictPos* relmousepos, glictContainer* callerclass);
	static void winOptions_btnOk_OnClick(glictPos* relmousepos, glictContainer* callerclass);

	static void winOptionsGeneral_btnOk_OnClick(glictPos* relmousepos, glictContainer* callerclass);
	static void winOptionsGeneral_btnCancel_OnClick(glictPos* relmousepos, glictContainer* callerclass);

	static void winOptionsConsole_btnOk_OnClick(glictPos* relmousepos, glictContainer* callerclass);
	static void winOptionsConsole_btnCancel_OnClick(glictPos* relmousepos, glictContainer* callerclass);

	static void winOptionsGraphics_btnOk_OnClick(glictPos* relmousepos, glictContainer* callerclass);
	static void winOptionsGraphics_btnCancel_OnClick(glictPos* relmousepos, glictContainer* callerclass);
	static void winOptionsGraphics_btnAdvanced_OnClick(glictPos* relmousepos, glictContainer* callerclass);

	static void winOptionsGraphicsAdvanced_btnOk_OnClick(glictPos* relmousepos, glictContainer* callerclass);
	static void winOptionsGraphicsAdvanced_btnCancel_OnClick(glictPos* relmousepos, glictContainer* callerclass);

	static void winOptionsHotkeys_btnOk_OnClick(glictPos* relmousepos, glictContainer* callerclass);
	static void winOptionsHotkeys_btnCancel_OnClick(glictPos* relmousepos, glictContainer* callerclass);

	static void winOptionsNetwork_btnOk_OnClick(glictPos* relmousepos, glictContainer* callerclass);
	static void winOptionsNetwork_btnCancel_OnClick(glictPos* relmousepos, glictContainer* callerclass);

};


#endif
