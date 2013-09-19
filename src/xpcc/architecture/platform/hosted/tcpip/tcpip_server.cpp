#include "tcpip_server.hpp"
#include <iostream>

xpcc::tcpip::Server::Server():
	ioService(new boost::asio::io_service()),
	work(new boost::asio::io_service::work(*ioService)),
	endpoint(boost::asio::ip::tcp::v4(), 32003),
	receiveSocket(*ioService),
	acceptor(*ioService, endpoint)
{
	this->acceptor.async_accept(this->receiveSocket,
			 boost::bind(&xpcc::tcpip::Server::accept_handler, this,
			          boost::asio::placeholders::error));
	ioService->run();
}


void
xpcc::tcpip::Server::accept_handler(const boost::system::error_code& error)
{
	std::cout<< "Connection Accepted" << std::endl;
}

void
xpcc::tcpip::Server::update()
{
}
