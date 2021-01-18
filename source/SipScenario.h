#ifndef SIPSCENARIO_H
#define SIPSCENARIO_H

#include <string>
#include <vector>
#include "SipMethods.h"
#include "Networking.h"
#include "SipHelper.h"
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

class SipInitContext : public SipAction
{
	public:
		SipInitContext(SipScenario* sc, std::string this_ip, int this_port, std::string other_ip, int other_port, std::string local_ip_type, std::string 
			_type, std::string 
			, std::string media_port, std::string rtcp_port, std::string transport, std::string service, std::string name);
		bool execute() override;
	private:
		SipHelper m_helper;
		const int m_length_of_code = 16;
};

class Pause:public SipAction
{
	public:
		Pause(int sec);
		bool execute() override;
	private:
		int m_Sec;
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
		std::string m_service = "";
		std::string m_transport = "";
		std::string m_remote_ip = "";
		std::string m_remote_port = "";
		std::string m_local_port = "";
		std::string m_local_ip = "";
		std::string m_last_Via = "";
		std::string m_last_From = "";
		std::string m_last_To = "";
		std::string m_call_number;
		std::string m_last_Callid = "";
		std::string m_last_CSeq = "";
		std::string m_local_ip_type;
		std::string m_media_port;
		std::string m_rtcp_port = "";
		std::string m_media_ip_type;
		std::string m_media_ip;

};

class SipScenario 
{
	public:
		SipScenario();
		bool run();
		void addAction(SipAction* action);
		SipContext* getContext();
		UdpBase* getConnector();
		bool setConnector(UdpBase* connector);
		std::vector<SipAction*> m_actions;
		SipContext* m_context;
	protected:
		UdpBase* m_connector;
};
#endif