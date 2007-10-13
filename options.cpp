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

#include <string>
#include <sstream>
#include "options.h"
#include "confighandler.h"

Options options;

Options::Options()
{
	configHandler = new ConfigHandler();

	engine = ENGINE_SDL;
	os_cursor = 0;
	skin = "default";
	server = "change.me.now";
	port = 7171;

	classiccontrol = 0;
	autochase = 1;
	showhints = 1;
	shownames = 1;
	showtexteffects = 1;

	fullscreen = 0;
	w = 640;
	h = 480;
	bpp = 16;

	motdtext = "No current information.";
	motdnum = 0;
}

Options::~Options()
{
	delete configHandler;
}

void Options::Save()
{
	//Update all the values for saving
	std::stringstream ss;
	Section* section = configHandler->newSection("window");
	ss << engine;
	section->addKey("engine", ss.str());
	ss.str("");
	ss << os_cursor;
	section->addKey("os_cursor", ss.str());
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


	section = configHandler->newSection("network");
	ss << server;
	section->addKey("server", ss.str());
	ss.str("");
	ss << port;
	section->addKey("port", ss.str());
	ss.str("");

	configHandler->saveConfig("yatc.cfg");
}

void Options::Load()
{
	if(!configHandler->loadConfig("yatc.cfg")){
		return;
	}

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

	skin = configHandler->getKeyValue("client", "skin");
	motdnum = atoi(configHandler->getKeyValue("client", "motdnum").c_str());
	motdtext = configHandler->getKeyValue("client", "motdtext");

	classiccontrol = (atoi(configHandler->getKeyValue("general", "classiccontrol").c_str()) == 1);
	autochase = (atoi(configHandler->getKeyValue("general", "autochase").c_str()) == 1);
	showhints = (atoi(configHandler->getKeyValue("general", "showhints").c_str()) == 1);
	shownames = (atoi(configHandler->getKeyValue("general", "shownames").c_str()) == 1);
	showtexteffects = (atoi(configHandler->getKeyValue("general", "showtexteffects").c_str()) == 1);


	fullscreen = (atoi(configHandler->getKeyValue("graphics", "fullscreen").c_str()) == 1);
	w = atoi(configHandler->getKeyValue("graphics", "width").c_str());
	h = atoi(configHandler->getKeyValue("graphics", "height").c_str());
	bpp = atoi(configHandler->getKeyValue("graphics", "bpp").c_str());


	server = configHandler->getKeyValue("network", "server");
	port = atoi(configHandler->getKeyValue("network", "port").c_str());

	configHandler->clear();
}

