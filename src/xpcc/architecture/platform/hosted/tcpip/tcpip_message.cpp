#include "tcpip_message.hpp"

xpcc::tcpip::TCPHeader::TCPHeader(uint8_t sender):
		type(Type::REGISTER), header()
{
	this->header.source = sender;
	std::cout<<"Type"<<sizeof(Type)<<std::endl;
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


xpcc::tcpip::TCPHeader&
xpcc::tcpip::Message::getTCPHeader()
{
	return this->header;
}

xpcc::tcpip::Message::Message(xpcc::Header& , SmartPointer payload):
		header(header), data(payload), dataLength(payload.getSize())
{

}


xpcc::tcpip::Message::Message(uint8_t identifier):
		header(identifier), data(), dataLength(0)
{
}


void
xpcc::tcpip::Message::encodeMessage()
{
	//deep copy message to char array
	char* header = reinterpret_cast<char*>(&this->getTCPHeader());
	char* content = this->data.get<char*>();
	for(int i = 0; i<xpcc::tcpip::TCPHeader::headerSize(); ++i)
	{
		this->dataStorage[i] = header[i];
	}
	for(int i = xpcc::tcpip::TCPHeader::headerSize(); i<xpcc::tcpip::TCPHeader::headerSize() + this->dataLength; ++i)
	{
		this->dataStorage[i] = content[i-xpcc::tcpip::TCPHeader::headerSize()];
	}
}

const char*
xpcc::tcpip::Message::getEncodedMessage() const
{
	return this->dataStorage;
}

bool
xpcc::tcpip::Message::decode(boost::shared_ptr<char> msg)
{

}


int
xpcc::tcpip::Message::getMessageLength() const
{
	std::cout<<"header: "<<xpcc::tcpip::TCPHeader::headerSize()<<std::endl;
	std::cout<<"dataLength: "<< this->dataLength<<std::endl;
	return xpcc::tcpip::TCPHeader::headerSize() + this->dataLength;
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
