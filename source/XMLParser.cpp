#include "XMLParser.h"
#include "tinyxml2/tinyxml2.h"
#define ROOT_NODE "scenario"
using namespace std;
using namespace tinyxml2;
void XMLParser::parse(const char* path)
{
	string text, text1;
	XMLDocument doc;
	//UDPServer* connector = new UDPServer(this_port);
	UDPClient* connector = new UDPClient(ip, this_port);
	connector->connectTo(ip, other_port);
	SipScenario UACScenario(connector);
	if (doc.LoadFile(path) != 0)
	{
		cout << "load xml file failed";
		return;
	}
	tinyxml2::XMLElement* rootNode = doc.FirstChildElement(ROOT_NODE);
	tinyxml2::XMLElement* node = rootNode->FirstChildElement();

	SipInitContext* init_context = new SipInitContext(&UACScenario, this_ip, this_port, other_ip, other_port, local_ip_type,
		media_ip_type, media_ip,
		media_port, rtcp_port, TRANSPORT, SERVICE, from_name);
	UACScenario.addAction(init_context);
	while (NULL != node)
	{
		if (strcmp("send", node->Name()) == 0)
		{
			string* str = new string(node->GetText());
			str->erase(0, 1);
			SendSipRequest* sendsiprequest = new SendSipRequest((*str));

			UACScenario.addAction(sendsiprequest);
			cout << str;
			sendsiprequest->m_scenario = &UACScenario;
		}
		if (strcmp("recv", node->Name()) == 0)
		{
			if (node->Attribute("response"))
			{
				string response = node->Attribute("response");
				ReceiveSipRequest* receivesiprequest = NULL;
				if (response == "200")
					receivesiprequest = new ReceiveSipRequest(OK);
				else
					if (response == "100")
						receivesiprequest = new ReceiveSipRequest(TRY);
					else if (response == "183")
						receivesiprequest = new ReceiveSipRequest(SESSION_PROGRESS);
					else if (response == "180")
						receivesiprequest = new ReceiveSipRequest(RINGNG);
				if (receivesiprequest)
				{
					UACScenario.addAction(receivesiprequest);
					receivesiprequest->m_scenario = &UACScenario;
					cout << text << "recv" << endl;
				}
			}
			else if (node->Attribute("request"))
			{
				string request = node->Attribute("request");
				ReceiveSipRequest* receivesiprequest = NULL;
				if (request == "INVITE")
					receivesiprequest = new ReceiveSipRequest(INVITE);
				if (request == "OK")
					receivesiprequest = new ReceiveSipRequest(OK);
				else if (request == "ACK")
					receivesiprequest = new ReceiveSipRequest(ACK);
				else if (request == "BYE")
					receivesiprequest = new ReceiveSipRequest(BYE);
				if (receivesiprequest)
				{
					UACScenario.addAction(receivesiprequest);
					receivesiprequest->m_scenario = &UACScenario;
					cout << text << "recv" << endl;
				}
			}
		}

		if (strcmp("pause", node->Name()) == 0)
		{
			Pause* p = new Pause(12);
			UACScenario.addAction(p);
			cout << "pause" << endl;
		}
		node = node->NextSiblingElement();
	}

	UACScenario.run();
}