#include "XMLParser.h"
#include "tinyxml2/tinyxml2.h"
#include <boost/asio/ip/address_v4.hpp>
#define ROOT_NODE "scenario"
using namespace std;
using namespace tinyxml2;
ReceiveSipRequest* XMLParser::factorymethodReceiveSipRequest(const string& response)
{
	ReceiveSipRequest* receivesiprequest = NULL;
	if (response == "200")
		receivesiprequest = new ReceiveSipRequest(OK);
	else if (response == "100")
		receivesiprequest = new ReceiveSipRequest(TRY);
	else if (response == "183")
		receivesiprequest = new ReceiveSipRequest(SESSION_PROGRESS);
	else if (response == "180")
		receivesiprequest = new ReceiveSipRequest(RINGNG);
	else if (response == "INVITE")
		receivesiprequest = new ReceiveSipRequest(INVITE);
	else if (response == "OK")
		receivesiprequest = new ReceiveSipRequest(OK);
	else if (response == "ACK")
		receivesiprequest = new ReceiveSipRequest(ACK);
	else if (response == "BYE")
		receivesiprequest = new ReceiveSipRequest(BYE);
	return receivesiprequest;
};

bool XMLParser::parse(const string&  pathstr, SipScenario* ParsedScenario)
{
	string text, text1;
	XMLDocument doc;
	
	const char* path = pathstr.c_str();;
	if (doc.LoadFile(path) != 0)
	{
		cout << "load xml file failed";
		return false;
	}
	tinyxml2::XMLElement* rootNode = doc.FirstChildElement(ROOT_NODE);
	if (rootNode == NULL)
		return false;
	tinyxml2::XMLElement* node = rootNode->FirstChildElement();
	if (node == NULL)
		return false;
	
	
	while (NULL != node)
	{
		if (strcmp("send", node->Name()) == 0)
		{
			string* str = new string(node->GetText());
			str->erase(0, 1);
			SendSipRequest* sendsiprequest = new SendSipRequest((*str));

			ParsedScenario->addAction(sendsiprequest);
			sendsiprequest->m_scenario = ParsedScenario;
		}
		if (strcmp("recv", node->Name()) == 0)
		{
			if (node->Attribute("response"))
			{
				string response = node->Attribute("response");
				ReceiveSipRequest* receivesiprequest = factorymethodReceiveSipRequest(response);
				if (receivesiprequest)
				{
					ParsedScenario->addAction(receivesiprequest);
					receivesiprequest->m_scenario = ParsedScenario;
				}
			}
			else if (node->Attribute("request"))
			{
				string request = node->Attribute("request");
				ReceiveSipRequest* receivesiprequest = factorymethodReceiveSipRequest(request);
				if (receivesiprequest)
				{
					ParsedScenario->addAction(receivesiprequest);
					receivesiprequest->m_scenario = ParsedScenario;
				}
			}
		}

		if (strcmp("pause", node->Name()) == 0)
		{
			if (node->Attribute("milliseconds"))
			{






				string request = node->Attribute("milliseconds");
				Pause* p = new Pause(stoi(request));
				ParsedScenario->addAction(p);
			}
		}
		node = node->NextSiblingElement();
	}
	return ParsedScenario;
}