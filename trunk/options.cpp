//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
// Options keeper
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

#include <string>
#include <sstream>
#include <stdlib.h> // atoi
#include "options.h"
#include "confighandler.h"
#include "util.h"
Options options;

Options::Options()
{
	configHandler = new ConfigHandler();

	// [window]
	engine = ENGINE_SDL;
	//engine = ENGINE_OPENGL;
	os_cursor = 0;

	// [login]
	account = "";
	password = "";

	// [client]
	skin = "default";
	motdtext = "No current information.";
	motdnum = 0;
	ui_compat = false;
	lang = "";

	// [general]
	classiccontrol = 0;
	autochase = 1;
	showhints = 1;
	shownames = 1;
	showtexteffects = 1;

	// [graphics]
	fullscreen = 0;
	w = 640;
	h = 480;
	bpp = 32;
	maxfps = 0;

	// [console]
	infomsgs = true;
	eventmsgs = true;
	statusmsgs = true;
	timestamps = true;
	levels = true;
	privatemsgs = true;

	// [network]
	server = "change.me.now";
	port = 7171;
	otkey = true;
	protocol = CLIENT_VERSION_AUTO;
	overrideversion = 0;



#ifdef WINCE
	w = 240;
	h = 320;
#endif
}

Options::~Options()
{
	delete configHandler;
}

void Options::Save()
{
	//Update all the values for saving
	std::stringstream ss;

	Section *section;

	section = configHandler->newSection("window");
	ss << engine;
	section->addKey("engine", ss.str());
	ss.str("");
	ss << os_cursor;
	section->addKey("os_cursor", ss.str());
	ss.str("");


	section = configHandler->newSection("login");
	ss << account;
	section->addKey("account", ss.str());
	ss.str("");
	ss << password;
	section->addKey("password", ss.str());
	ss.str("");

	section = configHandler->newSection("client");
	ss << skin;
	section->addKey("skin", ss.str());
	ss.str("");
	ss << motdnum;
	section->addKey("motdnum", ss.str());
	ss.str("");
	ss << motdtext;
	section->addKey("motdtext", ss.str());
	ss.str("");
	ss << ui_compat;
	section->addKey("ui_compat", ss.str());
	ss.str("");
	ss << lang;
	section->addKey("lang", ss.str());
	ss.str("");


	section = configHandler->newSection("general");
	ss << classiccontrol;
	section->addKey("classiccontrol", ss.str());
	ss.str("");
	ss << autochase;
	section->addKey("autochase", ss.str());
	ss.str("");
	ss << showhints;
	section->addKey("showhints", ss.str());
	ss.str("");
	ss << shownames;
	section->addKey("shownames", ss.str());
	ss.str("");
	ss << showtexteffects;
	section->addKey("showtexteffects", ss.str());
	ss.str("");


	section = configHandler->newSection("graphics");
	ss << fullscreen;
	section->addKey("fullscreen", ss.str());
	ss.str("");
	ss << w;
	section->addKey("width", ss.str());
	ss.str("");
	ss << h;
	section->addKey("height", ss.str());
	ss.str("");
	ss << bpp;
	section->addKey("bpp", ss.str());
	ss.str("");
	ss << maxfps;
	section->addKey("maxfps", ss.str());
	ss.str("");


	section = configHandler->newSection("console");
	ss << infomsgs;
	section->addKey("infomsgs", ss.str());
	ss.str("");
	ss << eventmsgs;
	section->addKey("eventmsgs", ss.str());
	ss.str("");
	ss << statusmsgs;
	section->addKey("statusmsgs", ss.str());
	ss.str("");
	ss << timestamps;
	section->addKey("timestamps", ss.str());
	ss.str("");
	ss << levels;
	section->addKey("levels", ss.str());
	ss.str("");
	ss << privatemsgs;
	section->addKey("privatemsgs", ss.str());
	ss.str("");

	section = configHandler->newSection("network");
	ss << server;
	section->addKey("server", ss.str());
	ss.str("");
	ss << port;
	section->addKey("port", ss.str());
	ss.str("");
	ss << otkey;
	section->addKey("otkey", ss.str());
	ss.str("");
	ss << protocol;
	section->addKey("protocol", ss.str());
	ss.str("");
	ss << overrideversion;
	section->addKey("overrideversion", ss.str());
	ss.str("");

	configHandler->saveConfig("yatc.cfg");
}

