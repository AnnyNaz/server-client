#include "SipScenario.h"
#include "UdpServer.h"
#include "UdpClient.h"
#include <thread>         
#include <chrono>
#include <string>
using namespace std;
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
	*(action->getScenario()) = *this;
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
	m_sip_response = new SipResponse(m_scenario->getContext()->last_message);
	return true;
}
bool Pause::execute()  
{
	std::this_thread::sleep_for(std::chrono::seconds(m_Sec)); 
	return true;
}
bool SendSipRequest::execute() 
{
	return true;
};
SipScenario::SipScenario(bool is_server, int port_this, string ip, int port_remote, string ip_remote)
{
	if (is_server)
	{
		m_connector = new UDPServer(port_this);
	}
	else
	{
		UDPClient * a= new UDPClient(ip, port_this);
		a->connectTo(ip_remote, port_remote);
		m_connector = a;
	}
}
