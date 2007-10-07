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
	fullscreen = 0;
	engine = ENGINE_SDL;
	os_cursor = 0;
	skin = "default";

	motdtext = "No message of the day retrieved yet.";
	motdnum = 0;

	server = "change.me.now";
	port = 7171;
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
	ss << fullscreen;
	section->addKey("fullscreen", ss.str());
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
		return false;	
	}
	
	switch((enginelist_t)atoi(configHandler->getKeyValue("window", "engine").c_str())){
		case ENGINE_SDL:
				setEngine(ENGINE_SDL);
			break;
		case ENGINE_OPENGL:
				setEngine(ENGINE_OPENGL);
			break;
		case ENGINE_DIRECTX:
				setEngine(ENGINE_DIRECTX);
			break;
		default:
				setEngine(ENGINE_SDL);
			break;	
	}
	
	switch(atoi(configHandler->getKeyValue("window", "fullscreen").c_str())){
		case 0:
				fullscreen = false;
			break;
		case 1:
				fullscreen = true;
			break;
		default:
				fullscreen = false;
			break;
	}
	
	switch(atoi(configHandler->getKeyValue("window", "os_cursor").c_str())){
		case 0:
				os_cursor = false;
			break;
		case 1:
				os_cursor = true;
			break;
		default:
				os_cursor = false;
			break;
	}
	
	skin = configHandler->getKeyValue("client", "skin");
	motdnum = configHandler->getKeyValue("client", "motdnum");
	motdtext = configHandler->getKeyValue("client", "motdtext");
	
	server = configHandler->getKeyValue("network", "server");
	port = atoi(configHandler->getKeyValue("network", "port").c_str());
	
	configHandler->clear();
}
