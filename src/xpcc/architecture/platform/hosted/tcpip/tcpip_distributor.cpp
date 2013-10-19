#include "tcpip_distributor.hpp"


xpcc::tcpip::Distributor::Distributor(xpcc::tcpip::Server* parent, std::string ip, int port)
{

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

//TODO
void
xpcc::tcpip::Distributor::disconnect()
{

}

void
xpcc::tcpip::Distributor::connect_handler(const boost::system::error_code& error)
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
