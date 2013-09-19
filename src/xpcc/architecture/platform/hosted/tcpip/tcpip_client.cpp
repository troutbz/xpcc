#include "tcpip_client.hpp"

#include <iostream>

xpcc::tcpip::Client::Client(std::string ip):
	ioService(new boost::asio::io_service()),
	work(new boost::asio::io_service::work(*ioService))
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
	std::cout << "Connected with: "<< error <<std::endl;
}
