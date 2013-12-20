#include <xpcc/architecture/platform/hosted/tcpip/tcpip_server.hpp>


#include <xpcc/debug/logger.hpp>

// set new log level
#undef XPCC_LOG_LEVEL
#define	XPCC_LOG_LEVEL xpcc::log::DEBUG

//This program is a first test for the tcpip communication, still lots of work to be done

// i.e. create an instance of the generated postman

int
main()
{
	
    XPCC_LOG_INFO << "This is a test of the tcpip server!" << xpcc::endl;
	
    xpcc::tcpip::Server server(6666);

	while (1)
	{
        server.update();
	}
}