void Options::Load()
{
	// vars in case of need of additional parsin
	int protocol_int;

	if(!configHandler->loadConfig("yatc.cfg")){
		return;
	}

	// [window]
	switch((enginelist_t)atoi(configHandler->getKeyValue("window", "engine").c_str())){
		case ENGINE_SDL:
				engine = ENGINE_SDL;
			break;
		case ENGINE_OPENGL:
				engine = ENGINE_OPENGL;
			break;
		case ENGINE_DIRECTX:
				engine = ENGINE_DIRECTX;
			break;
		default:
				engine = ENGINE_SDL;
			break;
	}
	os_cursor = (atoi(configHandler->getKeyValue("window", "os_cursor").c_str()) == 1);

	// [login]
	account = configHandler->getKeyValue("login", "account");
	password = configHandler->getKeyValue("login", "password");

	// [client]
	skin = configHandler->getKeyValue("client", "skin");
	motdnum = atoi(configHandler->getKeyValue("client", "motdnum").c_str());
	motdtext = configHandler->getKeyValue("client", "motdtext");
	ui_compat = (atoi(configHandler->getKeyValue("client", "ui_compat").c_str()) == 1);
	lang = configHandler->getKeyValue("client", "lang");

	// [general]
	classiccontrol = (atoi(configHandler->getKeyValue("general", "classiccontrol").c_str()) == 1);
	autochase = (atoi(configHandler->getKeyValue("general", "autochase").c_str()) == 1);
	showhints = (atoi(configHandler->getKeyValue("general", "showhints").c_str()) == 1);
	shownames = (atoi(configHandler->getKeyValue("general", "shownames").c_str()) == 1);
	showtexteffects = (atoi(configHandler->getKeyValue("general", "showtexteffects").c_str()) == 1);

	// [graphics]
	fullscreen = (atoi(configHandler->getKeyValue("graphics", "fullscreen").c_str()) == 1);
	w = atoi(configHandler->getKeyValue("graphics", "width").c_str());
	h = atoi(configHandler->getKeyValue("graphics", "height").c_str());
	bpp = atoi(configHandler->getKeyValue("graphics", "bpp").c_str());
	maxfps = atoi(configHandler->getKeyValue("graphics", "maxfps").c_str());

	// [console]
    infomsgs = (atoi(configHandler->getKeyValue("console", "infomsgs").c_str()) == 1);
    eventmsgs = (atoi(configHandler->getKeyValue("console", "eventmsgs").c_str()) == 1);
    statusmsgs = (atoi(configHandler->getKeyValue("console", "statusmsgs").c_str()) == 1);
    timestamps = (atoi(configHandler->getKeyValue("console", "timestamps").c_str()) == 1);
    levels = (atoi(configHandler->getKeyValue("console", "levels").c_str()) == 1);
    privatemsgs = (atoi(configHandler->getKeyValue("console", "privatemsgs").c_str()) == 1);

	// [network]
	server = configHandler->getKeyValue("network", "server");
	port = atoi(configHandler->getKeyValue("network", "port").c_str());
    otkey = (atoi(configHandler->getKeyValue("network", "otkey").c_str()) == 1);
    protocol_int = atoi(configHandler->getKeyValue("network", "protocol").c_str());
    overrideversion = atoi(configHandler->getKeyValue("network", "overrideversion").c_str());


    // further parsing of some options
    // [network]
    switch (protocol_int) {
		default:
		case 0:
			protocol = CLIENT_VERSION_AUTO;
			break;
    	case 800:
			protocol = CLIENT_VERSION_800;
			break;
		case 810:
			protocol = CLIENT_VERSION_810;
			break;
		case 811:
			protocol = CLIENT_VERSION_811;
			break;
		case 820:
			protocol = CLIENT_VERSION_820;
			break;
		case 821:
			protocol = CLIENT_VERSION_821;
			break;
        case 822:
            protocol = CLIENT_VERSION_822;
            break;
        case 830:
            protocol = CLIENT_VERSION_830;
            break;
        case 831:
            protocol = CLIENT_VERSION_831;
            break;
        case 840:
            protocol = CLIENT_VERSION_840;
            break;
        case 841:
            protocol = CLIENT_VERSION_841;
            break;
        case 842:
            protocol = CLIENT_VERSION_842;
            break;
    }

	configHandler->clear();
}

