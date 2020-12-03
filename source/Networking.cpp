#include "Networking.h"
bool UdpBase::sendString(const std::string& text)
{
	try
	{
		socket->send_to(boost::asio::buffer(text), other_endpoint);
		return true;
	}
	catch (std::exception& e)
	{
		//std::cerr <<"UdpBase"<< e.what() << std::endl;
		return false;
	}
}

std::string UdpBase::receiveString()
{
	std::string res;
	try
	{
		boost::array<char, 128> recv_buf;
		std::fill(recv_buf.begin(), recv_buf.end(), 0);
		udp::endpoint sender_endpoint;
		size_t len = socket->receive_from(
			boost::asio::buffer(res), sender_endpoint);
		return recv_buf.data();
	}
	catch (std::exception& e)
	{
		std::cerr <<"UDPClient" << e.what() << std::endl;
	}
	return "";
}
