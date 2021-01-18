#ifndef SIPTESTERCONFIG_H
#define SIPTESTERCONFIG_H
#include "XMLParser.h"
#include "SIPReadConfig.h"
#include <string>

class SipTesterConfig
{
public:
	static SipTesterConfig* instance();
	
	static bool run(const std::string& name_scenario);
	
protected:
	static  SipTesterConfig* tester;
	SipTesterConfig();
	static bool init(const std::string& path);
	static std::string from_name;
	static std::string ip;
	static std::string this_ip;
	static std::string other_ip;
	static int other_port;
	static int this_port;
	static std::string SERVICE;
	static std::string TRANSPORT;
	static int length_of_code;
	static std::string local_ip_type;
	static std::string media_port;
	static std::string media_ip_type;
	static std::string rtcp_port;
	static std::string media_ip;

};

#endif