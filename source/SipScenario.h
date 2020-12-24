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
protected:
	SipScenario* m_scenario;
};
class ReceiveSipRequest : public SipAction
{
public:
	bool execute() override;
private:
	SipResponse* m_sip_response;
};
class Pause:public SipAction
{
public:
	bool execute() override;
private:
	int m_Sec;
};
class SendSipRequest : public SipAction
{
public:
	bool execute() override;
private:
	SipRequest* m_sip_request;
};
class SipContext
{
public:
	std::string last_message;
	std::string last_via;
};
class SipScenario 
{
public:
	SipScenario(bool is_server, int port_this, std::string ip, int port_remote, std::string ip_remote);
	bool run();
	void addAction(SipAction* action);
	SipContext* getContext();
	UdpBase* getConnector();
protected:
	std::vector<SipAction*> m_actions;
	SipContext* m_context;
	UdpBase* m_connector;
};
