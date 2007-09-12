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

Options options;

Options::Options()
{
	maptrack = 0;
	fullscreen = 0;
	engine = ENGINE_SDL;
	os_cursor = 0;
	skin = "default";
}

Options::~Options()
{

}

void Options::Save()
{
	dbExecPrintf(dbUser, 0, 0, 0, "begin transaction;");

	// bools
	dbSaveSetting("maptrack", maptrack ? "1" : "0");
	dbSaveSetting("fullscreen", fullscreen ? "1" : "0");
	dbSaveSetting("os_cursor", os_cursor ? "1" : "0");

	// integers
	std::stringstream s;
	s << engine;
	dbSaveSetting("engine", s.str().c_str() );
	s.str("");

	// strings
	dbSaveSetting("skin", skin.c_str());

	dbExecPrintf(dbUser, 0, 0, 0, "end transaction;");
}

void Options::Load()
{
	char tmp[256];

	// bools
	dbLoadSetting("maptrack", tmp, 256, "0");
	if(tmp[0]=='1')
		maptrack = true;
	else
		maptrack = false;

	dbLoadSetting("fullscreen", tmp, 256, "0");
	if(tmp[0]=='1')
		fullscreen = true;
	else
		fullscreen = false;

	dbLoadSetting("os_cursor", tmp, 256, "0");
	if(tmp[0]=='1')
		os_cursor = true;
	else
		os_cursor = false;


	// integers
	dbLoadSetting("engine", tmp, 256, "1");
	engine = (enginelist_t)atoi(tmp);

	// strings
	dbLoadSetting("skin", tmp, 256, "default");
	skin = tmp;
}
