
#include <thread>
#include "UdpServer.h"
#include "UdpClient.h"
#include "SipMethods.h"
#include <string>
#include <sstream>
#include <iostream>
#include <boost/algorithm/string.hpp> 
#include <thread>         
#include <chrono>
unsigned int seconds = 5;
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
	//UDPClient udp(ip, this_port);
	//udp.connectTo(ip, other_port);
	UDPServer udp(this_port);
	string str = udp.receiveString();
	printMessage(str, true);
	SipResponse response_ok(str);
	SipRequest request;
	request.getfrom(response_ok);
	printMessage(request.toString(), false);

	request.setType(OK);
	udp.sendString(request.toString());
	string ack = "";
		ack = udp.receiveString();
		printMessage(request.toString(), true);
		SipResponse ack_sip(ack);
		printMessage(ack, true);
		std::this_thread::sleep_for(std::chrono::seconds(seconds));
		SipRequest request_bye;
		request_bye.getfrom(ack_sip);
		request_bye.setType(BYE);
	printMessage(request_bye.toString(), false);
}

void UACScenarion()
{
	UDPClient udp(ip, this_port);
	udp.connectTo(ip, other_port);
	std::string message = "INVITE sip:7887@192.168.0.106:5065 SIP\/2.0\n\
Via:SIP/2.0/UDP 192.168.0.106 : 5062; branch = 04cygtA - FePAi - MfzpeXc - h8dghuiU\n\
From:PySip<sip : PySip@192.168.0.106:5062>; tag = qLTwaTqgSXmJ - KMM84jfC - IiIe - wAEUI - oUBgN - n8oAgce9 - cSBYF\n\
To:MCU<sip : 7887@192.168.0.106:5065>\n\
Call-ID: PbypGh7a - GcEz3HCz - J1NS - cXOL - Aj3k - WE0crjbz - sakW\n\
CSeq: 1 INVITE\n\
Max-Forwards : 70\n\
User-Agent: Avaya Communicator / 3.0 (3.9.0.84.67; Avaya CSDK; Microsoft Windows NT 6.2.9200.0)\mAllow : INVITE, ACK, OPTIONS, BYE, CANCEL, NOTIFY, MESSAGE, REFER, INFO, PUBLISH, UPDATE\n\
Subject: Performance Test\n\
Content-Type: application / sdp\n\
		Content-Length: 164\n\
		v = 0\n\
		o = user1 53655765 2353687637 IN IP4 192.168.0.106\n\
		s = -\n\
		c = IN IP4 192.168.0.106\n\
		t = 0 0\n\
		a = sourceid:1234567890\n\
		m = audio 51551 RTP / AVP 0\n\
		a = rtpmap : 0 PCMU / 8000\n\
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
