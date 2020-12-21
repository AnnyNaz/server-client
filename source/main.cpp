
#include <thread>
#include "UdpServer.h"
#include "UdpClient.h"
#include "SipMethods.h"
#include <string>
#include <sstream>
#include <iostream>
#include <boost/algorithm/string.hpp> 
using namespace std;

const string ip = "127.0.0.1";
const int other_port = 5060;
const int this_port = 5061;


void printMessage(string str, bool isIn) 
{
	vector<string> result;
	boost::split(result, str, boost::is_any_of("\n"));	
	if (isIn) 
	{
		cout << "<-- " ;
	}
	else 
	{
		cout << "-->";
	}
	for (auto i : result) 
	{
		cout << "\t" << i << endl;
	}
}
void UASScenarion() 
{
	string str;
	//UDPClient udp(ip, this_port);
	//udp.connectTo(ip, other_port);
	UDPServer udp(this_port);
	while(str.size()==0)
		auto str = udp.receiveString();
	printMessage(str, true);
	SipResponse response_ok(str);
	SipRequest request;
	request.getfrom(response_ok);
	printMessage(request.toString(), false);

	request.setType(OK);
	udp.sendString(request.toString());
	while (true)
	{
		str = udp.receiveString();
		printMessage(request.toString(), false);
		SipResponse response(str);
		printMessage(request.toString(), true); 
		if (response.type() == BYE)
		{
			udp.sendString(request.toString());
			break;
		}
	}
}

void UACScenarion()
{
	UDPClient udp(ip, other_port);
	std::string message = "INVITE sip:7887@192.168.0.106:5065 SIP/2.0\
		Via: SIP / 2.0 / UDP 192.168.0.106 : 5062; branch = 04cygtA - FePAi - MfzpeXc - h8dghuiU\
		From : PySip<sip : PySip@192.168.0.106:5062>; tag = qLTwaTqgSXmJ - KMM84jfC - IiIe - wAEUI - oUBgN - n8oAgce9 - cSBYF\
		To : MCU<sip : 7887@192.168.0.106:5065>\
		Call - ID : PbypGh7a - GcEz3HCz - J1NS - cXOL - Aj3k - WE0crjbz - sakW\
		CSeq : 1 INVITE\
		Max - Forwards : 70\
		User - Agent : Avaya Communicator / 3.0 (3.9.0.84.67; Avaya CSDK; Microsoft Windows NT 6.2.9200.0)\mAllow : INVITE, ACK, OPTIONS, BYE, CANCEL, NOTIFY, MESSAGE, REFER, INFO, PUBLISH, UPDATE\
		Subject : Performance Test\
		Content - Type : application / sdp\
		Content - Length : 164\
		v = 0\
		o = user1 53655765 2353687637 IN IP4 192.168.0.106\
		s = -\
		c = IN IP4 192.168.0.106\
		t = 0 0\
		a = sourceid:1234567890\
		m = audio 51551 RTP / AVP 0\
		a = rtpmap : 0 PCMU / 8000\
		a = sendrecv";
	bool k = udp.sendString(message);
	cout << k << endl;
	k = udp.sendString(message);
	cout << k << endl;
	if (k)
		printMessage(message, true);
	/*SipResponse response_ok(str);
	SipRequest request;
	request.getfrom(response_ok);
	printMessage(request.toString(), false);

	request.setType(OK);
	udp.sendString(request.toString());*/
	while (true)
	{
		message = udp.receiveString();
		printMessage(message, true);
		SipResponse response(message);
		
		if (response.type() == OK)
		{
			//udp.sendString(request.toString());
			cout << "\nhurra\n";
			break;
		}
	}
}



std::string generateHexString(int length) 
{
	
	std::stringstream ss;
	for (int i = 0; i < length; ++i) 
	{
		ss<< std::hex<<((rand()%16));
	}
	std::string res(ss.str());
	return res;

}
int main()
{
	UASScenarion();
	system("pause");
	return 0;
}
