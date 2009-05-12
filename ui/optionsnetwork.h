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
#if defined(HAVE_LIBINTL_H)
    #include <libintl.h>
#else
    #define gettext(x) (x)
#endif
#include "../net/connection.h"
#include "checkbox.h"
#include "../choicegrid.h"
class winOptionsNetwork_t {
public:


	glictWindow window;

	glictPanel lblServer;// 16 16, 198 11
	glictTextbox txtServer;// 16 27, 198 11

	glictPanel lblPort;// 16 49, 198 11
	glictTextbox txtPort;// 16 60, 198 11

    uiCheckbox chkOTKey;

	//std::map<ClientVersion_t, glictButton> btnProtocol;
	ChoiceGrid btnProtocol;
	std::vector <ChoiceGrid::Item*> protocols;

	// 10 189, 210 2
	glictPanel pnlSeparator;


	glictButton btnHelp;// 72 200, 40 17
	glictButton btnOk; // 125 200
	glictButton btnCancel; // 178 200


	ClientVersion_t currentversion;


	winOptionsNetwork_t () {

		window.SetVisible(false);
		window.SetHeight(324);
		window.SetWidth(225);
		window.SetCaption(gettext("Network Options"));
		window.SetBGColor(.4, .4, .4, 1.);

		window.AddObject(&lblServer);
		lblServer.SetCaption(gettext("Server address:"));
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
		txtServer.SetNext(&txtPort);

		window.AddObject(&lblPort);
		lblPort.SetCaption(gettext("Server port:"));
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
		txtPort.SetPrevious(&txtServer);


        window.AddObject(&chkOTKey.pnlPanel);
		chkOTKey.SetPos(16,80);
		chkOTKey.SetWidth(198);
		chkOTKey.SetHeight(20);
		chkOTKey.SetCaption(gettext("OT Encryption Key"));



		window.AddObject(btnProtocol.getGrid());
		btnProtocol.getGrid()->SetPos(12,96);
		btnProtocol.setItemSize(100,20);
		btnProtocol.setPadding(4,4);
		btnProtocol.setRows(2);
		btnProtocol.setOnClick(OnProtocol);
		btnProtocol.setData(this);
		btnProtocol.setFont("system");
		/*
		// implementation is just cut-and-paste, may need work.
		protocols.push_back(btnProtocol.addItem(std::string(gettext("Protocol")) + " 7.8", NULL, (void*)CLIENT_VERSION_780));
// not yet implemented

		protocols.push_back(btnProtocol.addItem(std::string(gettext("Protocol")) + " 7.81", NULL, (void*)CLIENT_VERSION_781));
		protocols.push_back(btnProtocol.addItem(std::string(gettext("Protocol")) + " 7.82", NULL, (void*)CLIENT_VERSION_782));
		protocols.push_back(btnProtocol.addItem(std::string(gettext("Protocol")) + " 7.9", NULL, (void*)CLIENT_VERSION_790));
		protocols.push_back(btnProtocol.addItem(std::string(gettext("Protocol")) + " 7.91", NULL, (void*)CLIENT_VERSION_791));
		protocols.push_back(btnProtocol.addItem(std::string(gettext("Protocol")) + " 7.92", NULL, (void*)CLIENT_VERSION_780));
		*/
// end of not yet implemented
		protocols.push_back(btnProtocol.addItem(std::string(gettext("Protocol")) + " 8.0", NULL, (void*)CLIENT_VERSION_800));
		protocols.push_back(btnProtocol.addItem(std::string(gettext("Protocol")) + " 8.1", NULL, (void*)CLIENT_VERSION_810));
		protocols.push_back(btnProtocol.addItem(std::string(gettext("Protocol")) + " 8.11", NULL, (void*)CLIENT_VERSION_820));
		protocols.push_back(btnProtocol.addItem(std::string(gettext("Protocol")) + " 8.2", NULL, (void*)CLIENT_VERSION_821));
		protocols.push_back(btnProtocol.addItem(std::string(gettext("Protocol")) + " 8.22", NULL, (void*)CLIENT_VERSION_822));
		protocols.push_back(btnProtocol.addItem(std::string(gettext("Protocol")) + " 8.3", NULL, (void*)CLIENT_VERSION_830));
		protocols.push_back(btnProtocol.addItem(std::string(gettext("Protocol")) + " 8.31", NULL, (void*)CLIENT_VERSION_831));
		protocols.push_back(btnProtocol.addItem(std::string(gettext("Protocol")) + " 8.4", NULL, (void*)CLIENT_VERSION_840));
		protocols.push_back(btnProtocol.addItem(std::string(gettext("Protocol")) + " 8.41", NULL, (void*)CLIENT_VERSION_841));
		protocols.push_back(btnProtocol.addItem(std::string(gettext("Protocol")) + " 8.42", NULL, (void*)CLIENT_VERSION_842));
		protocols.push_back(btnProtocol.addItem(gettext("Autodetect"), NULL, (void*)CLIENT_VERSION_AUTO));

		window.AddObject(&pnlSeparator);
		pnlSeparator.SetPos(10, 100+((btnProtocol.getCount()-1)/2+1)*24 + 3);
		pnlSeparator.SetWidth(210);
		pnlSeparator.SetHeight(2);
		pnlSeparator.SetBGColor(.2,.2,.2,1.);
		pnlSeparator.SetSkin(&g_skin.chk);

		window.AddObject(&btnHelp);
		btnHelp.SetPos(72, 210-102 + ((btnProtocol.getCount()-1)/2+1)*24);
		btnHelp.SetWidth(43);
		btnHelp.SetHeight(19);
		btnHelp.SetCaption(gettext("Help"));
		btnHelp.SetFont("minifont",8);

		window.AddObject(&btnOk);
		btnOk.SetPos(125, 210-102 + ((btnProtocol.getCount()-1)/2+1)*24);
		btnOk.SetWidth(43);
		btnOk.SetHeight(19);
		btnOk.SetCaption(gettext("Ok"));
		btnOk.SetFont("minifont",8);

		window.AddObject(&btnCancel);
		btnCancel.SetPos(178, 210-102 + ((btnProtocol.getCount()-1)/2+1)*24);
		btnCancel.SetWidth(43);
		btnCancel.SetHeight(19);
		btnCancel.SetCaption(gettext("Cancel"));
		btnCancel.SetFont("minifont",8);


		window.SetHeight(210-102 + ((btnProtocol.getCount()-1)/2+1)*24 + 25);
	}

	void Init() {
	    std::string port = yatc_itoa(options.port);

		txtServer.SetCaption(options.server);
		txtPort.SetCaption(port/*.str()*/);

        chkOTKey.SetValue(options.otkey);

		currentversion = options.protocol;
        for (std::vector<ChoiceGrid::Item*>::iterator it = protocols.begin(); it != protocols.end(); it++) {

            ChoiceGrid::Item* i = *it;
            if ((ClientVersion_t)((long)i->data) == currentversion)
            {
                btnProtocol.setSelected(i);
            }
		}
	}

	void Store() {
		options.server = txtServer.GetCaption();
		options.port = atoi(txtPort.GetCaption().c_str());
		options.otkey = chkOTKey.GetValue();
		options.protocol = currentversion;
		options.Save();
	}

	static void OnProtocol(ChoiceGrid* parent, ChoiceGrid::Item* item, ChoiceGrid::Item* olditem) {

		winOptionsNetwork_t* won = (winOptionsNetwork_t*)parent->getData();
		won->currentversion = (ClientVersion_t)((long)item->data);
		//ProtocolConfig::getInstance().setVersion(CLIENT_OS_WIN, (ClientVersion_t)((long)item->data));
	}

};


#endif

