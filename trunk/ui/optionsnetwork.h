#ifndef __UI_OPTIONSNETWORK_H
#define __UI_OPTIONSNETWORK_H

//#include <sstream> // FIXME (ivucica#1#): remove me after cleaning up the int=>str conversion
#include "util.h"
#include <map>
#include "net/connection.h"
class winOptionsNetwork_t {
public:


	glictWindow window;

	glictPanel lblServer;// 16 16, 198 11
	glictTextbox txtServer;// 16 27, 198 11

	glictPanel lblPort;// 16 49, 198 11
	glictTextbox txtPort;// 16 60, 198 11

	glictPanel pnlOTKey;
	glictPanel btnOTKey;
	glictPanel lblOTKey;

	std::map<ClientVersion_t, glictButton> btnProtocol;

	// 10 189, 210 2
	glictPanel pnlSeparator;


	glictButton btnHelp;// 72 200, 40 17
	glictButton btnOk; // 125 200
	glictButton btnCancel; // 178 200


	ClientVersion_t currentversion;


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



        window.AddObject(&pnlOTKey);
		pnlOTKey.SetPos(16, 80);
		pnlOTKey.SetWidth(198);
		pnlOTKey.SetHeight(20);
		pnlOTKey.AddObject(&btnOTKey);
		pnlOTKey.AddObject(&lblOTKey);
		pnlOTKey.SetBGActiveness(false);
		btnOTKey.SetWidth(12);
		btnOTKey.SetHeight(12);
		btnOTKey.SetPos(5, 5);
		btnOTKey.SetOnClick(winOptionsNetwork_t::OnCheckbox);
		lblOTKey.SetWidth(170);
		lblOTKey.SetHeight(11);
		lblOTKey.SetPos(22,7);
		lblOTKey.SetCaption("OT Encryption Key");
		lblOTKey.SetFont("aafont");
		lblOTKey.SetBGActiveness(false);

		window.AddObject(&btnProtocol[CLIENT_VERSION_800]);
		btnProtocol[CLIENT_VERSION_800].SetCaption("Protocol 8.0");
		window.AddObject(&btnProtocol[CLIENT_VERSION_810]);
		btnProtocol[CLIENT_VERSION_810].SetCaption("Protocol 8.1");
		window.AddObject(&btnProtocol[CLIENT_VERSION_AUTO]);
		btnProtocol[CLIENT_VERSION_AUTO].SetCaption("Autodetect");

		int pc=0;
		for (std::map<ClientVersion_t, glictButton>::iterator it = btnProtocol.begin(); it != btnProtocol.end(); it++) {
			it->second.SetHeight(20);
			it->second.SetWidth(100);
			it->second.SetPos(16+(pc%2)*100, 100+(pc/2)*24);
			it->second.SetCustomData(this);
			it->second.SetOnClick(winOptionsNetwork_t::OnProtocol);
			pc++;
		}

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
		//std::stringstream port; // FIXME (ivucica#1#) somebody should enlighten me on a clean integer=>string conversion
		//port << options.port;
		std::string buffer;
		std::string port = yatc_itoa(options.port, buffer);

		txtServer.SetCaption(options.server);
		txtPort.SetCaption(port/*.str()*/);

		btnOTKey.SetCustomData(options.otkey ? (void*)1 : NULL);
		btnOTKey.SetSkin(options.otkey ? &g_skin.chk : &g_skin.txt);

		currentversion = options.protocol;
		#if (GLICT_APIREV < 52)
		#warning Update your GLICT to rev52 or newer; OptionsNetwork will not work correctly until you do.
		#else
		for (std::map<ClientVersion_t, glictButton>::iterator it = btnProtocol.begin(); it != btnProtocol.end(); it++) {
			if (it->first == currentversion)
				it->second.SetHold(true);
			else
				it->second.SetHold(false);
		}
		#endif
	}

	void Store() {
		options.server = txtServer.GetCaption();
		options.port = atoi(txtPort.GetCaption().c_str());
		options.otkey = (btnOTKey.GetCustomData() != NULL);
		options.protocol = currentversion;
		options.Save();
	}

	static void OnCheckbox(glictPos* pos, glictContainer *caller) {
		if ((long)caller->GetCustomData() == 1) {
			caller->SetCustomData(NULL);
			((glictPanel*)caller)->SetSkin(&g_skin.txt);
		} else {
			caller->SetCustomData((void*)1);
			((glictPanel*)caller)->SetSkin(&g_skin.chk);
		}
	}
	static void OnProtocol(glictPos* pos, glictContainer *caller) {
		glictButton* b = (glictButton*)caller;
		winOptionsNetwork_t* won = (winOptionsNetwork_t*)b->GetCustomData();

		for (std::map<ClientVersion_t, glictButton>::iterator it = won->btnProtocol.begin(); it != won->btnProtocol.end(); it++) {
			if (&it->second == b) {
				won->currentversion = it->first;
			} else {
				#if (GLICT_APIREV < 52)
				#warning Update your GLICT to rev52 or newer; OptionsNetwork will not work correctly until you do.
				#else
				it->second.SetHold(false);
				#endif
			}
		}
		#if (GLICT_APIREV < 52)
		#warning Update your GLICT to rev52 or newer; OptionsNetwork will not work correctly until you do.
		#else
		b->SetHold(true);
		#endif
	}

};


#endif

