#include "SipMethods.h"
#include <boost/format.hpp>
#include <string>
#include <regex>
#include <iostream>
#include <sstream>
using namespace std;
unordered_map<std::string, ESipMethod > SipMessage::m_map_string_sip_methods;
unordered_map<ESipMethod, std::string> SipMessage::m_map_sipmethods_string;
SipRequest SipRequest::operator= (const SipResponse& rhs) 
{
	SipRequest* res = new SipRequest();
	res->getfrom(rhs);
	return *(res);
}
void SipRequest::getfrom(const SipResponse& response)
{
	setTo(response.getToName(), response.getToURL(), response.getToTag());
	setFrom(response.getFromName(), response.getFromURL(), response.getFromTag());
	setVia(response.getVia());
	setSequence(response.getSequence());
	setCallId(response.getCallId());
	setSDP(response.getSDP(), response.getSDPLen());
	setContact(response.getContact());
	setContentType(response.getContentType());
}
SipMessage::SipMessage()
{
	SipMessage::m_map_sipmethods_string[UNKNOWN] = "UNKNOWN";
	SipMessage::m_map_sipmethods_string[INVITE] = "INVITE";
	SipMessage::m_map_sipmethods_string[RINGNG] = "180 Ringing";
	SipMessage::m_map_sipmethods_string[ACK] = "ACK";
	SipMessage::m_map_sipmethods_string[BYE] = "BYE";
	SipMessage::m_map_sipmethods_string[OK] = "200 OK";
	SipMessage::m_map_sipmethods_string[TRY] = "100 Trying";
	SipMessage::m_map_sipmethods_string[SESSION_PROGRESS] = "183 Session Progress";

	SipMessage::m_map_string_sip_methods["UNKNOWN"] = UNKNOWN;
	SipMessage::m_map_string_sip_methods["INVITE"] = INVITE;
	SipMessage::m_map_string_sip_methods["180 Ringing"] = RINGNG;
	SipMessage::m_map_string_sip_methods["ACK"] = ACK;
	SipMessage::m_map_string_sip_methods["BYE"] = BYE;
	SipMessage::m_map_string_sip_methods["200 OK"] = OK;
	SipMessage::m_map_string_sip_methods["100 Trying"] = TRY;
	SipMessage::m_map_string_sip_methods["183 Session Progress"] = SESSION_PROGRESS;
}
	bool SipMessage::operator==(const SipMessage& rhs)
	{
		bool res =  m_type == rhs.m_type &&  m_to == rhs.m_to &&  m_to_URI == rhs.m_to_URI &&  m_to_tag == rhs.m_to_tag;
		res = res &&  m_from == rhs.m_from &&  m_from_URI == rhs.m_from_URI &&  m_from_tag == rhs.m_from_tag &&  m_via_branch == rhs.m_via_branch;
		res = res &&  m_call_id == rhs.m_call_id &&  m_sequence == rhs.m_sequence;
		res = res &&  m_sdp_len == rhs.m_sdp_len &&  m_addititonal_headers == rhs.m_addititonal_headers &&  m_content_type == rhs.m_content_type;
		return res;
	}
	SipRequest::SipRequest()
	{
		m_type = UNKNOWN;
	}
	SipRequest::SipRequest(const std::string& temp)
	{
		m_type = UNKNOWN;
		m_template = temp;
	}
	std::string& replace_all_mute(std::string& s, const std::string& from, const std::string& to)
	{
		if (!from.empty())
			for (std::size_t pos = 0; (pos = s.find(from, pos) + 1); pos += to.size())
				s.replace(--pos, from.size(), to);
		return s;
	}

	std::string change(string template_str, unordered_map<string, string> m) 
	{
		string res = template_str;
		for (auto it : m) 
		{
			replace_all_mute(res, it.first, it.second);
		}
		
		return res;
	}
	std::string SipRequest::toString()
	{
		string result = "";
		unordered_map<string, string> map;
		map["[service]"] = m_service;
		map["[remote_ip]"] = m_remote_ip;
		map["[local_ip]"] = m_local_ip;
		map["[remote_port]"] = m_remote_port;
		map["[transport]"] = m_transport;
		map["[local_port]"] = m_local_port;
		map["[local_port]"] = m_local_port;
		map["[branch]"] = m_via_branch;
		map["[pid]"] = m_from_tag;
		map["[call_number]"] = m_call_number;
		map["[call_id]"] = m_call_id;
		//map["[len]"] = m_sdp_len;
		map["[peer_tag_param]"] = m_to_tag;
		map["[last_Via:]"] = m_last_Via;
		map["[last_From:]"] = m_last_From;
		map["[last_To:]"] = m_last_To;
		map["[last_Call-ID:]"] = m_last_Callid;
		map["[last_CSeq:]"] = m_last_CSeq;
		map["[local_ip_type]"] = m_local_ip_type;
		map["[media_port]"] = m_media_port;
		map["[rtcp_port]"] = m_rtcp_port;
		map["[media_ip_type]"] = m_media_ip_type;
		map["[media_ip]"] = m_media_ip;
		
		result = change(m_template, map);
		replace_all_mute(result, "\n", "\r\n");
		int found_len = result.find(LEN_XML);
		int len = result.size() - found_len -LEN_XML.size() - 3;
		replace_all_mute(result, LEN_XML, to_string(len));
		return result;
			
	}
	SipResponse::SipResponse(const std::string& str)
	{
		fromString(str);
	}
	bool SipResponse::parse(const string& regex_message, list<std::string*> vect_strings, const string& str)
	{
		std::regex message(regex_message);
		//std::cout <<"\n\nMESSAGE__\n\n" <<regex_message << "\n\nMESSAGE__\n\n" << str << endl;
		std::smatch matched;
		if (regex_match(str, matched, message))
		{
			int k = 1;
			for (auto i: vect_strings)
			{
				*(i) = matched[k];
				++k;
			}
			return true;
		}
		else
		{
			cout << "\n\nSipResponse couldn`t match anything\n\n" << endl;
			return false;
		}
	}

	bool SipResponse::parse_header(const std::string& str)
	{
		bool res;
		string type;
		string regex_message = "";
		list<string*> list_of_variables;
		if ((str.find(SipMessage::m_map_sipmethods_string[INVITE])) != string::npos || (str.find(SipMessage::m_map_sipmethods_string[ACK])) != string::npos || (str.find(SipMessage::m_map_sipmethods_string[BYE]) != string::npos))
		{
			regex_message = "(.*?) (.*?) SIP\/2.0\\s*";
			list_of_variables.push_back(&type);
			list_of_variables.push_back(&m_to_URI);
		}
		else
		{
			regex_message = "SIP\/2.0 (.*?)\\s*";
			list_of_variables.push_back(&type);
			 
		}
		res = parse(regex_message, list_of_variables, str);
		m_type = SipMessage::m_map_string_sip_methods[*list_of_variables.front()];
		return res;
	}

	bool SipResponse::fromString(const std::string& str)
	{
		std::istringstream sstream(str);
		std::string line;
		string regex_message = "(?s).*";
		getline(sstream, line);
		if (!parse_header(line))
		{
			return false;
		}
		
		bool is_sdp = false;
		while (!sstream.eof())
		{

			getline(sstream, line);
			if (!line.empty() && line[line.size() - 1] == '\r')
				line.erase(line.size() - 1);
			if (is_sdp)
			{
				m_sdp = m_sdp + line + "\n";
				continue;
			}
			list<string*> list_of_variables;
			regex_message = "(?s).*";
			if (line.rfind("To", 0) == 0) 
			{
				regex_message = "To:(.*?)<(.*?)>(.*?)\\s*";
				list_of_variables.push_back(&m_to);
				list_of_variables.push_back(&m_to_URI);
				list_of_variables.push_back(&m_to_tag);
				m_last_To = line;
			}
			else if (line.rfind("From", 0) == 0)
			{
				regex_message = "From:(.*?)<(.*?)>(.*?)\\s*";
				list_of_variables.push_back(&m_from);
				list_of_variables.push_back(&m_from_URI);
				list_of_variables.push_back(&m_from_tag);
				m_last_From = line;
			}
			else if (line.rfind("CSeq", 0) == 0)
			{
				regex_message = "CSeq:(.*?)\\s*";
				list_of_variables.push_back(&m_content_type);
				m_last_CSeq = line;
			}
			else if (line.rfind("Via", 0) == 0)
			{
				regex_message = "Via:(.*?)\\s*";
				list_of_variables.push_back(&m_via_branch);
				m_last_Via = line;
			}
			else if (line.rfind("CSeq:", 0) == 0)
			{
				regex_message = "CSeq:(.*?)\\s*";
				list_of_variables.push_back(&m_sequence);
			}
			else if (line.rfind("Contact:", 0) == 0)
			{
				regex_message = "Contact:(.*?)\\s*";
				list_of_variables.push_back(&m_contact);
			}
			else if (line.rfind("Call-ID", 0) == 0)
			{
				regex_message = "Call-ID:(.*?)\\s*";
				list_of_variables.push_back(&m_call_id);
				m_last_Callid = line;
			}
			else if (line.rfind("Content-Length:", 0) == 0)
			{
				regex_message = "Content-Length: (.*?)\\s*";
				list_of_variables.push_back(&m_sdp_len);
				is_sdp = true;
			}
			else 
			{
				continue;
			}
			if (!parse(regex_message, list_of_variables, line))
				return false;
		}
		
		return true;
	}
	ESipMethod SipResponse::type() const { return m_type; }

	void SipRequest::setType(ESipMethod m)
	{
		m_type = m;
	};
	void SipRequest::setUserAgent(const std::string& useragent) {};

	void SipRequest::setFrom(const std::string& from_name, const std::string& from_url, const std::string& from_tag) 
	{
		m_from = from_name;
		m_from_tag = from_tag;
		m_from_URI = from_url;
	};
	void SipRequest::setTo(const std::string& to_name, const std::string& to_url, const std::string& to_tag) 
	{
		m_to = to_name;
		m_to_tag = to_tag;
		m_to_URI = to_url;
	};
	void SipRequest::setSDP(const std::string& sdp, const std::string& sdp_len)
	{
		m_sdp = sdp;
		m_sdp_len = sdp_len;
	} 
	void SipRequest::setContact(const std::string& contact)
	{
		m_contact = contact;
	}
	void SipRequest::setContentType(const std::string& content_type) 
	{
		m_content_type = content_type;
	};
	void SipRequest::setCallId(const std::string& id) 
	{
		m_call_id = id;
	}
	void SipRequest::setVia(const std::string& via)
	{
		m_via_branch = via;
	}
	void SipRequest::setSequence(const std::string& seq) 
	{
		m_sequence = seq;
	}
	void SipRequest::setlast_Via(const std::string& last_Via)
	{
		m_last_Via = last_Via;
	}
	void SipRequest::setlast_From(const std::string& last_From)
	{
		m_last_From = last_From;
	}
	void SipRequest::setlast_To(const std::string& last_To)
	{
		m_last_To = last_To;
	}
	void SipRequest::setcall_number(const std::string& call_number)
	{
		m_call_number = call_number;
	}
	void SipRequest::setlast_Callid(const std::string& last_Callid)
	{
		m_last_Callid = last_Callid;
	}
	void SipRequest::setlast_CSeq(const std::string& last_CSeq)
	{
		m_last_CSeq = last_CSeq;
	}
	void SipRequest::setAddititonalHeaders(const std::string& headers)
	{
		m_addititonal_headers = headers;
	}
	std::string SipResponse::getFromName() const
	{
		return m_from;
	}
	std::string SipResponse::getContact() const
	{
		return m_contact;
	}
	std::string SipResponse::getFromURL() const
	{
		return m_from_URI;
	}
	std::string SipResponse::getFromTag() const
	{
		return m_from_tag;
	}
	std::string SipResponse::getToName() const
	{
		return m_to;
	}
	std::string SipResponse::getToURL() const
	{
		return m_to_URI;
	}
	std::string SipResponse::getToTag() const
	{
		return m_to_tag;
	}
	std::string SipResponse::getSDP() const
	{
		return m_sdp;
	}
	std::string SipResponse::getSDPLen() const
	{
		return m_sdp_len;
	}
	std::string SipResponse::getContentType() const
	{
		return m_content_type;
	};
	std::string SipResponse::getCallId() const
	{
		return m_call_id;
	}
	std::string SipResponse::getVia() const
	{
		return m_via_branch;
	}
	std::string SipResponse::getSequence() const
	{
		return m_sequence;
	}
	std::string SipResponse::getAddititonalHeaders() const
	{
		return m_addititonal_headers;
	}
	void SipRequest::setService(const std::string& service) { m_service = service; }
	void SipRequest::setTransport(const std::string& transport) { m_transport = transport; }
	void SipRequest::setRemoteIp(const std::string& remote_ip) { m_remote_ip = remote_ip; }
	void SipRequest::setLocalIP(const std::string& local_ip) { m_local_ip = local_ip; }
	void SipRequest::setLocalPort(const std::string& local_port) { m_local_port = local_port; }
	void SipRequest::setRemotePort(const std::string& remote_port) { m_remote_port = remote_port; }
	void SipRequest::setlocal_ip_type(const std::string& str) { m_local_ip_type = str; }
	void SipRequest::setmedia_port(const std::string& str) { m_media_port = str; }
	void SipRequest::set_type(const std::string& str) { m_media_ip_type = str; }
	void SipRequest::setmedia_ip(const std::string& str) { m_media_ip = str; }
	void SipRequest::setrtcp_port(const std::string& str) { m_rtcp_port = str; }
	void SipRequest::setmedia_ip_type(const std::string& str) { m_media_ip_type = str; }
	std::string SipResponse::getlast_Via()const
	{
		return m_last_Via;
	}
	std::string SipResponse::getlast_From() const
	{
		return m_last_From;
	}
	std::string SipResponse::getlast_To() const
	{
		return m_last_To;
	}
	std::string SipResponse::getcall_number() const
	{
		return m_call_number;
	}
	std::string SipResponse::getlast_Callid() const
	{
		return m_last_Callid;
	}
	std::string SipResponse::getlast_CSeq() const
	{
		return m_last_CSeq;
	}
	