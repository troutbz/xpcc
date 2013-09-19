#include "tcpip_server.hpp"
#include <iostream>

xpcc::tcpip::Server::Server():
	ioService(new boost::asio::io_service()),
	endpoint(boost::asio::ip::tcp::v4(), 32003),
	acceptor(*ioService, endpoint)
{
	spawnReceiveConnection();
	ioService->run();
}


void
xpcc::tcpip::Server::accept_handler(boost::shared_ptr<xpcc::tcpip::Connection> receive,
		const boost::system::error_code& error)
{
	std::cout<< "Connection from " << receive->getSocket().remote_endpoint().address().to_string()
			<< " accepted" << std::endl;

    if (!error)
    {
      this->receiveConnections.push_back(receive);
      receive->start();
    }

    spawnReceiveConnection();
}

void
xpcc::tcpip::Server::spawnReceiveConnection()
{
	boost::shared_ptr<xpcc::tcpip::Connection> receiveConnection(new Connection(ioService));
		this->acceptor.async_accept(receiveConnection->getSocket(),
				 boost::bind(&xpcc::tcpip::Server::accept_handler, this,
						 receiveConnection, boost::asio::placeholders::error));
}

void
xpcc::tcpip::Server::update()
{
}
