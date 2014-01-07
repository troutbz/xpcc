#include <xpcc/architecture/platform/hosted/tcpip/tcpip_client.hpp>
#include <xpcc/processing/periodic_timer.hpp>
#include <xpcc/debug/logger.hpp>
#include <stdint.h>

#include <boost/shared_ptr.hpp>

// set new log level
#undef XPCC_LOG_LEVEL
#define	XPCC_LOG_LEVEL xpcc::log::DEBUG

//This program is a first test for the tcpip communication, still lots of work to be done

// create an instance of the generated postman

int
main()
{
	
    XPCC_LOG_INFO << "This is a test of the tcpip client!" << xpcc::endl;
	
    xpcc::tcpip::Client client("127.0.0.1", 6666);
    xpcc::PeriodicTimer<> timer(500);
    uint8_t counter = 0;
    XPCC_LOG_INFO << "Adding first component!" << xpcc::endl;
    client.addComponent(11);
    XPCC_LOG_INFO << "Adding second component!" << xpcc::endl;
    client.addComponent(14);
	while (1)
	{
        client.update();
        //send message every second
        if(timer.isExpired())
        {
            xpcc::Header header;
            header.type = xpcc::Header::REQUEST;
            header.source = 11;
            header.destination = 14;
            header.packetIdentifier = 1;

            xpcc::SmartPointer payload(&counter);
            boost::shared_ptr<xpcc::tcpip::Message> msg(new xpcc::tcpip::Message(header, payload));

            client.sendPacket(msg);
            counter++;
            //timer.stop();
        }
        //check for received messages
        if(client.isMessageAvailable())
        {
            boost::shared_ptr<xpcc::tcpip::Message>  packet = client.getMessage();

            XPCC_LOG_INFO << "Component "<< (int) packet->getXpccHeader().destination << " received packet of type "<<
                             packet->getXpccHeader().packetIdentifier << " from Component "<< (int) packet->getXpccHeader().source<<
                             " with payload "<< packet->getMessagePayload().get<uint8_t>() <<xpcc::endl;

        }
	}
}
