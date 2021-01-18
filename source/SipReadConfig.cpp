#include "SipReadConfig.h"
#include <string>
#include <regex>
#include <sstream>
#include <fstream>
using namespace std;
bool ReadConfig::read(unordered_map<string, string*> &map,  std::istream& sstream)
{
	string line;
	while (!sstream.eof())
	{
		getline(sstream, line);
		
		for (auto i : map) 
		{
			if (line.rfind(i.first, 0) == 0)
			{

				std::smatch matched;
				string regex_message = i.first + "\\s*=\\s*(.*?)\\s*";
				std::regex message(regex_message);
				if (regex_match(line, matched, message))
				{
					*(map[i.first] )= matched[1];
				}
			}
		}
	}
	return true;
}

//---------------------------------------------------------------------
SipReadConfig::SipReadConfig()  {};

bool SipReadConfig::read(const string& path, unordered_map< string,  string*>& map)
{
	bool res;
	ifstream f(path);
	res = ReadConfig::read(map, f);
	return res;
}