#ifndef XMLPARSER_H
#define XMLPARSER_H
#include <string>
#include "UdpServer.h"
#include "UdpClient.h"
#include "SipScenario.h"

class XMLParser 
{

public:
	static bool XMLParser::parse(const std::string& path, SipScenario* ParsedScenario);
private:
	static ReceiveSipRequest* factorymethodReceiveSipRequest(const std::string& response);
};
#endif