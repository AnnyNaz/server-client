#include <thread>
#include "UdpServer.h"
#include "UdpClient.h"
#include "SipScenario.h"
#include <string>
#include <sstream>
#include <iostream>
#include <thread>         
#include <chrono>
#include "tinyxml2/tinyxml2.h"
using namespace tinyxml2;
using namespace std;
#define ROOT_NODE "scenario"


unsigned int seconds = 5;
const string from_name = "CppSip";
const string ip = "127.0.0.1";
const string this_ip = "127.0.0.1";
const string other_ip = "127.0.0.1";
const int other_port = 5060;
const int this_port = 5061;
const int SERVICE = 7887;
const string TRANSPORT = "UDP";
const int length_of_code = 32;
string local_ip_type = "4";
string media_port = "12001";
string media_ip_type = "4";
string media_ip = "192.168.1.13";


/*void UASSenario() 
{
	UDPServer* connector = new UDPServer(this_port);
	SipScenario UASScenario(connector);
	SipAction* act1 = new ReceiveSipRequest(INVITE);
	//SipAction* act2 = new SendSipRequest(RINGNG);
	SipAction* act3 = new SendSipRequest(OK);
	SipAction* act4 = new ReceiveSipRequest(ACK);
	SipAction* act5 = new SendSipRequest(BYE);
	SipAction* act6 = new ReceiveSipRequest(OK);
	UASScenario.addAction(act1);
	//UASScenario.addAction(act2);
	UASScenario.addAction(act3);
	UASScenario.addAction(act4);
	UASScenario.addAction(act5);
	UASScenario.addAction(act6);
	UASScenario.run();
}*/
/*void UACSenario()
{
	UDPClient* connector = new UDPClient(ip, this_port);
	connector->connectTo(ip, other_port);
	SipScenario UACScenario(connector);
	(UACScenario.getContext())->last_message = inviteMessage(TRANSPORT, SERVICE, from_name, ip, this_port, other_ip, other_port);
	SipAction* act1 = new SendSipRequest(INVITE);
	SipAction* act2 = new ReceiveSipRequest(RINGNG);
	SipAction* act3 = new ReceiveSipRequest(OK);
	SipAction* act_wait = new Pause(6);
	SipAction* act4 = new SendSipRequest(ACK);
	SipAction* act5 = new ReceiveSipRequest(BYE);
	SipAction* act6 = new SendSipRequest(OK);
	UACScenario.addAction(act1);
	UACScenario.addAction(act2);
	UACScenario.addAction(act3);	
	UACScenario.addAction(act4);
	UACScenario.addAction(act_wait);
	UACScenario.addAction(act5);
	UACScenario.addAction(act6);
	UACScenario.run();
}*/
std::string generateHexString(int length)
{

	std::stringstream ss;
	for (int i = 0; i < length; ++i)
	{
		ss << std::hex << ((rand() % 16));
	}
	std::string res(ss.str());
	return res;

}
void xmlUAS(const char* path)
{	
	string text, text1;
	XMLDocument doc;
	UDPClient* connector = new UDPClient(this_ip, this_port);
	connector->connectTo(other_ip, other_port);
	SipScenario UACScenario(connector);
	
	if (doc.LoadFile(path) != 0)
	{
		cout << "load xml file failed";
		return;
	}
	tinyxml2::XMLElement* rootNode = doc.FirstChildElement(ROOT_NODE);
	tinyxml2::XMLElement* node = rootNode->FirstChildElement();
	UACScenario.m_context = new SipContext();
	UACScenario.m_context->m_call_id = generateHexString(length_of_code);
	UACScenario.m_context->m_from_tag = generateHexString(length_of_code);
	UACScenario.m_context->m_via_branch = generateHexString(length_of_code);
	UACScenario.m_context->m_call_number = generateHexString(length_of_code);
	UACScenario.m_context->m_local_ip_type = local_ip_type;
	UACScenario.m_context->m_media_port = media_port;
	UACScenario.m_context->m_media_ip_type = media_ip_type;
	UACScenario.m_context->m_media_ip = media_ip;
	while (NULL != node)
	{
		if (strcmp("send", node->Name()) == 0)
		{
			//cout << node->GetText() << "recv" << endl;
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
				ReceiveSipRequest*  receivesiprequest = NULL;
				if (request == "INVITE")
					receivesiprequest = new ReceiveSipRequest(INVITE);
				if (request == "OK")
					receivesiprequest = new ReceiveSipRequest(OK);
				else if (request == "ACK")
					receivesiprequest = new ReceiveSipRequest(ACK);
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
void xmlUAC()
{
	string text, text1;
	XMLDocument doc;
	UDPServer* connector = new UDPServer(this_port);
	//connector->connectTo(other_ip, other_port);
	
	SipScenario UACScenario(connector);

	if (doc.LoadFile("..////scenarios/uac.xml") != 0)
	{
		cout << "load xml file failed";
		return;
	}
	tinyxml2::XMLElement* rootNode = doc.FirstChildElement(ROOT_NODE);
	assert(rootNode != 0);
	tinyxml2::XMLElement* node = rootNode->FirstChildElement();
	UACScenario.m_context = new SipContext();
	UACScenario.m_context->m_call_id = generateHexString(length_of_code);
	UACScenario.m_context->m_from_tag = generateHexString(length_of_code);
	UACScenario.m_context->m_via_branch = generateHexString(length_of_code);
	UACScenario.m_context->m_call_number = generateHexString(length_of_code);
	UACScenario.m_context->m_local_ip_type = local_ip_type;
	UACScenario.m_context->m_media_port = media_port;
	UACScenario.m_context->m_media_ip_type = media_ip_type;
	UACScenario.m_context->m_media_ip = media_ip;
	while (NULL != node)
	{
		if (strcmp("send", node->Name()) == 0)
		{
			//cout << node->GetText() << "recv" << endl;
			string* str = new string(node->GetText());
			str->erase(0, 1);
			SendSipRequest* sendsiprequest = new SendSipRequest((*str));

			UACScenario.addAction(sendsiprequest);
			cout << str;
			sendsiprequest->m_scenario = &UACScenario;
		}
		if (strcmp("recv", node->Name()) == 0)
		{
			string response = node->Attribute("request");
			ReceiveSipRequest* receivesiprequest = NULL;
			if (response == "INVITE")
				receivesiprequest = new ReceiveSipRequest(INVITE);
			if (response == "OK")
				receivesiprequest = new ReceiveSipRequest(OK);
			else if (response == "ACK")
				receivesiprequest = new ReceiveSipRequest(ACK);
			if (receivesiprequest)
			{
				UACScenario.addAction(receivesiprequest);
				receivesiprequest->m_scenario = &UACScenario;
				cout << text << "recv" << endl;
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
int main()
{
	const char* path = "..\\source\\scenarios\\uas.xml";
	xmlUAS(path);
	//UASSenario();	
	system("pause");
	return 0;
}
