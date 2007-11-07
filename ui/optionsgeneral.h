#ifndef __UI_OPTIONSGENERAL_H
#define __UI_OPTIONSGENERAL_H

#include "skin.h"

class winOptionsGeneral_t {
public:


	glictWindow window;

	// 16 16, 198 20
	glictPanel pnlClassicControl;
	glictPanel btnClassicControl;
	glictPanel lblClassicControl;

	// 16 49, 198 20
	glictPanel pnlAutoChase;
	glictPanel btnAutoChase;
	glictPanel lblAutoChase;

	// 16 82
	glictPanel pnlShowHints;
	glictPanel btnShowHints;
	glictPanel lblShowHints;

	// 16 115
	glictPanel pnlShowNames;
	glictPanel btnShowNames;
	glictPanel lblShowNames;

	// 16 148
	glictPanel pnlShowTextEffects;
	glictPanel btnShowTextEffects;
	glictPanel lblShowTextEffects;

	// 10 189, 210 2
	glictPanel pnlSeparator;


	glictButton btnHelp;// 72 200, 40 17
	glictButton btnOk; // 125 200
	glictButton btnCancel; // 178 200
	winOptionsGeneral_t () {

		window.SetVisible(false);
		window.SetHeight(256);
		window.SetWidth(228);
		window.SetCaption("General Options");
		window.SetBGColor(.4, .4, .4, 1.);

		window.AddObject(&pnlClassicControl);
		pnlClassicControl.SetPos(16,16);
		pnlClassicControl.SetWidth(198);
		pnlClassicControl.SetHeight(20);
		pnlClassicControl.AddObject(&btnClassicControl);
		pnlClassicControl.AddObject(&lblClassicControl);
		pnlClassicControl.SetBGActiveness(false);
		btnClassicControl.SetWidth(8);
		btnClassicControl.SetHeight(8);
		btnClassicControl.SetPos(7, 6);
		btnClassicControl.SetOnClick(winOptionsGeneral_t::OnCheckbox);
		lblClassicControl.SetWidth(170);
		lblClassicControl.SetHeight(11);
		lblClassicControl.SetPos(22,4);
		lblClassicControl.SetCaption("Classic Control");
		lblClassicControl.SetFont("aafont");
		lblClassicControl.SetBGActiveness(false);

		window.AddObject(&pnlAutoChase);
		pnlAutoChase.SetPos(16, 49);
		pnlAutoChase.SetWidth(198);
		pnlAutoChase.SetHeight(20);
		pnlAutoChase.AddObject(&btnAutoChase);
		pnlAutoChase.AddObject(&lblAutoChase);
		pnlAutoChase.SetBGActiveness(false);
		btnAutoChase.SetWidth(8);
		btnAutoChase.SetHeight(8);
		btnAutoChase.SetPos(7, 6);
		btnAutoChase.SetOnClick(winOptionsGeneral_t::OnCheckbox);
		lblAutoChase.SetWidth(170);
		lblAutoChase.SetHeight(11);
		lblAutoChase.SetPos(22,4);
		lblAutoChase.SetCaption("Auto Chase Off");
		lblAutoChase.SetFont("aafont");
		lblAutoChase.SetBGActiveness(false);

		window.AddObject(&pnlShowHints);
		pnlShowHints.SetPos(16, 82);
		pnlShowHints.SetWidth(198);
		pnlShowHints.SetHeight(20);
		pnlShowHints.AddObject(&btnShowHints);
		pnlShowHints.AddObject(&lblShowHints);
		pnlShowHints.SetBGActiveness(false);
		btnShowHints.SetWidth(8);
		btnShowHints.SetHeight(8);
		btnShowHints.SetPos(7, 6);
		btnShowHints.SetOnClick(winOptionsGeneral_t::OnCheckbox);
		lblShowHints.SetWidth(170);
		lblShowHints.SetHeight(11);
		lblShowHints.SetPos(22,4);
		lblShowHints.SetCaption("Show Hints");
		lblShowHints.SetFont("aafont");
		lblShowHints.SetBGActiveness(false);

		window.AddObject(&pnlShowNames);
		pnlShowNames.SetPos(16, 115);
		pnlShowNames.SetWidth(198);
		pnlShowNames.SetHeight(20);
		pnlShowNames.AddObject(&btnShowNames);
		pnlShowNames.AddObject(&lblShowNames);
		pnlShowNames.SetBGActiveness(false);
		btnShowNames.SetWidth(8);
		btnShowNames.SetHeight(8);
		btnShowNames.SetPos(7, 6);
		btnShowNames.SetOnClick(winOptionsGeneral_t::OnCheckbox);
		lblShowNames.SetWidth(170);
		lblShowNames.SetHeight(11);
		lblShowNames.SetPos(22,4);
		lblShowNames.SetCaption("Show Names of Creatures");
		lblShowNames.SetFont("aafont");
		lblShowNames.SetBGActiveness(false);

		window.AddObject(&pnlShowTextEffects);
		pnlShowTextEffects.SetPos(16, 148);
		pnlShowTextEffects.SetWidth(198);
		pnlShowTextEffects.SetHeight(20);
		pnlShowTextEffects.AddObject(&btnShowTextEffects);
		pnlShowTextEffects.AddObject(&lblShowTextEffects);
		pnlShowTextEffects.SetBGActiveness(false);
		btnShowTextEffects.SetWidth(8);
		btnShowTextEffects.SetHeight(8);
		btnShowTextEffects.SetPos(7, 6);
		btnShowTextEffects.SetOnClick(winOptionsGeneral_t::OnCheckbox);
		lblShowTextEffects.SetWidth(170);
		lblShowTextEffects.SetHeight(11);
		lblShowTextEffects.SetPos(22,4);
		lblShowTextEffects.SetCaption("Show Textual Effects");
		lblShowTextEffects.SetFont("aafont");
		lblShowTextEffects.SetBGActiveness(false);

		window.AddObject(&pnlSeparator);
		pnlSeparator.SetPos(10, 189);
		pnlSeparator.SetWidth(210);
		pnlSeparator.SetHeight(2);
		pnlSeparator.SetBGColor(.2,.2,.2,1.);

		window.AddObject(&btnHelp);
		btnHelp.SetPos(72, 200);
		btnHelp.SetWidth(41);
		btnHelp.SetHeight(17);
		btnHelp.SetCaption("Help");
		btnHelp.SetFont("minifont",8);

		window.AddObject(&btnOk);
		btnOk.SetPos(125, 200);
		btnOk.SetWidth(41);
		btnOk.SetHeight(17);
		btnOk.SetCaption("Ok");
		btnOk.SetFont("minifont",8);

		window.AddObject(&btnCancel);
		btnCancel.SetPos(178, 200);
		btnCancel.SetWidth(41);
		btnCancel.SetHeight(17);
		btnCancel.SetCaption("Cancel");
		btnCancel.SetFont("minifont",8);
	}


