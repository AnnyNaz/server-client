
#include <thread>
#include "UdpServer.h"
#include "UdpClient.h"
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
	udpbase.sendString("HI Bob");
	auto str = udpbase.receiveString();
	std::cout << "server received: >" << str << "<" << std::endl;
	str = udpbase.receiveString();
	std::cout << "server received: >" << str << "<" << std::endl;

	udpbase.sendString("Bye Bob");
}

int main()
{
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