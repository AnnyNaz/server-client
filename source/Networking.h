#ifndef NETWORKING_H
#define NETWORKING_H
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;
class UdpBase
{
public:
	virtual bool sendString(const std::string& text);
	virtual std::string receiveString();
protected:
	udp::endpoint other_endpoint;
	udp::socket* socket;
	boost::asio::io_context* io_context;
};

#endif