	void Init() {
		btnAutoChase.SetCustomData(options.autochase ? (void*)1 : NULL);
		btnAutoChase.SetSkin(options.autochase ? &g_skin.chk : &g_skin.txt);
		btnShowHints.SetCustomData(options.showhints ? (void*)1 : NULL);
		btnShowHints.SetSkin(options.showhints ? &g_skin.chk : &g_skin.txt);
		btnShowTextEffects.SetCustomData(options.showtexteffects ? (void*)1 : NULL);
		btnShowTextEffects.SetSkin(options.showtexteffects ? &g_skin.chk : &g_skin.txt);
		btnClassicControl.SetCustomData(options.classiccontrol ? (void*)1 : NULL);
		btnClassicControl.SetSkin(options.classiccontrol ? &g_skin.chk : &g_skin.txt);
		btnShowNames.SetCustomData(options.shownames ? (void*)1 : NULL);
		btnShowNames.SetSkin(options.shownames ? &g_skin.chk : &g_skin.txt);
	}

	void Store() {
		options.autochase = (btnAutoChase.GetCustomData() != NULL);
		options.showhints = (btnShowHints.GetCustomData() != NULL);
		options.showtexteffects = (btnShowTextEffects.GetCustomData() != NULL);
		options.classiccontrol = (btnClassicControl.GetCustomData() != NULL);
		options.shownames = (btnShowNames.GetCustomData() != NULL);
	}

	static void OnCheckbox(glictPos* pos, glictContainer *caller) {
		if ((int)caller->GetCustomData() == 1) {
			caller->SetCustomData(NULL);
			((glictPanel*)caller)->SetSkin(&g_skin.txt);
		} else {
			caller->SetCustomData((void*)1);
			((glictPanel*)caller)->SetSkin(&g_skin.chk);
		}
	}

};


#endif
