
#include <thread>
#include "UdpServer.h"
#include "UdpClient.h"
#include "SipMethods.h"
#include <string>
#include <sstream>
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
	request.setType(INVITE);
	request.setFrom("bob", "boburl", "bobtag");
	request.setTo("alisa", "alisaurl", "alisatag");
	request.setCallId("a");
	request.setSequence("b");
	request.setUserAgent("alisa");
	//request.setUserAgent("sdp blabla");
	request.setSDP("sdp bla bla");
	std::string str = request.toString();
	SipResponse response(str);
	std::cout << "Equal " << (SipMessage(response) == SipMessage(request)) << std::endl;
	request.setType(OK);
	std::string str_ok = request.toString();
	SipResponse response_ok(str_ok);
	std::cout << "Equal " << (SipMessage(response_ok) == SipMessage(request)) << std::endl;
	
};
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
	testSipMethods();
	UDPServer udp(3058);

	auto str = udp.receiveString();

	SipResponse response_ok(str);
	SipRequest request;
	request.getfrom(response_ok);
	

	request.setType(OK);
	std::cout << "\n\n\n____\n"<< request.toString()<<"\n\n\n____\n";
	udp.sendString(request.toString());
	while (true) 
	{
		str = udp.receiveString();
		SipResponse response(str);
		std::cout << str << "\n\n\n____\n";
		if (response.type() == BYE)
		{
			udp.sendString(request.toString());
			break;
		}
	}
	//std::cout << generateHexString(6) << std::endl;
	//std::cout << str;
	//
	/*std::cout << " program started\n";
	std::thread BobThread(Bob);
	std::thread AliceThread(Alice);

	BobThread.join();
	AliceThread.join();*/

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