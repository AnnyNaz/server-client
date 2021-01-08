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
		if (!(*action).execute())
			return false;
	}
	return true;
}
void SipScenario::addAction(SipAction* action) 
{
	m_actions.push_back(action);
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
		cout << "waiting for response" << m_method << endl;
		message = m_scenario->getConnector()->receiveString();
		printMessage(message, true);
		request.getfrom(message);
		
		SipResponse response(message);
		type = response.type();

		m_scenario->getContext()->m_last_Via = response.getlast_Via();
		m_scenario->getContext()->m_last_From = response.getlast_From();
		m_scenario->getContext()->m_last_To = response.getlast_To();
		
		m_scenario->getContext()->m_last_Callid = response.getlast_Callid();
		m_scenario->getContext()->m_last_CSeq = response.getlast_CSeq();
	}
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
	SipRequest request(m_template);
	request.setFrom(m_scenario->getContext()->m_from, m_scenario->getContext()->m_from_URI, m_scenario->getContext()->m_from_tag);
	request.setTo(m_scenario->getContext()->m_to, m_scenario->getContext()->m_to_URI, m_scenario->getContext()->m_to_tag);
	request.setVia(m_scenario->getContext()->m_via_branch);
	request.setSequence(m_scenario->getContext()->m_sequence);
	request.setSDP(m_scenario->getContext()->m_sdp, m_scenario->getContext()->m_sdp_len);
	request.setContentType(m_scenario->getContext()->m_content_type);
	request.setCallId(m_scenario->getContext()->m_call_id);
	request.setContact(m_scenario->getContext()->m_contact);
	request.setService(m_scenario->getContext()->m_service);
	request.setTransport(m_scenario->getContext()->m_transport);
	request.setRemoteIp(m_scenario->getContext()->m_remote_ip);
	request.setLocalIP(m_scenario->getContext()->m_local_ip);
	request.setLocalPort(m_scenario->getContext()->m_local_port);
	request.setRemotePort(m_scenario->getContext()->m_remote_port);
	request.setlast_Via(m_scenario->getContext()->m_last_Via);
	request.setlast_From(m_scenario->getContext()->m_last_From);
	request.setlast_To(m_scenario->getContext()->m_last_To);
	request.setcall_number(m_scenario->getContext()->m_call_number);
	request.setlast_Callid(m_scenario->getContext()->m_last_Callid);
	request.setlast_CSeq(m_scenario->getContext()->m_last_CSeq);
	request.setlocal_ip_type(m_scenario->getContext()->m_local_ip_type);
	request.setmedia_port(m_scenario->getContext()->m_media_port);
	request.setmedia_ip_type(m_scenario->getContext()->m_media_ip_type);
	request.setmedia_ip(m_scenario->getContext()->m_media_ip);
	printMessage(request.toString(), false);
	return m_scenario->getConnector()->sendString(request.toString());
	
};
SipScenario::SipScenario(UdpBase* connector)
{
	m_connector = connector;
	m_context = new SipContext();
}
SendSipRequest::SendSipRequest(const string & m) : m_template(m)
{
}
ReceiveSipRequest::ReceiveSipRequest(ESipMethod method) : m_method(method)
{
}
/*
string inviteMessage(string tem, string transport, int service , string from_name, string ip, int this_port, string other_ip, int other_port )
{
	int id_length = 12;
	SipRequest invite(tem);
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
	//return invite.toString();
	string res = "";
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
	return res;
}*/