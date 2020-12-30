#include <thread>
#include "UdpServer.h"
#include "UdpClient.h"
#include "SipScenario.h"
#include <string>
#include <sstream>
#include <iostream>
#include <thread>         
#include <chrono>
using namespace std;
unsigned int seconds = 5;
const string from_name = "CppSip";
const string ip = "192.168.0.106";
const string this_ip = "192.168.0.106";
const string other_ip = "192.168.0.106";
const int other_port = 5061;
const int this_port = 5060;
const int SERVICE = 7887;
const string TRANSPORT = "UDP";

void UASSenario() 
{
	UDPServer* connector = new UDPServer(this_port);
	SipScenario UASScenario(connector);
	SipAction* act1 = new ReceiveSipRequest(INVITE);
	SipAction* act2 = new SendSipRequest(RINGNG);
	SipAction* act3 = new SendSipRequest(OK);
	SipAction* act4 = new ReceiveSipRequest(ACK);
	SipAction* act5 = new SendSipRequest(BYE);
	SipAction* act6 = new ReceiveSipRequest(OK);
	UASScenario.addAction(act1);
	UASScenario.addAction(act2);
	UASScenario.addAction(act3);
	UASScenario.addAction(act4);
	UASScenario.addAction(act5);
	UASScenario.addAction(act6);
	UASScenario.run();
}
void UACSenario()
{
	UDPClient* connector = new UDPClient(ip, this_port);
	connector->connectTo(ip, other_port);
	SipScenario UACScenario(connector);
	(UACScenario.getContext())->last_message = inviteMessage(TRANSPORT, SERVICE, from_name, ip, this_port, other_ip, other_port);
	SipAction* act1 = new SendSipRequest(INVITE);
	SipAction* act2 = new ReceiveSipRequest(RINGNG);
	SipAction* act3 = new ReceiveSipRequest(OK);
	SipAction* act_wait = new Pause(6);
	SipAction* act4 = new SendSipRequest(ACK);
	SipAction* act5 = new ReceiveSipRequest(BYE);
	SipAction* act6 = new SendSipRequest(OK);
	UACScenario.addAction(act1);
	UACScenario.addAction(act2);
	UACScenario.addAction(act3);
	
	UACScenario.addAction(act4);
	UACScenario.addAction(act_wait);
	UACScenario.addAction(act5);
	UACScenario.addAction(act6);
	UACScenario.run();
}
int main()
{
	UACSenario();
	
	system("pause");
	return 0;
}
