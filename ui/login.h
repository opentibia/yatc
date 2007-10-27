#ifndef __UI_LOGIN_H
#define __UI_LOGIN_H

class winLogin_t {
public:

	glictWindow window;
	glictPanel pnlUsername, pnlPassword, pnlCreateAc;
	glictTextbox txtUsername, txtPassword;
	glictButton btnCreateAc;

	glictPanel pnlSeparator;

	glictButton btnOk, btnCancel;

	winLogin_t() {

		window.SetWidth(231-4);
		window.SetHeight(173-17);
		window.SetCaption("Enter Game");
		window.SetVisible(false);
		window.SetBGColor(.4, .4, .4, 1.);

		window.AddObject(&pnlUsername);
		window.AddObject(&txtUsername);
		window.AddObject(&pnlPassword);
		window.AddObject(&txtPassword);
		window.AddObject(&pnlCreateAc);
		window.AddObject(&btnCreateAc);
		window.AddObject(&pnlSeparator); /* **** */
		window.AddObject(&btnOk);
		window.AddObject(&btnCancel);

		pnlUsername.SetWidth(131-17);
		pnlUsername.SetHeight(12);
		pnlUsername.SetCaption("Account number:");
		pnlUsername.SetPos(13,17);
		pnlUsername.SetBGActiveness(false);
		pnlUsername.SetFont("aafont");

		txtUsername.SetPos(128,15);
		txtUsername.SetWidth(216-134);
		txtUsername.SetHeight(46-33);
		txtUsername.SetPassProtectCharacter('*');
		#if (GLICT_APIREV >= 2)
		txtUsername.SetAllowedChars("0123456789");
		#endif
		#if (GLICT_APIREV >= 46)
		txtUsername.SetPrevious(&txtPassword);
		txtUsername.SetNext(&txtPassword);
		#endif

		pnlPassword.SetWidth(131-17);
		pnlPassword.SetHeight(12);
		pnlPassword.SetCaption("Password:");
		pnlPassword.SetPos(13,47);
		pnlPassword.SetBGActiveness(false);
		pnlPassword.SetFont("aafont");

		txtPassword.SetPos(128,45);
		txtPassword.SetWidth(216-134);
		txtPassword.SetHeight(46-33);
		txtPassword.SetPassProtectCharacter('*');
		#if (GLICT_APIREV >= 2)
		txtPassword.SetAllowedChars(" !\"#$%/()=?*'+<>[]{}`~\\|;:_,.-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
		#endif
		#if (GLICT_APIREV >= 46)
		txtPassword.SetPrevious(&txtUsername);
		txtPassword.SetNext(&txtUsername);
		#endif

		pnlCreateAc.SetWidth(131-17);
		pnlCreateAc.SetHeight(24);
		pnlCreateAc.SetCaption("If you don't have\nan account yet:");
		pnlCreateAc.SetPos(13,85);
		pnlCreateAc.SetBGActiveness(false);
		pnlCreateAc.SetFont("aafont");

		btnCreateAc.SetPos(128,94);
		btnCreateAc.SetWidth(216-133);
		btnCreateAc.SetHeight(112-95);
		btnCreateAc.SetCaption("Create Account");
		btnCreateAc.SetBGColor(.2,.2,.2,1.);
		btnCreateAc.SetFont("minifont",8);

		pnlSeparator.SetPos(9, 121);
		pnlSeparator.SetBGColor(.1,.1,.1,1.);
		pnlSeparator.SetWidth(222-13);
		pnlSeparator.SetHeight(2);

		btnOk.SetPos(127-4,148-17);
		btnOk.SetWidth(40);
		btnOk.SetHeight(166-149);
		btnOk.SetCaption("Ok");
		btnOk.SetBGColor(.1,.1,.1,1.);
		btnOk.SetFont("minifont",8);
		/*
		#if (GLICT_APIREV >= 46)
		btnOk.SetPrevious(&txtPassword);
		btnOk.SetNext(&btnCancel);
		#endif
		*/

		btnCancel.SetPos(180-4,148-17);
		btnCancel.SetWidth(40);
		btnCancel.SetHeight(166-149);
		btnCancel.SetCaption("Cancel");
		btnCancel.SetBGColor(.1,.1,.1,1.);
		btnCancel.SetFont("minifont",8);
		/*
		#if (GLICT_APIREV >= 46)
		btnCancel.SetPrevious(&btnOk);
		btnCancel.SetNext(&txtUsername);
		#endif
		*/

	}
};

#endif
