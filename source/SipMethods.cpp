#include "SipMethods.h"
#include <boost/format.hpp>
#include <string>
#include <regex>
#include <iostream>
using namespace std;
	SipRequest::SipRequest() 
	{
		m_type = UNKNOWN;
		m_map_string_sip_methods[UNKNOWN] = "UNKNOWN";
		m_map_string_sip_methods[INVITE] = "INVITE";
		m_map_string_sip_methods[RINGNG] = "RINGNG";
		m_map_string_sip_methods[ACK] = "ACK";
		m_map_string_sip_methods[BYE] = "BYE";
		m_map_string_sip_methods[OK] = "200 OK";
	}
	void SipRequest::setType(ESipMethod m) 
	{
		m_type = m;
	};
	void SipRequest::setUserAgent(const std::string& useragent) {};
	void SipRequest::setFrom(const std::string& from)
	{
		m_from = from;
	};
	void SipRequest::setTo(const std::string& to)
	{
		m_to = to;
	}
	std::string SipRequest::toString()
	{
		string header = "SIP/2.0 ";
		header += m_map_string_sip_methods[m_type];
		boost::format fmt
			= boost::format{ "Via:%1%\n" \
				"From:%2%<%3%>%4%\n" \
				"To:%5%<%6%>%7%\n" \
				"Call-ID: %8%\n" \
				"CSeq: %9%\n" \
			//   "Contact: sip:sipp@{local_ip}:{local_port}\n" \ #TODO
			"Max-Forwards: 70\n" \
			"%10%" \
			"Subject: Performance Test\n" \
			"%11%" \
			"Content-Length: %12%\n" \
			"\n" \
			"%13%"}% m_via% m_from% m_from_URI% m_from_tag% m_to% m_to_URI% m_to_tag% m_call_id% m_sequence% m_addititonal_headers% m_content_type% m_sdp_len% m_sdp;
		return header + "\n" + fmt.str();
	}
	SipResponse::SipResponse(const std::string& str)
	{
		m_map_string_sip_methods["UNKNOWN"] = UNKNOWN;
		m_map_string_sip_methods["INVITE"] = INVITE;
		m_map_string_sip_methods["RINGNG"] = RINGNG;
		m_map_string_sip_methods["ACK"] = ACK;
		m_map_string_sip_methods["BYE"] = BYE;
		m_map_string_sip_methods["200 OK"] = OK;
		fromString(str);
	}
	void SipResponse::fromString(const std::string& str)
	{
		regex message = regex("SIP/2.0 (.*?)\nVia:(.*?)\nFrom:(.*?)<(.*?)>(.*?)\nTo:(.*?)<(.*?)>(.*?)\nCall-ID: (.*?)\nCSeq: (.*?)\nMax-Forwards: 70\n"\
			"(.*?)" \
			"Subject: Performance Test\n"\
			"(.*?)" \
			"Content-Length: (.*?)\n" \
			"\n" \
			"(.*?)");
		std::smatch matched;
		if (regex_match(str, matched, message))
		{
			m_type = m_map_string_sip_methods[matched[1].str()];
			m_via = matched[2].str();
			m_from = matched[3].str();
			m_from_URI = matched[4].str();
			m_from_tag = matched[5].str();
			m_to = matched[6].str();
			m_to_URI = matched[7].str();
			m_to_tag = matched[8].str();
			m_call_id = matched[9].str();
			m_sequence = matched[10].str();
			m_addititonal_headers = matched[11].str();
			m_content_type = matched[12].str();
			m_sdp_len = matched[13].str();
			m_sdp = matched[14].str();
		}
		else
		{
			cout << "couldn`t match anything" << endl;
		}
	}
	ESipMethod SipResponse::type() { return m_type; };
	string SipResponse::getSDP() { return m_sdp; };
	std::string SipResponse::sequence() { return m_sequence; };
	std::string SipResponse::callID() { return m_call_id; };
	std::string SipResponse::via() { return m_via; };