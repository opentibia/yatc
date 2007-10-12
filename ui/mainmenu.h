#ifndef __UI_MAINMENU_H
#define __UI_MAINMENU_H

class pnlMainMenu_t {
public:

	glictPanel mainmenu;
	glictButton btnLogIn, btnOptions, btnAbout, btnExit;

	pnlMainMenu_t () {

		mainmenu.SetWidth(118);
		mainmenu.SetHeight(170);
		mainmenu.SetBGColor(.4,.4,.4,1.);

		mainmenu.AddObject(&btnLogIn);
		mainmenu.AddObject(&btnOptions);
		mainmenu.AddObject(&btnAbout);
		mainmenu.AddObject(&btnExit);

		btnLogIn.SetWidth(84);
		btnLogIn.SetHeight(20);
		btnLogIn.SetCaption("Enter Game");
		btnLogIn.SetPos(16,16);
		btnLogIn.SetBGColor(.2,.2,.2,1.);
		btnLogIn.SetFont("minifont",8);

		btnOptions.SetWidth(84);
		btnOptions.SetHeight(20);
		btnOptions.SetCaption("Options");
		btnOptions.SetPos(16,76);
		btnOptions.SetBGColor(.2,.2,.2,1.);
		btnOptions.SetFont("system");
		btnOptions.SetFont("minifont",8);

		btnAbout.SetWidth(84);
		btnAbout.SetHeight(20);
		btnAbout.SetCaption("Info");
		btnAbout.SetPos(16,106);
		btnAbout.SetBGColor(.2,.2,.2,1.);
		btnAbout.SetFont("minifont",8);

		btnExit.SetWidth(84);
		btnExit.SetHeight(20);
		btnExit.SetCaption("Exit Game");
		btnExit.SetPos(16,136);
		btnExit.SetBGColor(.2,.2,.2,1.);
		btnExit.SetFont("minifont",8);
	}
};

#endif
