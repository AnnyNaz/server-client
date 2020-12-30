#include "SipScenario.h"
#include "UdpServer.h"
#include "UdpClient.h"
#include <thread>         
#include <chrono>
#include <string>
#include <iostream>
#include <fstream>
#include "utilities.cpp"
using namespace std;
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
void printMessage(string str, bool isIn)
{
	vector<string> result;
	boost::split(result, str, boost::is_any_of("\n"));
	if (isIn)
	{
		cout << "<-- ";
	}
	else
	{
		cout << "-->";
	}
	for (auto i : result)
	{
		cout << "\t" << i << endl;
	}
}

bool SipScenario::run() 
{
	for (auto action : m_actions) 
	{
		if (!action->execute())
			return false;
	}
	return true;
}
void SipScenario::addAction(SipAction* action) 
{
	m_actions.push_back(action);
	(*action).m_scenario = this;
}
SipContext* SipScenario::getContext()
{
	return m_context;
}
SipScenario* SipAction::getScenario() 
{
	return m_scenario;
}
UdpBase* SipScenario::getConnector() 
{
	return m_connector;
}
bool ReceiveSipRequest::execute() 
{
	string message = "";
	SipRequest request;
	auto type = UNKNOWN;
	while (message == "" || type != m_method)
	{
		message = "";
		message = m_scenario->getConnector()->receiveString();
		request.getfrom(message);
		printMessage(message, true);
		SipResponse response(message);
		type = response.type();
	}
	m_scenario->getContext()->last_message = message;
	return true;
}
Pause::Pause(int sec):m_Sec(sec)
{
}
bool Pause::execute()  
{
	std::this_thread::sleep_for(std::chrono::seconds(m_Sec)); 
	return true;
}
bool SendSipRequest::execute() 
{
	SipRequest request;
	request.getfrom(m_scenario->getContext()->last_message);
	request.setType(m_method);
	printMessage(request.toString(), false);
	return m_scenario->getConnector()->sendString(request.toString());
	
};
SipScenario::SipScenario(UdpBase* connector)
{
	m_connector = connector;
	m_context = new SipContext();
}
SendSipRequest::SendSipRequest(ESipMethod method) : m_method(method)
{
}
ReceiveSipRequest::ReceiveSipRequest(ESipMethod method) : m_method(method)
{
}
string inviteMessage(string transport, int service , string from_name, string ip, int this_port, string other_ip, int other_port )
{
	int id_length = 12;
	SipRequest invite;
	string via = " SIP/2.0/" + transport + " " + ip + ":" + to_string(this_port) + ";branch=" + generateHexString(id_length);

	invite.setVia(via);
	invite.setTo("MCU", "sip:" + to_string(service) + "@" + other_ip + ":" + to_string(other_port), "");
	invite.setFrom(from_name, "sip:" + from_name + "@" + ip + ":" + to_string(this_port), generateHexString(id_length));
	invite.setCallId(generateHexString(10));
	invite.setContact("sip:" + from_name + "@" + ip + ":" + to_string(this_port) + ";transport=udp");
	invite.setType(INVITE);
	invite.setContentType(" 8263 INVITE");
	string sdp = "v=0\n\
o=- 3818317038 3818317038 IN IP4 192.168.0.106\n\
s=pjmedia\n\
b=AS:84\n\
t=0 0\n\
a=X-nat:0\n\
m=audio 4002 RTP/AVP 8 0 101\n\
c=IN IP4 192.168.0.106\n\
b=TIAS:64000\n\
a=rtcp:4003 IN IP4 192.168.0.106\n\
a=sendrecv\n\
a=rtpmap:8 PCMA/8000\n\
a=rtpmap:0 PCMU/8000\n\
a=rtpmap:101 telephone-event/8000\n\
a=fmtp:101 0-16\n\
a=ssrc:1492521541 cname:48da188672ef56db";
	invite.setSDP(sdp, to_string(sdp.size()));
	return invite.toString();
	/*string res = "";
	string line;
	ifstream file("C:\\Users\\hanna.nazarkevych\\source\\repos\\server\\source\\invite.txt");
	if (file.is_open())
	{
		while (getline(file, line))
		{
			res = res + line + "\n";
		}
		file.close();
	}
	return res;*/
}