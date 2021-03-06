#ifndef NETWORKING_H
#define NETWORKING_H
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::udp;



class UdpBase
{
public:
	virtual bool sendString(const std::string& text);
	virtual std::string receiveString();
	virtual ~UdpBase();
protected:
	udp::endpoint other_endpoint;
	udp::socket* socket;
	boost::asio::io_context* io_context;
	const int  MESSAGE_SIZE = 1024;
};

#endif
