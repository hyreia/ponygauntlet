#include <sstream>
#include <allegro5/allegro.h>

#include "ConfigFile.hpp"


ConfigFile::ConfigFile(const char *file)
{
	configFile = NULL;

	this->filename = file;

	configFile = al_load_config_file(file);
}

ConfigFile::ConfigFile():
configFile(NULL)
{}

bool ConfigFile::Open(const char *file)
{
	filename = file;
	if(configFile)
		al_destroy_config(configFile);

	configFile = al_load_config_file(filename.c_str());

	if(configFile) return true;
	else return false;
}

void ConfigFile::CreateEmpty()
{
	if(IsOpen())
	{
		Close();
	}
	configFile = al_create_config();
}

void ConfigFile::Close()
{
	if(configFile)
		al_destroy_config(configFile);
}

void ConfigFile::Save()
{
	//Don't bother testing if it saved successfully
	al_save_config_file(filename.c_str(), configFile);
}

bool ConfigFile::IsOpen()
{
	if(configFile) return true;
	else return false;
}

ConfigFile::~ConfigFile()
{
	Close();
}

void ConfigFile::SetValue(const char *section, const char *key, const char *value)
{
	if(configFile) al_set_config_value(configFile, section, key, value);
}

void ConfigFile::SetValue(const char *section, const char *key, int value)
{
	std::stringstream ss;
	ss << value;
	std::string str = ss.str(); //intermediate because ss.str().c_str() doesn't seem to work
	const char *cvalue = str.c_str();

	SetValue(NULL, key, cvalue);
}

const char *ConfigFile::GetValue(const char *section, const char *key)
{
	if(configFile)
		return al_get_config_value(configFile, section, key);
	else return NULL;
}

int ConfigFile::GetIntValue(const char *section, const char *key)
{
	const char *cvalue = GetValue(section, key);
	if(cvalue)
	{
		int ivalue = atoi(cvalue);
		return ivalue;
	}
	else return 0;
}

bool ConfigFile::GetBooleanValue(const char *section, const char *key)
{
	const char *bvalue = GetValue(section, key);
	if(bvalue)
	{
		return strcmp(GetValue(section, key), "true") == 0? true: false;
	}
	else return false;

	
}