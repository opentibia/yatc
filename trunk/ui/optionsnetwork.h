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

		window.AddObject(&lblServer);
		lblServer.SetCaption("Server address:");
		lblServer.SetPos(16, 16);
		lblServer.SetWidth(198);
		lblServer.SetHeight(11);

		window.AddObject(&txtServer);
		txtServer.SetCaption("");
		txtServer.SetPos(16, 27);
		txtServer.SetWidth(198);
		txtServer.SetHeight(11);

		window.AddObject(&lblPort);
		lblPort.SetCaption("Server port:");
		lblPort.SetPos(16, 49);
		lblPort.SetWidth(198);
		lblPort.SetHeight(11);

		window.AddObject(&txtPort);
		txtPort.SetCaption("");
		txtPort.SetPos(16, 60);
		txtPort.SetWidth(198);
		txtPort.SetHeight(11);



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
		std::stringstream port; // FIXME (ivucica#1#) somebody should enlighten me on a clean integer=>string conversion
		port << options.port;

		txtServer.SetCaption(options.server);
		txtPort.SetCaption(port.str());
	}

	static void OnCheckbox(glictPos* pos, glictContainer *caller) {
		if (caller->GetCaption() == "X")
			caller->SetCaption("");
		else
			caller->SetCaption("X");
	}

};


#endif

