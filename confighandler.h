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


#ifndef __CONFIGHANDLER_H__
#define __CONFIGHANDLER_H__

#include <iostream>
#include <string>
#include <vector>

struct Key
{
	std::string name;
	std::string value;
	
	Key(std::string _name, std::string _value)
	{
		name = _name;
		value = _value;	
	}	
};

typedef std::vector<Key*> KeyVector;

struct Section
{
	std::string name;
	KeyVector keys;
	
	Section(std::string _name)
	{
		name = _name;	
	}
	Section(){ };
	
	void addKey(std::string _name, std::string _value)
	{
		Key* key = new Key(_name, _value);
		keys.push_back(key);	
	}
};

typedef std::vector<Section*> SectionVector;

class ConfigHandler
{
public:
	ConfigHandler();
	~ConfigHandler();
	
	void clear();
		
	bool loadConfig(const char* filename);
	bool saveConfig(const char* filename);
	
	bool readSection(const char* buffer, int& currentSection);
	void readKey(const char* buffer, int currentSection);
	
	std::string getKeyValue(const std::string section, const std::string keyname);
	
	Section* newSection(std::string val){
		Section* section = new Section(val);
		sections.push_back(section);
		
		return section;	
	};
	
	SectionVector::iterator getSectionsBegin(){return sections.begin();};
	SectionVector::iterator getSectionsEnd(){return sections.end();};
	
	SectionVector sections;
};

extern ConfigHandler* configHandler;

#endif
