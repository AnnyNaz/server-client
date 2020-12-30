#include <string>
#include <vector>
#include"SipMethods.h"
#include "Networking.h"
class SipScenario;
class SipAction
{
public:
	SipScenario* getScenario();
	virtual bool execute() = 0;
	SipScenario* m_scenario;
	
};
class ReceiveSipRequest : public SipAction
{
public:
	ReceiveSipRequest(ESipMethod m_method);
	bool execute() override;
private:
	SipResponse* m_sip_response;
	ESipMethod m_method;
};
class Pause:public SipAction
{
public:
	Pause(int sec);
	bool execute() override;
private:
	int m_Sec;
};
class SendSipRequest : public SipAction
{
public:
	SendSipRequest(ESipMethod m_method);
	bool execute() override;
private:
	SipRequest* m_sip_request;
	ESipMethod m_method;
};
class SipContext
{
public:
	SipContext() {};
	std::string last_message;
	std::string last_via;
};
class SipScenario 
{
public:
	SipScenario(UdpBase* connector);
	bool run();
	void addAction(SipAction* action);
	SipContext* getContext();
	UdpBase* getConnector();
protected:
	std::vector<SipAction*> m_actions;
	SipContext* m_context;
	UdpBase* m_connector;
};
std::string inviteMessage(std::string transport, int service, std::string from_name, std::string ip, int this_port, std::string other_ip, int other_port);