#ifndef __UI_OPTIONS_H
#define __UI_OPTIONS_H

class winOptions_t {
public:

	glictWindow window;

	glictButton btnGeneral; // 15, 16 -- 84 18
	glictButton btnGraphics; // 15, 49
	glictButton btnConsole; // 15, 82
	glictButton btnHotkeys; // 15 115

	glictPanel pnlGeneral; //112, 14; 142 27
	glictPanel pnlGraphics; // 112 47
	glictPanel pnlConsole; // 112 80
	glictPanel pnlHotkeys; // 112 113

	glictPanel pnlSeparator1; // 13, 147 -- 250 2

	glictButton btnMotd; // 15 165
	glictPanel pnlMotd; // 112 163

	glictPanel pnlSeparator2; // 9 204 -- 260 2

	glictButton btnOk; // 227 215 -- 41 17

	winOptions_t () {

		window.SetVisible(false);
		window.SetHeight(238);
		window.SetWidth(278);
		window.SetCaption("Options");

		window.AddObject(&btnGeneral);
		btnGeneral.SetPos(15, 16);
		btnGeneral.SetWidth(84);
		btnGeneral.SetHeight(18);
		btnGeneral.SetCaption("General");

		window.AddObject(&btnGraphics);
		btnGraphics.SetPos(15, 49);
		btnGraphics.SetWidth(84);
		btnGraphics.SetHeight(18);
		btnGraphics.SetCaption("Graphics");

		window.AddObject(&btnConsole);
		btnConsole.SetPos(15, 82);
		btnConsole.SetWidth(84);
		btnConsole.SetHeight(18);
		btnConsole.SetCaption("Console");

		window.AddObject(&btnHotkeys);
		btnHotkeys.SetPos(15, 115);
		btnHotkeys.SetWidth(84);
		btnHotkeys.SetHeight(18);
		btnHotkeys.SetCaption("Hotkeys");

		window.AddObject(&pnlGeneral);
		pnlGeneral.SetPos(112, 14);
		pnlGeneral.SetWidth(142);
		pnlGeneral.SetHeight(27);
		pnlGeneral.SetCaption("Change general\ngame options");
		pnlGeneral.SetBGActiveness(false);

		window.AddObject(&pnlGraphics);
		pnlGraphics.SetPos(112, 47);
		pnlGraphics.SetWidth(142);
		pnlGraphics.SetHeight(27);
		pnlGraphics.SetCaption("Change graphics and\nperformance settings");
		pnlGraphics.SetBGActiveness(false);

		window.AddObject(&pnlConsole);
		pnlConsole.SetPos(112, 80);
		pnlConsole.SetWidth(142);
		pnlConsole.SetHeight(27);
		pnlConsole.SetCaption("Customize the console");
		pnlConsole.SetBGActiveness(false);

		window.AddObject(&pnlHotkeys);
		pnlHotkeys.SetPos(112, 113);
		pnlHotkeys.SetWidth(142);
		pnlHotkeys.SetHeight(27);
		pnlHotkeys.SetCaption("Edit your hotkey texts");
		pnlHotkeys.SetBGActiveness(false);

		window.AddObject(&pnlSeparator1);
		pnlSeparator1.SetPos(13, 143);
		pnlSeparator1.SetWidth(250);
		pnlSeparator1.SetHeight(2);
		pnlSeparator1.SetBGColor(.2,.2,.2,1.);

		window.AddObject(&btnMotd);
		btnMotd.SetPos(15, 165);
		btnMotd.SetWidth(84);
		btnMotd.SetHeight(18);
		btnMotd.SetCaption("Motd");

		window.AddObject(&pnlMotd);
		pnlMotd.SetPos(112, 163);
		pnlMotd.SetWidth(142);
		pnlMotd.SetHeight(27);
		pnlMotd.SetCaption("Show the most recent\nMessage of the Day");
		pnlMotd.SetBGActiveness(false);

		window.AddObject(&pnlSeparator2);
		pnlSeparator2.SetPos(9, 204);
		pnlSeparator2.SetWidth(260);
		pnlSeparator2.SetHeight(2);
		pnlSeparator2.SetBGColor(.2,.2,.2,1.);


		window.AddObject(&btnOk);
		btnOk.SetPos(227, 215 );
		btnOk.SetWidth(41);
		btnOk.SetHeight(17);
		btnOk.SetCaption("Ok");
	}

};


#endif
