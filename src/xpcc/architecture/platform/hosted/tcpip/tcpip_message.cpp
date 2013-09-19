#include "tcpip_message.hpp"

xpcc::tcpip::TCPHeader::TCPHeader(uint8_t sender):
		type(Type::REGISTER), header()
{
	this->header.source = sender;
}

xpcc::tcpip::TCPHeader::TCPHeader(xpcc::Header& header):
		type(Type::DATA), header(header)
{

}

xpcc::Header&
xpcc::tcpip::TCPHeader::getXpccHeader()
{
	return this->header;
}

xpcc::tcpip::TCPHeader::Type
xpcc::tcpip::TCPHeader::getType() const
{
	return this->type;
}

xpcc::tcpip::Message::Message(xpcc::Header& , SmartPointer payload):
		header(header), data(payload)
{

}


xpcc::tcpip::Message::Message(uint8_t identifier):
		header(identifier), data()
{
}

bool
xpcc::tcpip::Message::decode(boost::shared_ptr<char> msg)
{

}


int16_t
xpcc::tcpip::Message::getMessageLength() const
{

}

bool
xpcc::tcpip::Message::isDataMessage() const
{
	return (this->header.getType() == TCPHeader::Type::DATA);
}

xpcc::Header&
xpcc::tcpip::Message::getXpccHeader()
{
	return this->header.getXpccHeader();
}
