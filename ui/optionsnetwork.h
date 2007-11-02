#ifndef __UI_OPTIONSNETWORK_H
#define __UI_OPTIONSNETWORK_H

#include <sstream> // FIXME (ivucica#1#): remove me after cleaning up the int=>str conversion
class winOptionsNetwork_t {
public:

	glictWindow window;

	glictPanel lblServer;// 16 16, 198 11
	glictTextbox txtServer;// 16 27, 198 11

	glictPanel lblPort;// 16 49, 198 11
	glictTextbox txtPort;// 16 60, 198 11

	// 10 189, 210 2
	glictPanel pnlSeparator;


	glictButton btnHelp;// 72 200, 40 17
	glictButton btnOk; // 125 200
	glictButton btnCancel; // 178 200
	winOptionsNetwork_t () {

		window.SetVisible(false);
		window.SetHeight(238);
		window.SetWidth(225);
		window.SetCaption("Network Options");
		window.SetBGColor(.4, .4, .4, 1.);

		window.AddObject(&lblServer);
		lblServer.SetCaption("Server address:");
		lblServer.SetPos(16, 16);
		lblServer.SetWidth(198);
		lblServer.SetHeight(11);
		lblServer.SetBGActiveness(false);
		lblServer.SetFont("aafont");

		window.AddObject(&txtServer);
		txtServer.SetCaption("");
		txtServer.SetPos(16, 27);
		txtServer.SetWidth(200);
		txtServer.SetHeight(13);
		txtServer.SetFont("aafont");

		window.AddObject(&lblPort);
		lblPort.SetCaption("Server port:");
		lblPort.SetPos(16, 49);
		lblPort.SetWidth(200);
		lblPort.SetHeight(13);
		lblPort.SetBGActiveness(false);
		lblPort.SetFont("aafont");

		window.AddObject(&txtPort);
		txtPort.SetCaption("");
		txtPort.SetPos(16, 60);
		txtPort.SetWidth(200);
		txtPort.SetHeight(13);
		txtPort.SetFont("aafont");



		window.AddObject(&pnlSeparator);
		pnlSeparator.SetPos(10, 189);
		pnlSeparator.SetWidth(210);
		pnlSeparator.SetHeight(2);
		pnlSeparator.SetBGColor(.2,.2,.2,1.);

		window.AddObject(&btnHelp);
		btnHelp.SetPos(72, 200);
		btnHelp.SetWidth(43);
		btnHelp.SetHeight(19);
		btnHelp.SetCaption("Help");
		btnHelp.SetFont("minifont",8);

		window.AddObject(&btnOk);
		btnOk.SetPos(125, 200);
		btnOk.SetWidth(43);
		btnOk.SetHeight(19);
		btnOk.SetCaption("Ok");
		btnOk.SetFont("minifont",8);

		window.AddObject(&btnCancel);
		btnCancel.SetPos(178, 200);
		btnCancel.SetWidth(43);
		btnCancel.SetHeight(19);
		btnCancel.SetCaption("Cancel");
		btnCancel.SetFont("minifont",8);
	}

	void Init() {
		std::stringstream port; // FIXME (ivucica#1#) somebody should enlighten me on a clean integer=>string conversion
		port << options.port;

		txtServer.SetCaption(options.server);
		txtPort.SetCaption(port.str());
	}

	void Store() {
		options.server = txtServer.GetCaption();
		options.port = atoi(txtPort.GetCaption().c_str());
		options.Save();
	}

	static void OnCheckbox(glictPos* pos, glictContainer *caller) {
		if (caller->GetCaption() == "X")
			caller->SetCaption("");
		else
			caller->SetCaption("X");
	}

};


#endif

