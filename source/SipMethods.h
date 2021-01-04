#include <string>
#include <unordered_map>
enum  ESipMethod {UNKNOWN, INVITE, RINGNG, ACK, BYE, OK};

class SipMessage
{
public:
	SipMessage();
	bool operator==(const SipMessage& rhs);
	
protected:
	
	ESipMethod m_type;
	std::string m_to = "";
	std::string m_to_URI = "";
	std::string m_to_tag = "";
	std::string m_from = "";
	std::string m_from_URI = "";
	std::string m_from_tag = "";
	std::string m_via = "";
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

	
	static std::unordered_map<std::string, ESipMethod > m_map_string_sip_methods;
	static std::unordered_map<ESipMethod, std::string> m_map_sipmethods_string;

};

class SipResponse : public SipMessage
{
public:
	SipResponse(const std::string& str);
	bool fromString(const std::string& str);
	ESipMethod type() const;
	std::string getFromName() const;
	std::string getFromURL() const;
	std::string getFromTag() const;
	std::string getToName() const;
	std::string getToURL() const;
	std::string getToTag() const;
	std::string getVia() const;
	std::string getSequence() const;
	std::string getSDP() const;
	std::string getSDPLen() const;
	std::string getContact() const;
	std::string getContentType() const;
	std::string getAddititonalHeaders() const;
	std::string getCallId() const;
private:
	bool parse(const std::string& regex_message, std::list<std::string*> vect_strings, const std::string& str);
	bool parse_header(const std::string& str);
};
class SipRequest : public SipMessage
{
public:
	SipRequest();
	SipRequest(const std::string& temp);
	std::string m_template = "";
	SipRequest operator= (const SipResponse& rhs);
	void getfrom( const SipResponse& rhs);
	void setType(ESipMethod m);
	std::string toString();
	void setUserAgent(const std::string& useragent);//??
	void setFrom(const std::string& from_name, const std::string& from_url, const std::string& from_tag);
	void setTo(const std::string& to_name, const std::string& to_url, const std::string& to_tag);
	void setVia(const std::string& via);
	void setSequence(const std::string& seq);
	void setSDP(const std::string& sdp, const std::string& sdp_len);
	void setContentType(const std::string& content_type);
	void setAddititonalHeaders(const std::string& headers);
	void setCallId(const std::string& id);
	void setContact(const std::string& contact);
};