
#include <thread>
#include "UdpServer.h"
#include "UdpClient.h"
#include "SipMethods.h"
const int free_port = 3205;
void Bob()
{
	UDPClient udpbase("localhost", free_port);

	udpbase.sendString("HI Alice");

	auto str = udpbase.receiveString();
	std::cout << "client received: >" << str << "<" << std::endl;

	udpbase.sendString("Bye Alice");

	str = udpbase.receiveString();
	std::cout << "client received: >" << str << "<" << std::endl;
};


void Alice()
{
	UDPServer udpbase(free_port );
	udpbase.sendString("HI Bobjkjkj");
	auto str = udpbase.receiveString();
	std::cout << "server received: >" << str << "<" << std::endl;
	str = udpbase.receiveString();
	std::cout << "server received: >" << str << "<" << std::endl;

	udpbase.sendString("Bye Bob");
}
void testSipMethods() 
{
	SipRequest request;
	request.setFrom("bob");
	request.setTo("alisa");
	request.setUserAgent("alisa");
	std::cout << request.toString(); 
	
	SipResponse response("SIP/2.0 UNKNOWN\nVia:\nFrom:bob<boburl>boburl\nTo:alisa<>\nCall-ID: a\nCSeq: b\nMax-Forwards: 70\n l Subject: Performance Test\n g Content-Length: 12\n\nsdp blabla");
	std::cout << "sdp: " << response.getSDP() << "sequence " << response.sequence() << " callID()" << response.callID() << " via" << response.via() << std::endl;
};
int main()
{
	//testSipMethods();
	std::cout << " program started\n";
	std::thread BobThread(Bob);
	std::thread AliceThread(Alice);

	BobThread.join();
	AliceThread.join();

	system("pause");

	return 0;
}

/*{
	UDPClient udpbase("localhost",5060);

	udpbase.sendString("HI Alice");
	
	auto str = udpbase.receiveString();
	std::cout << "client received: >" << str << "<" << std::endl;
	
	udpbase.sendString("Bye Alice");
	
	str = udpbase.receiveString();
	std::cout << "client received: >" << str << "<" << std::endl;
};


void Alice()
{
	UDPServer udpbase(5060);

	auto str = udpbase.receiveString();
	std::cout << "server received: >" << str  << "<" << std::endl;

	udpbase.sendString("HI Bob");

	str = udpbase.receiveString();
	std::cout << "server received: >" << str << "<" << std::endl;

	udpbase.sendString("Bye Bob");
}

*/