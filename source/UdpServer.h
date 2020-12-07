#ifndef UDPSERVER_H
#define UDPSERVER_H
#include "Networking.h"
class UDPServer : public UdpBase
{
public:
	UDPServer();
	UDPServer(int port);
	bool listenFrom(int port);
	bool sendString(const std::string& text) override;
	std::string receiveString() override;
	
};
#endif
