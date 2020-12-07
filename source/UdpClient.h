
#include "Networking.h"
#include <iostream>

class UDPClient : public UdpBase
{
public:
	UDPClient();
	UDPClient(const std::string& ip, int port);
	bool connectTo(const std::string& ip, int port);
	bool sendString(const std::string& text) override;
	std::string receiveString() override;
};
