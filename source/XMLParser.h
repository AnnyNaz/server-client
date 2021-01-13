#pragma once
#include <string>
#include "UdpServer.h"
#include "UdpClient.h"
#include "SipScenario.h"
const std::string from_name = "CppSip";
const std::string ip = "192.168.0.106";
const std::string this_ip = "192.168.0.106";
const std::string other_ip = "192.168.0.106";
const int other_port = 63250;
const int this_port = 5100;
const std::string SERVICE = "test";
const std::string TRANSPORT = "UDP";
const int length_of_code = 42;
const std::string local_ip_type = "4";
const std::string media_port = "12067";
const std::string media_ip_type = "4";
const std::string rtcp_port = "12068";
const std::string media_ip = "192.168.0.106";
class XMLParser 
{
public:
	void parse(const char* path);
private:
	const unsigned int m_seconds = 5;
};