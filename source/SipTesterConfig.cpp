#include "SipTesterConfig.h"
using namespace std;
std::string SipTesterConfig::from_name = "";
std::string SipTesterConfig::ip = "";
std::string SipTesterConfig::this_ip = "";
std::string SipTesterConfig::other_ip = "";
int SipTesterConfig::other_port =0;
int SipTesterConfig::this_port = 0;
std::string SipTesterConfig::SERVICE = "";
std::string SipTesterConfig::TRANSPORT = "";
int SipTesterConfig::length_of_code = 42;
std::string SipTesterConfig::local_ip_type = "";
std::string SipTesterConfig::media_port = "";
std::string SipTesterConfig::media_ip_type = "";
std::string SipTesterConfig::rtcp_port = "";
std::string SipTesterConfig::media_ip = "";
SipTesterConfig* SipTesterConfig::tester = NULL;
SipTesterConfig::SipTesterConfig() {};

SipTesterConfig *SipTesterConfig::instance()
{
	if (tester == NULL)
		tester = new SipTesterConfig();
	return tester;
};

bool SipTesterConfig::init(const string& path)
{
	bool res = false;
	SipReadConfig settings;
	std::unordered_map<std::string, std::string*> map;
	std::string SERVICE = "test";
	std::string TRANSPORT = "UDP";
	string this_portstr, other_portstr, length_of_codestr;
	map["this_port"] = &this_portstr;
	map["other_port"] = &other_portstr;
	map["from_name"] = &SipTesterConfig::from_name;
	map["ip"] = &SipTesterConfig::ip;
	map["this_ip"] = &SipTesterConfig::this_ip;
	map["other_ip"] = &SipTesterConfig::other_ip;
	map["this_port"] = &this_portstr;
	map["SERVICE"] = &SipTesterConfig::SERVICE;
	map["TRANSPORT"] = &SipTesterConfig::TRANSPORT;
	map["length_of_code"] = &length_of_codestr;
	map["local_ip_type"] = &SipTesterConfig::local_ip_type;
	map["media_port"] = &SipTesterConfig::media_port;
	map["media_ip_type"] = &SipTesterConfig::media_ip_type;
	map["rtcp_port"] = &SipTesterConfig::rtcp_port;
	map["media_ip"] = &SipTesterConfig::media_ip;
	res = settings.read(path, map);
	SipTesterConfig::this_port = stoi(this_portstr);
	SipTesterConfig::other_port = stoi(other_portstr);
	SipTesterConfig::length_of_code = stoi(length_of_codestr);
	return res;
};

bool SipTesterConfig::run(const std::string& name_scenario)
{
	std::string path_test_config = "..\\test_config.ini";
	if (init(path_test_config))
	{
		SipScenario* ParsedScenario = new SipScenario();
		SipInitContext* init_context = new SipInitContext(ParsedScenario, this_ip, this_port, other_ip, other_port, local_ip_type,
			media_ip_type, media_ip,
			media_port, rtcp_port, TRANSPORT, SERVICE, from_name);
		ParsedScenario->addAction(init_context);
		XMLParser parser;
		string path = "..\\source\\scenarios\\" + name_scenario + ".xml";
		if (parser.parse(path, ParsedScenario) && ParsedScenario != NULL)
			ParsedScenario->run();
		else
			return false;
		return true;
	}
	return false;

};

