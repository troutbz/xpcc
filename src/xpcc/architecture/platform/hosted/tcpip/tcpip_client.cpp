#include "tcpip_client.hpp"

#include <iostream>

xpcc::tcpip::Client::Client(std::string ip):
	ioService(new boost::asio::io_service())
{
	boost::asio::ip::tcp::resolver resolver(*ioService);
	boost::asio::ip::tcp::resolver::query query(ip, "32003");
	this->endpointIter = resolver.resolve(query);

	//this->serviceThread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&boost::asio::io_service::run, ioService)));

	this->sendSocket.reset(new boost::asio::ip::tcp::socket(*ioService));

	boost::asio::async_connect(*sendSocket, endpointIter,
        boost::bind(&xpcc::tcpip::Client::connect_handler, this,
          boost::asio::placeholders::error));
	this->ioService->run();

}


void
xpcc::tcpip::Client::addComponent(uint8_t id)
{
	boost::shared_ptr<ComponentInfo> newComponent = boost::shared_ptr<ComponentInfo>(new ComponentInfo(id));
	this->componentMap.insert(std::make_pair (id,newComponent));
	boost::shared_ptr<xpcc::tcpip::Message> msg(new xpcc::tcpip::Message(id));
	std::cout<<"add Component"<<std::endl;
	this->sendPacket(msg);

}

void
xpcc::tcpip::Client::registerComponents()
{
}

void
xpcc::tcpip::Client::update()
{
}


void
xpcc::tcpip::Client::sendAlivePing(int identifier)
{

}

void
xpcc::tcpip::Client::connect_handler(const boost::system::error_code& error)
{
	std::cout << "Connected with error-code: "<< error <<std::endl;
}

//send a xpcc packet to the server
void
xpcc::tcpip::Client::sendPacket(boost::shared_ptr<xpcc::tcpip::Message> msg)
{
	writingMessages = !messagesToBeSent.empty();
	messagesToBeSent.push_back(msg);
    if (!writingMessages)
    {
      std::cout<<"Sending: "<<messagesToBeSent.front()->getMessageLength() <<std::endl;
      messagesToBeSent.front()->encodeMessage();
      boost::asio::async_write(*sendSocket,
          boost::asio::buffer(messagesToBeSent.front()->getEncodedMessage(),
          messagesToBeSent.front()->getMessageLength()),
          boost::bind(&xpcc::tcpip::Client::writeHandler, this,
            boost::asio::placeholders::error));
    }
}

void
xpcc::tcpip::Client::writeHandler(const boost::system::error_code& error)
{
	std::cout<<"send Complete"<<std::endl;
    if (!error)
    {
    	//Remove sent message
    	messagesToBeSent.pop_front();
    	if (!messagesToBeSent.empty())
    	{
    		//Prepare next message
    		messagesToBeSent.front()->encodeMessage();
    		boost::asio::async_write(*sendSocket,
    		        boost::asio::buffer(messagesToBeSent.front()->getEncodedMessage(),
    		        messagesToBeSent.front()->getMessageLength()),
    				boost::bind(&xpcc::tcpip::Client::writeHandler, this,
    				boost::asio::placeholders::error));
      }
    }
}
