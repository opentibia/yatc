//////////////////////////////////////////////////////////////////////
// Yet Another Tibia Client
//////////////////////////////////////////////////////////////////////
// Stores and reads configuration files
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

#include <string.h>
#include <stdio.h>
#include "confighandler.h"
#include "util.h"
#include "product.h"
ConfigHandler* configHandler = NULL;

ConfigHandler::ConfigHandler()
{

}

ConfigHandler::~ConfigHandler()
{
	clear();
}

void ConfigHandler::clear()
{
   for(SectionVector::iterator it = sections.begin(); it != sections.end(); it++){
		for(KeyVector::iterator cit = (*it)->keys.begin(); cit != (*it)->keys.end(); cit++){
			delete (*cit);
		}

		(*it)->keys.clear();
		delete (*it);
	}

	sections.clear();
}

bool ConfigHandler::loadConfig(const char* filename)
{
	FILE* f = yatc_fopen(filename, "rb");

	if(!f){
		return false;
	}

	//bool sector = false;
	int currentSection = 0;
    
	while(!feof(f)){
        char read_buffer[2048] = {0};
        char *buffer = fgets(read_buffer, sizeof read_buffer, f);
        if (!buffer)
            break;
        
		if(buffer[0] != '#'){ // Not a comment: process it
            if(!readSection(buffer, currentSection)) { // Attempt to process as a section
                readKey(buffer, currentSection);
            }
		}
	}

	currentSection = 0;

	if(f)
		fclose(f);

	return true;
}

bool ConfigHandler::saveConfig(const char* filename)
{
	FILE* f = yatc_fopen(filename, "wb");

	if(!f){
		return false;
	}

	fprintf(f, "%s", "# This is the config file for " PRODUCTLONGNAME "\r\n");
	fprintf(f, "%s", "# Do not edit unless you know what you are doing!\r\n\r\n");

   	for(SectionVector::iterator it = sections.begin(); it != sections.end(); it++){
		fprintf(f, "[%s]\r\n", (*it)->name.c_str());
		for(KeyVector::iterator cit = (*it)->keys.begin(); cit != (*it)->keys.end(); cit++){
		    std::string val = (*cit)->value;
		    val = str_replace("\n", "<br>", val);
			fprintf(f, "%s=\"%s\"\r\n", (*cit)->name.c_str(), val.c_str());
		}

		fprintf(f, "\r\n");
	}

	if(f)
		fclose(f);

	clear();

	return true;
}

bool ConfigHandler::readSection(const char* buffer, int& currentSection)
{
	if(strstr(buffer, "[") && strstr(buffer, "]")){
		std::string str(buffer);

		int open = str.find_first_of("[");
		int close = str.find_first_of("]");
		int len = close - open - 1;
		std::string sectionName = str.substr(open+1, len);

		newSection(sectionName);
		currentSection = sections.size() - 1;

		return true;
	}

	return false;
}

void ConfigHandler::readKey(const char* buffer, int currentSection)
{
	if(strstr(buffer, "=")){
		Section* section = sections[currentSection];

		std::string str(buffer);

		int eq  = str.find_first_of("=");
		std::string keyname = str.substr(0, eq);
		int len = str.size() - eq - 2;
		std::string value = str.substr(eq + 2, len-1);

        value = str_replace("<br>", "\n", value);

		Key* key = new Key(keyname, value);
		section->keys.push_back(key);
	}
}

bool ConfigHandler::keyExists(const std::string section, const std::string keyname)
{
	for(SectionVector::iterator it = sections.begin(); it != sections.end(); it++){
		if((*it)->name == section){
			for(KeyVector::iterator cit = (*it)->keys.begin(); cit != (*it)->keys.end(); cit++){
				if((*cit)->name == keyname)
					return true;
			}
		}
	}


	return false;
}

std::string ConfigHandler::getKeyValue(const std::string section, const std::string keyname)
{
	for(SectionVector::iterator it = sections.begin(); it != sections.end(); it++){
		if((*it)->name == section){
			for(KeyVector::iterator cit = (*it)->keys.begin(); cit != (*it)->keys.end(); cit++){
				if((*cit)->name == keyname)
					return (*cit)->value;
			}
		}
	}


	return "";
}
