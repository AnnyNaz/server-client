#ifndef SIPREADCONFIG_H
#define SIPREADCONFIG_H
#include <string>
#include <unordered_map>
class ReadConfig
{
public:
	static bool read(std::unordered_map<std::string, std::string*>& map, std::istream& sstream);
};
class SipReadConfig:ReadConfig 
{
	public:
		SipReadConfig();
		bool read(const std::string& path, std::unordered_map<std::string, std::string*>&  map);
};

#endif