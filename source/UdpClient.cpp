#include "UdpClient.h"
#include <iostream>
#include <vector>
#include <string>
#include <boost/asio/ip/address_v4.hpp>
using namespace std;
UDPClient::UDPClient() 
{ 
	
};
UDPClient::UDPClient(const std::string& ip, int port)
{
	io_context = new boost::asio::io_context();
	udp::resolver resolver(*io_context);
	udp::endpoint this_endpoint = *resolver.resolve(udp::v4(), ip, to_string(port)).begin();
	socket = new udp::socket(*io_context);

	socket->open(udp::v4());
	
	
};

bool UDPClient::connectTo(const std::string& ip, int port)
{
	udp::resolver resolver(*io_context);
	other_endpoint = *resolver.resolve(udp::v4(),ip, to_string(port)).begin();
	socket->connect(other_endpoint);
	
	return true;
};
bool UDPClient::sendString(const std::string& text)
{
	return UdpBase::sendString(text);
};
std::string UDPClient::receiveString() 
{
	return UdpBase::receiveString();
};