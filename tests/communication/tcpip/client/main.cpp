
#include <xpcc/architecture/platform/hosted/tcpip/tcpip_client.hpp>


#include <xpcc/debug/logger.hpp>

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
    XPCC_LOG_INFO << "Adding first component!" << xpcc::endl;
    client.addComponent(11);
    XPCC_LOG_INFO << "Adding second component!" << xpcc::endl;
    client.addComponent(14);
	while (1)
	{
        client.update();
	}
}
