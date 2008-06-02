#ifndef __UI_MAINMENU_H
#define __UI_MAINMENU_H


class pnlMainMenu_t {
public:

	glictPanel mainmenu;
	glictButton btnLogIn, btnNetwork, btnOptions, btnAbout, btnExit;

	pnlMainMenu_t () {

		mainmenu.SetWidth(118);
		mainmenu.SetHeight(170);
		mainmenu.SetBGColor(.4,.4,.4,1.);
		mainmenu.SetSkin(&g_skin.tmm );

		mainmenu.AddObject(&btnLogIn);
		mainmenu.AddObject(&btnNetwork);
		mainmenu.AddObject(&btnOptions);
		mainmenu.AddObject(&btnAbout);
		mainmenu.AddObject(&btnExit);

		btnLogIn.SetWidth(84);
		btnLogIn.SetHeight(20);
		btnLogIn.SetCaption("Enter Game");
		btnLogIn.SetPos(16,16);
		btnLogIn.SetBGColor(.8,.8,.8,1.);
		btnLogIn.SetFont("minifont",8);
		if (options.ui_compat) {
			btnLogIn.SetNext(&btnNetwork),
			btnLogIn.SetPrevious(&btnExit);
        }

		btnNetwork.SetWidth(84);
		btnNetwork.SetHeight(20);
		btnNetwork.SetCaption("Network Options");
		btnNetwork.SetPos(16,46);
		btnNetwork.SetBGColor(.8,.8,.8,1.);
		btnNetwork.SetFont("minifont",8);
		if (options.ui_compat) {
			btnNetwork.SetNext(&btnOptions),
			btnNetwork.SetPrevious(&btnLogIn);
		}

		btnOptions.SetWidth(84);
		btnOptions.SetHeight(20);
		btnOptions.SetCaption("Options");
		btnOptions.SetPos(16,76);
		btnOptions.SetBGColor(.8,.8,.8,1.);
		btnOptions.SetFont("system");
		btnOptions.SetFont("minifont",8);
		if (options.ui_compat) {
			btnOptions.SetNext(&btnAbout),
			btnOptions.SetPrevious(&btnNetwork);
		}

		btnAbout.SetWidth(84);
		btnAbout.SetHeight(20);
		btnAbout.SetCaption("Info");
		btnAbout.SetPos(16,106);
		btnAbout.SetBGColor(.8,.8,.8,1.);
		btnAbout.SetFont("minifont",8);
		if (options.ui_compat) {
			btnAbout.SetNext(&btnExit),
			btnAbout.SetPrevious(&btnOptions);
		}

		btnExit.SetWidth(84);
		btnExit.SetHeight(20);
		btnExit.SetCaption("Exit Game");
		btnExit.SetPos(16,136);
		btnExit.SetBGColor(.8,.8,.8,1.);
		btnExit.SetFont("minifont",8);
		if (options.ui_compat) {
			btnExit.SetNext(&btnLogIn),
			btnExit.SetPrevious(&btnAbout);
		}
	}
};

#endif
