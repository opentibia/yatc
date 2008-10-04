//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////

#ifndef __UI_OPTIONSNETWORK_H
#define __UI_OPTIONSNETWORK_H

//#include <sstream> // FIXME (ivucica#1#): remove me after cleaning up the int=>str conversion
#include "../util.h"
#include <map>
#include "../net/connection.h"
#include "checkbox.h"
class winOptionsNetwork_t {
public:


	glictWindow window;

	glictPanel lblServer;// 16 16, 198 11
	glictTextbox txtServer;// 16 27, 198 11

	glictPanel lblPort;// 16 49, 198 11
	glictTextbox txtPort;// 16 60, 198 11

    uiCheckbox chkOTKey;

	std::map<ClientVersion_t, glictButton> btnProtocol;

	// 10 189, 210 2
	glictPanel pnlSeparator;


	glictButton btnHelp;// 72 200, 40 17
	glictButton btnOk; // 125 200
	glictButton btnCancel; // 178 200


	ClientVersion_t currentversion;


	winOptionsNetwork_t () {

		window.SetVisible(false);
		window.SetHeight(248);
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


        window.AddObject(&chkOTKey.pnlPanel);
		chkOTKey.SetPos(16,80);
		chkOTKey.SetWidth(198);
		chkOTKey.SetHeight(20);
		chkOTKey.SetCaption("OT Encryption Key");


		window.AddObject(&btnProtocol[CLIENT_VERSION_800]);
		btnProtocol[CLIENT_VERSION_800].SetCaption("Protocol 8.0");
		window.AddObject(&btnProtocol[CLIENT_VERSION_810]);
		btnProtocol[CLIENT_VERSION_810].SetCaption("Protocol 8.1");
		window.AddObject(&btnProtocol[CLIENT_VERSION_811]);
		btnProtocol[CLIENT_VERSION_811].SetCaption("Protocol 8.11");
		window.AddObject(&btnProtocol[CLIENT_VERSION_820]);
		btnProtocol[CLIENT_VERSION_820].SetCaption("Protocol 8.2");
		window.AddObject(&btnProtocol[CLIENT_VERSION_821]);
		btnProtocol[CLIENT_VERSION_821].SetCaption("Protocol 8.21");
		window.AddObject(&btnProtocol[CLIENT_VERSION_822]);
		btnProtocol[CLIENT_VERSION_822].SetCaption("Protocol 8.22");
		window.AddObject(&btnProtocol[CLIENT_VERSION_830]);
		btnProtocol[CLIENT_VERSION_830].SetCaption("Protocol 8.3");
		window.AddObject(&btnProtocol[CLIENT_VERSION_831]);
		btnProtocol[CLIENT_VERSION_831].SetCaption("Protocol 8.31");
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
		pnlSeparator.SetPos(10, 100+((pc-1)/2+1)*24 + 3);
		pnlSeparator.SetWidth(210);
		pnlSeparator.SetHeight(2);
		pnlSeparator.SetBGColor(.2,.2,.2,1.);

		window.AddObject(&btnHelp);
		btnHelp.SetPos(72, 210-102 + ((pc-1)/2+1)*24);
		btnHelp.SetWidth(43);
		btnHelp.SetHeight(19);
		btnHelp.SetCaption("Help");
		btnHelp.SetFont("minifont",8);

		window.AddObject(&btnOk);
		btnOk.SetPos(125, 210-102 + ((pc-1)/2+1)*24);
		btnOk.SetWidth(43);
		btnOk.SetHeight(19);
		btnOk.SetCaption("Ok");
		btnOk.SetFont("minifont",8);

		window.AddObject(&btnCancel);
		btnCancel.SetPos(178, 210-102 + ((pc-1)/2+1)*24);
		btnCancel.SetWidth(43);
		btnCancel.SetHeight(19);
		btnCancel.SetCaption("Cancel");
		btnCancel.SetFont("minifont",8);
	}

	void Init() {
	    std::string port = yatc_itoa(options.port);

		txtServer.SetCaption(options.server);
		txtPort.SetCaption(port/*.str()*/);

        chkOTKey.SetValue(options.otkey);

		currentversion = options.protocol;
		for (std::map<ClientVersion_t, glictButton>::iterator it = btnProtocol.begin(); it != btnProtocol.end(); it++) {
			if (it->first == currentversion)
				it->second.SetHold(true);
			else
				it->second.SetHold(false);
		}
	}

	void Store() {
		options.server = txtServer.GetCaption();
		options.port = atoi(txtPort.GetCaption().c_str());
		options.otkey = chkOTKey.GetValue();
		options.protocol = currentversion;
		options.Save();
	}

	static void OnProtocol(glictPos* pos, glictContainer *caller) {
		glictButton* b = (glictButton*)caller;
		winOptionsNetwork_t* won = (winOptionsNetwork_t*)b->GetCustomData();

		for (std::map<ClientVersion_t, glictButton>::iterator it = won->btnProtocol.begin(); it != won->btnProtocol.end(); it++) {
			if (&it->second == b) {
				won->currentversion = it->first;
			} else {
				it->second.SetHold(false);
			}
		}
		b->SetHold(true);
	}

};


#endif

