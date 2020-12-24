#include "UdpServer.h"
#include <string>
using namespace std;
std::string make_daytime_string()
{
    time_t now = time(0);
    return ctime(&now);
}
UDPServer::UDPServer()
{

};
UDPServer::UDPServer(int port)
{
    listenFrom(port);
}
bool UDPServer::listenFrom(int port)
{
    io_context = new boost::asio::io_context();
    socket = new udp::socket(*io_context, udp::endpoint(udp::v4(), port));
    udp::resolver resolver(*io_context);
    
    boost::array<char, 0> empty_message;
    boost::system::error_code error;
   
    return true;
}
bool UDPServer::sendString(const std::string& text)
{

    return UdpBase::sendString(text);
}
std::string UDPServer::receiveString()
{
    return UdpBase::receiveString();
}