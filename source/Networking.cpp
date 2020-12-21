
#include "Networking.h"
bool UdpBase::sendString(const std::string& text)
{
	boost::array<char, 1024> send_buf;
	for (int i = 0; i < text.size() / MESSAGE_SIZE + 1; ++i)
	{
		std::copy(text.begin() + i * MESSAGE_SIZE, text.begin() + std::min(i * MESSAGE_SIZE, int(text.size())), send_buf.data());
		try
		{
			socket->send_to(boost::asio::buffer(text), other_endpoint);
			return true;
		}
		catch (std::exception& e)
		{
			return false;
		}
	}


}

std::string UdpBase::receiveString()
{
	try
	{
		boost::array<char, 5000> recv_buf;
		std::fill(recv_buf.begin(), recv_buf.end(), 0);
		size_t len = socket->receive_from(
			boost::asio::buffer(recv_buf), other_endpoint);
		return recv_buf.data();
	}
	catch (std::exception& e)
	{
		std::cerr << "UDPClient" << e.what() << std::endl;
	}
	return "";
}
UdpBase::~UdpBase()
{
	delete socket;
	delete io_context;
}