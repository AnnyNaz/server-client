#include <string>
#include <unordered_map>
enum  ESipMethod {UNKNOWN, INVITE, RINGNG, ACK, BYE, OK, TRY, SESSION_PROGRESS};
const std::string LEN_XML = "[len]";
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
	std::string m_remote_ip;
	std::string m_remote_port;
	std::string m_rtcp_port;
	std::string m_local_port;
	std::string m_local_ip;
	std::string m_last_Via;
	std::string m_last_From;
	std::string m_last_To;
	std::string m_call_number;
	std::string m_last_Callid;
	std::string m_last_CSeq;
	std::string m_local_ip_type;
	std::string m_media_port;
	std::string m_media_ip_type;
	std::string m_media_ip;
	
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
	std::string getlast_Via()const;
	std::string getlast_From()const;
	std::string getlast_To()const;
	std::string getcall_number()const;
	std::string getlast_Callid()const;
	std::string getlast_CSeq()const;
	

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
	void getfrom(const SipResponse& rhs);
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
	void setService(const std::string& service);
	void setTransport(const std::string& transport);
	void setRemoteIp(const std::string& remote_ip);
	void setLocalIP(const std::string& local_ip);
	void setLocalPort(const std::string& local_port);
	void setRemotePort(const std::string& remote_port);
	void setlast_Via(const std::string& m_last_Via);
	void setlast_From(const std::string& m_last_From);
	void setlast_To(const std::string& m_last_To);
	void setcall_number(const std::string& m_call_number);
	void setlast_Callid(const std::string& m_last_Callid);
	void setlast_CSeq(const std::string& m_last_CSeq);
	void setlocal_ip_type(const std::string& str);
	void setmedia_port(const std::string& str);
	void setmedia_ip(const std::string& str);
	void set_type(const std::string& str);
	void setrtcp_port(const std::string& str);
	void setmedia_ip_type(const std::string& str);
};