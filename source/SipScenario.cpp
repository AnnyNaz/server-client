#include "SipScenario.h"
#include "UdpServer.h"
#include "UdpClient.h"
#include <thread>         
#include <chrono>
#include <string>
#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp> 
using namespace std;

void printMessage(string str, bool isIn)
{
	vector<string> result;
	boost::split(result, str, boost::is_any_of("\n"));
	if (isIn)
	{
		cout << "<-- " << endl;
	}
	else
	{
		cout << "-->" << endl;
	}
	for (auto i : result)
	{
		cout << i << endl;
	}
}

SipScenario* SipAction::getScenario()
{
	return m_scenario;
}
//---------------------------------------------------------------------------
ReceiveSipRequest::ReceiveSipRequest(ESipMethod method) : m_method(method)
{
}

bool ReceiveSipRequest::execute()
{
	if (m_scenario->getConnector() == NULL)
	{
		UDPServer* connector = new UDPServer(stoi(m_scenario->getContext()->m_local_port));
		m_scenario->setConnector(connector);
	}
	string message = "";
	SipRequest request;
	auto type = UNKNOWN;
	while (message == "" || type != m_method)
	{
		message = "";
		message = m_scenario->getConnector()->receiveString();
		printMessage(message, true);
		request.getfrom(message);
		SipResponse response(message);
		type = response.type();
		m_scenario->getContext()->m_to_tag = response.getToTag();
		m_scenario->getContext()->m_last_Via = response.getlast_Via();
		m_scenario->getContext()->m_last_From = response.getlast_From();
		m_scenario->getContext()->m_last_To = response.getlast_To();
		m_scenario->getContext()->m_last_Callid = response.getlast_Callid();
		m_scenario->getContext()->m_last_CSeq = response.getlast_CSeq();
	}
	return true;
}
//---------------------------------------------------------------------------

SendSipRequest::SendSipRequest(const string& m) : m_template(m)
{
}
bool SendSipRequest::execute()
{
	if (m_scenario->getConnector() == NULL)
	{
		UDPClient* connector = new UDPClient(m_scenario->getContext()->m_local_ip, stoi(m_scenario->getContext()->m_local_port));
		connector->connectTo(m_scenario->getContext()->m_remote_ip, stoi(m_scenario->getContext()->m_remote_port));
		m_scenario->setConnector(connector);
	}
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
	request.setrtcp_port(m_scenario->getContext()->m_rtcp_port);
	request.setmedia_ip_type(m_scenario->getContext()->m_media_ip_type);
	request.setmedia_ip(m_scenario->getContext()->m_media_ip);
	printMessage(request.toString(), false);
	return m_scenario->getConnector()->sendString(request.toString());

};
//---------------------------------------------------------------------------

SipInitContext::SipInitContext(SipScenario* sc, string this_ip, int this_port, string other_ip, int other_port, string local_ip_type, string media_ip_type, string media_ip, string media_port, string rtcp_port, string transport, string service, string name)
{
	m_scenario = sc;
	m_scenario->getContext()->m_local_ip_type = local_ip_type;
	m_scenario->getContext()->m_media_port = media_port;
	m_scenario->getContext()->m_media_ip_type = media_ip_type;
	m_scenario->getContext()->m_local_ip = this_ip;
	m_scenario->getContext()->m_remote_ip = other_ip;
	m_scenario->getContext()->m_media_ip = media_ip;
	m_scenario->getContext()->m_rtcp_port = rtcp_port;
	m_scenario->getContext()->m_remote_port = to_string(other_port);
	m_scenario->getContext()->m_local_port = to_string(this_port);
	m_scenario->getContext()->m_transport = transport;
	m_scenario->getContext()->m_service = service;
	m_scenario->getContext()->m_from = name;
};
bool SipInitContext::execute()
{

	m_scenario->getContext()->m_call_id = m_helper.generateHexString(m_length_of_code);
	m_scenario->getContext()->m_from_tag = m_helper.generateHexString(m_length_of_code);
	m_scenario->getContext()->m_via_branch = m_helper.generateHexString(m_length_of_code);
	m_scenario->getContext()->m_call_number = m_helper.generateHexString(m_length_of_code);
	return true;
}

//---------------------------------------------------------------------------
Pause::Pause(int sec) :m_Sec(sec)
{
}

bool Pause::execute()
{
	std::this_thread::sleep_for(std::chrono::microseconds(m_Sec));
	return true;
}

//---------------------------------------------------------------------------
SipScenario::SipScenario()
{
	m_connector = NULL;
	m_context = new SipContext();
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

UdpBase* SipScenario::getConnector()
{
	return m_connector;
}

bool SipScenario::setConnector(UdpBase* connector)
{
	if (connector == NULL)
		return false;
	m_connector = connector;
	return true;
}

//---------------------------------------------------------------------------



