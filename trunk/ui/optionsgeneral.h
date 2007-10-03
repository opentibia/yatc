#ifndef __UI_OPTIONSGENERAL_H
#define __UI_OPTIONSGENERAL_H

class winOptionsGeneral_t {
public:

	glictWindow window;

	// 16 16, 198 20
	glictPanel pnlClassicControl;
	glictButton btnClassicControl;
	glictPanel lblClassicControl;

	// 16 49, 198 20
	glictPanel pnlAutoChase;
	glictButton btnAutoChase;
	glictPanel lblAutoChase;

	// 16 82
	glictPanel pnlShowHints;
	glictButton btnShowHints;
	glictPanel lblShowHints;

	// 16 115
	glictPanel pnlShowNames;
	glictButton btnShowNames;
	glictPanel lblShowNames;

	// 16 148
	glictPanel pnlShowTextEffects;
	glictButton btnShowTextEffects;
	glictPanel lblShowTextEffects;

	// 10 189, 210 2
	glictPanel pnlSeparator;


	glictButton btnHelp;// 72 200, 40 17
	glictButton btnOk; // 125 200
	glictButton btnCancel; // 178 200
	winOptionsGeneral_t () {

		window.SetVisible(false);
		window.SetHeight(238);
		window.SetWidth(225);
		window.SetCaption("General Options");

		window.AddObject(&pnlClassicControl);
		pnlClassicControl.SetPos(16,16);
		pnlClassicControl.SetWidth(198);
		pnlClassicControl.SetHeight(20);
		pnlClassicControl.AddObject(&btnClassicControl);
		pnlClassicControl.AddObject(&lblClassicControl);
		btnClassicControl.SetWidth(8);
		btnClassicControl.SetHeight(8);
		btnClassicControl.SetPos(7, 6);
		btnClassicControl.SetOnClick(winOptionsGeneral_t::OnCheckbox);
		lblClassicControl.SetWidth(170);
		lblClassicControl.SetHeight(11);
		lblClassicControl.SetPos(22,4);
		lblClassicControl.SetCaption("Classic Control");

		window.AddObject(&pnlAutoChase);
		pnlAutoChase.SetPos(16, 49);
		pnlAutoChase.SetWidth(198);
		pnlAutoChase.SetHeight(20);
		pnlAutoChase.AddObject(&btnAutoChase);
		pnlAutoChase.AddObject(&lblAutoChase);
		btnAutoChase.SetWidth(8);
		btnAutoChase.SetHeight(8);
		btnAutoChase.SetPos(7, 6);
		btnAutoChase.SetOnClick(winOptionsGeneral_t::OnCheckbox);
		lblAutoChase.SetWidth(170);
		lblAutoChase.SetHeight(11);
		lblAutoChase.SetPos(22,4);
		lblAutoChase.SetCaption("Auto Chase Off");

		window.AddObject(&pnlShowHints);
		pnlShowHints.SetPos(16, 82);
		pnlShowHints.SetWidth(198);
		pnlShowHints.SetHeight(20);
		pnlShowHints.AddObject(&btnShowHints);
		pnlShowHints.AddObject(&lblShowHints);
		btnShowHints.SetWidth(8);
		btnShowHints.SetHeight(8);
		btnShowHints.SetPos(7, 6);
		btnShowHints.SetOnClick(winOptionsGeneral_t::OnCheckbox);
		lblShowHints.SetWidth(170);
		lblShowHints.SetHeight(11);
		lblShowHints.SetPos(22,4);
		lblShowHints.SetCaption("Show Hints");

		window.AddObject(&pnlShowNames);
		pnlShowNames.SetPos(16, 115);
		pnlShowNames.SetWidth(198);
		pnlShowNames.SetHeight(20);
		pnlShowNames.AddObject(&btnShowNames);
		pnlShowNames.AddObject(&lblShowNames);
		btnShowNames.SetWidth(8);
		btnShowNames.SetHeight(8);
		btnShowNames.SetPos(7, 6);
		btnShowNames.SetOnClick(winOptionsGeneral_t::OnCheckbox);
		lblShowNames.SetWidth(170);
		lblShowNames.SetHeight(11);
		lblShowNames.SetPos(22,4);
		lblShowNames.SetCaption("Show Names of Creatures");

		window.AddObject(&pnlShowTextEffects);
		pnlShowTextEffects.SetPos(16, 148);
		pnlShowTextEffects.SetWidth(198);
		pnlShowTextEffects.SetHeight(20);
		pnlShowTextEffects.AddObject(&btnShowTextEffects);
		pnlShowTextEffects.AddObject(&lblShowTextEffects);
		btnShowTextEffects.SetWidth(8);
		btnShowTextEffects.SetHeight(8);
		btnShowTextEffects.SetPos(7, 6);
		btnShowTextEffects.SetOnClick(winOptionsGeneral_t::OnCheckbox);
		lblShowTextEffects.SetWidth(170);
		lblShowTextEffects.SetHeight(11);
		lblShowTextEffects.SetPos(22,4);
		lblShowTextEffects.SetCaption("Show Textual Effects");

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

		window.AddObject(&btnOk);
		btnOk.SetPos(125, 200);
		btnOk.SetWidth(41);
		btnOk.SetHeight(17);
		btnOk.SetCaption("Ok");

		window.AddObject(&btnCancel);
		btnCancel.SetPos(178, 200);
		btnCancel.SetWidth(41);
		btnCancel.SetHeight(17);
		btnCancel.SetCaption("Cancel");
	}
	void Init() {
		btnAutoChase.SetCaption("");
		btnShowHints.SetCaption("");
		btnShowTextEffects.SetCaption("");
		btnClassicControl.SetCaption("");
		btnShowNames.SetCaption("");
	}

	static void OnCheckbox(glictPos* pos, glictContainer *caller) {
		if (caller->GetCaption() == "X")
			caller->SetCaption("");
		else
			caller->SetCaption("X");
	}

};


#endif
