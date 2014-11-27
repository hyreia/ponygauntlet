#ifndef CONFIG_FILE_HPP
#define CONFIG_FILE_HPP

#include <string>

struct ALLEGRO_CONFIG;

/* RAII-based struct for automatically closing config files after use. 
Also used for more complex objects that don't want a file interface but are config-based */
class ConfigFile
{
public:
	ConfigFile(const char *filename);
	ConfigFile();

	bool Open(const char *filename);
	void CreateEmpty();
	void Close();
	void Save();

	bool IsOpen();

	void SetValue(const char *section, const char *key, const char *value);
	void SetValue(const char *section, const char *key, int value);

	const char* GetValue(const char *section, const char *key);
	int GetIntValue(const char *section, const char *key);
	bool GetBooleanValue(const char *section, const char *key);

		

	~ConfigFile();

protected:
	ALLEGRO_CONFIG *configFile;
	std::string filename;
};

#endif