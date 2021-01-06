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
	SendSipRequest(const std::string& templ);
	bool execute() override;
private:
	SipRequest* m_sip_request;
	ESipMethod m_method;
	const std::string& m_template;
};
class SipContext
{
public:
	SipContext() {};
	ESipMethod m_type;
	std::string m_to = "";
	std::string m_to_URI = "";
	std::string m_to_tag = "";
	std::string m_from = "";
	std::string m_from_URI = "";
	std::string m_from_tag = "";
	std::string m_via_branch = "";
	std::string m_call_id = "";
	std::string m_sequence = "";
	std::string m_addititonal_headers = "";
	std::string m_content_type = "";
	std::string m_sdp_len = "0";
	std::string m_sdp = "";
	std::string m_contact = "";
	std::string m_service = "7887";
	std::string m_transport = "UDP";
	std::string m_remote_ip = "127.0.0.1";
	std::string m_remote_port = "5060";
	std::string m_local_port = "5061";
	std::string m_local_ip = "127.0.0.1";
};
class SipScenario 
{
public:
	SipScenario(UdpBase* connector);
	bool run();
	void addAction(SipAction* action);
	SipContext* getContext();
	UdpBase* getConnector();
	std::vector<SipAction*> m_actions;
	SipContext* m_context;
protected:
	UdpBase* m_connector;
};
std::string inviteMessage(std::string transport, int service, std::string from_name, std::string ip, int this_port, std::string other_ip, int other_port);