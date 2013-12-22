#include "tcpip_connection.hpp"
#include "tcpip_server.hpp"

xpcc::tcpip::Connection::Connection(boost::shared_ptr<boost::asio::io_service> ioService, xpcc::tcpip::Server* server):
    socket(*ioService),
    server(server)
{

}


boost::asio::ip::tcp::socket&
xpcc::tcpip::Connection::getSocket()
{
	return this->socket;
}


void
xpcc::tcpip::Connection::start()
{
	std::cout<< "receive: "<<xpcc::tcpip::TCPHeader::headerSize()<< "byte"<<std::endl;
    boost::asio::async_read(socket,
        boost::asio::buffer(this->header, xpcc::tcpip::TCPHeader::headerSize()),
        boost::bind(
          &xpcc::tcpip::Connection::handleReadHeader, this,
          boost::asio::placeholders::error));
}


void
xpcc::tcpip::Connection::handleReadHeader(const boost::system::error_code& error)
{
	std::cout << "Received Header"<<std::endl;
	if(! error){
		xpcc::tcpip::TCPHeader* header = reinterpret_cast<xpcc::tcpip::TCPHeader*>(this->header);
	    boost::asio::async_read(socket,
	        boost::asio::buffer(this->message, header->getDataSize()),
	        boost::bind(
	          &xpcc::tcpip::Connection::handleReadBody, this,
	          boost::asio::placeholders::error));
	}
}

void
xpcc::tcpip::Connection::handleReadBody(const boost::system::error_code& error)
{
	std::cout << "Received Body"<<std::endl;
    if(! error){
    	xpcc::tcpip::TCPHeader* header = reinterpret_cast<xpcc::tcpip::TCPHeader*>(this->header);
    	if(header->isDataMessage())
    	{
    		//evaluating data only required if Message is a data message
    	}
    	else
    	{
    		//message is a register message, spawn a new distributor thread
    		std::string ip = socket.remote_endpoint().address().to_string();
    		int componentId = header->getXpccHeader().source;
    		std::cout<<"Spawning sender for componentId "<<componentId;
    		this->server->spawnSendThread(componentId, ip);
    	}

        boost::asio::async_read(socket,
            boost::asio::buffer(this->header, xpcc::tcpip::TCPHeader::headerSize()),
            boost::bind(
              &xpcc::tcpip::Connection::handleReadHeader, this,
              boost::asio::placeholders::error));
    }
}
