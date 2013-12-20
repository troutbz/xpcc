#include "tcpip_distributor.hpp"
#include <xpcc/architecture/platform/hosted/tcpip/tcpip_server.hpp>
#include <sstream>

xpcc::tcpip::Distributor::Distributor(xpcc::tcpip::Server* parent, std::string ip, int port):
	connected(false),
	writingMessages(false),
	ioService(parent->getIoService()),
	server(parent),
	port(port)
{
	boost::asio::ip::tcp::resolver resolver(*ioService);
	//port required as string
	std::stringstream portStream;
	portStream<<port;
	boost::asio::ip::tcp::resolver::query query(ip, portStream.str());
	this->endpointIter = resolver.resolve(query);
	this->sendSocket.reset(new boost::asio::ip::tcp::socket(*ioService));

}

void
xpcc::tcpip::Distributor::run()
{

}


void
xpcc::tcpip::Distributor::sendMessage(boost::shared_ptr<xpcc::tcpip::Message> msg)
{
	writingMessages = !messagesToBeSent.empty();
	messagesToBeSent.push_back(msg);
    if (!writingMessages)
    {
      messagesToBeSent.front()->encodeMessage();
      boost::asio::async_write(*sendSocket,
          boost::asio::buffer(messagesToBeSent.front()->getEncodedMessage(),
          messagesToBeSent.front()->getMessageLength()),
          boost::bind(&xpcc::tcpip::Distributor::sendHandler, this,
            boost::asio::placeholders::error));
    }
}

//TODO
void
xpcc::tcpip::Distributor::disconnect()
{

}

void
xpcc::tcpip::Distributor::connectHandler(const boost::system::error_code& error)
{

}

void
xpcc::tcpip::Distributor::sendHandler(const boost::system::error_code& error)
{

}

//TODO
void
xpcc::tcpip::Distributor::close()
{

}
