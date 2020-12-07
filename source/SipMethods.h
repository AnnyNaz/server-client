#include <string>
#include <unordered_map>
enum  ESipMethod {UNKNOWN, INVITE, RINGNG, ACK, BYE, OK};
class SipMessage
{

protected:
	ESipMethod m_type;
	std::string m_to = "";
	std::string m_to_URI = "";
	std::string m_to_tag = "";
	std::string m_from = "";
	std::string m_from_URI = "";
	std::string m_from_tag = "";
	std::string m_via = "";
	std::string m_cseq = "";
	std::string m_call_id = "";
	std::string m_sequence = "";
	std::string m_addititonal_headers = "";
	std::string m_content_type = "";
	std::string m_sdp_len = "0";
	std::string m_sdp = "";
	
};
class SipRequest: public SipMessage
{
public:
	SipRequest();
	void setType(ESipMethod m);
	std::string toString();
	void setUserAgent(const std::string& useragent);//??
	void setFrom(const std::string& from);
	void setTo(const std::string& to);
private:
	std::unordered_map<ESipMethod, const char*> m_map_string_sip_methods;
};
class SipResponse : public SipMessage
{
public:
	SipResponse(const std::string& str);
	void fromString(const std::string& str);
	ESipMethod type();
	std::string getSDP();
	std::string sequence();
	std::string callID();
	std::string via();
private:
	std::unordered_map<std::string, ESipMethod > m_map_string_sip_methods;
	
};
