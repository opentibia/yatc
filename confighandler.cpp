#include "confighandler.h"

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
	FILE* f = fopen(filename, "rb");
	
	if(!f){
		return false;	
	}
	
	fseek(f,0,SEEK_END);
	int size = ftell(f);
	fseek(f, 0, SEEK_SET);
	
	char* content = new char[size];
	fread(content, 1, size, f);
	
	bool sector = false;
	int currentSection = 0;
	int failsafe = 0;
	
	char* buffer = strtok(content, "\r\n");
	while(buffer != 0){
		buffer[strlen(buffer)] = '\0';
		
		if(strncmp(buffer, "##", 2) == 0){ //Comment, ignore the line
			buffer = strtok(content,"\r\n");
			continue;
		}
		
		if(!readSection(buffer, currentSection)){
			readKey(buffer, currentSection);
		}
		
		buffer = strtok(0, "\r\n");
		
		if(failsafe > size)
			break;
	}
		
	delete[] content;
	delete buffer;
	currentSection = 0;
		
	if(f)
		fclose(f);
		
	return true;
}

bool ConfigHandler::saveConfig(const char* filename)
{
	FILE* f = fopen(filename, "wb");
	
	if(!f){
		return false;	
	}
	
	fprintf(f, "%s", "# This is the config file for the YATC Client\r\n");
	fprintf(f, "%s", "# Do not edit unless you know what you are doing!\r\n\r\n");

   	for(SectionVector::iterator it = sections.begin(); it != sections.end(); it++){
		fprintf(f, "[%s]\r\n", (*it)->name.c_str());
		for(KeyVector::iterator cit = (*it)->keys.begin(); cit != (*it)->keys.end(); cit++){
			fprintf(f, "%s=\"%s\"\r\n", (*cit)->name.c_str(), (*cit)->value.c_str());
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
		
		Key* key = new Key(keyname, value);
		section->keys.push_back(key);
	}
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
