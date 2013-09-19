// coding: utf-8
// ----------------------------------------------------------------------------
/* Copyright (c) 2013, Roboterclub Aachen e.V.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Roboterclub Aachen e.V. nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ROBOTERCLUB AACHEN E.V. ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ROBOTERCLUB AACHEN E.V. BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
// ----------------------------------------------------------------------------
#ifndef XPCC_TCPIP__CLIENT_HPP
#define XPCC_TCPIP__CLIENT_HPP

//-----------------------------------------------------------------------------
//boost lib includes
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>
//-----------------------------------------------------------------------------
//stl lib includes
#include <list>
#include <string>
#include <map>

//-----------------------------------------------------------------------------
//xpcc dependencies
#include <xpcc/workflow/periodic_timer.hpp>
#include <xpcc/architecture/platform/hosted/tcpip/tcpip_message.hpp>



namespace xpcc
{
	namespace tcpip
	{

	    /**
         * The Client sends all actions and events to the Server and receives
         * all responses.
         * The client consists of several threads:
         * One thread is reserved for for the io_service
         * One thread is used for sending packages to the server
         * And one thread for receiving data from each component
	     *
	     *  \author Thorsten Lajewski
	     */
		class Client
		{
		public:

			Client(std::string ip);

			//~Client();

			void connect();

			void addComponent(uint8_t id);

			//the server has to receive ping from each component
			void sendAlivePing(int identifier);

			//send a xpcc packet to the server
			void sendPacket(const xpcc::tcpip::Message &msg);

			//get a pointer to the last message in the receivedMessages list
			boost::shared_ptr<xpcc::tcpip::Message> getMessage();

			//remove last Message from the receivedMessages list
			void deleteMessage();

			//timer need to be checked continously
			void update();

			//close the tcpip connection
			void disconnect();

		private:

			void connect_handler(const boost::system::error_code& error);

			void receive_header_handler(const boost::system::error_code& error);

			void receive_data_handler(const boost::system::error_code& error);

			void close();

			void registerComponents();

			struct ComponentInfo{
				ComponentInfo(uint8_t id):
						identifier(id),
						timer(200)
				{
				}

				uint8_t identifier;
				xpcc::PeriodicTimer<> timer;

			};

			bool connected;
			bool writingMessages;
			bool receivingMessages;
			bool closeConnection;

			boost::shared_ptr< boost::asio::io_service >  ioService;
			boost::shared_ptr< boost::asio::io_service::work > work;

			//send connection to the server
			boost::asio::ip::tcp::resolver::iterator endpointIter;
			boost::shared_ptr<boost::asio::ip::tcp::socket> sendSocket;

			boost::shared_ptr< boost::thread > serviceThread;
			boost::shared_ptr< boost::thread > sentThread;
			std::list< boost::shared_ptr< boost::thread > > receiveThreadPool;

			std::map< uint8_t, boost::shared_ptr<ComponentInfo> > componentMap;
			//std::list< boost::shared_ptr<xpcc::tcpip::Message > > receivedMessages;
			std::list< boost::shared_ptr<xpcc::tcpip::Message> > messagesToBeSent;

		};
	}
}
#endif // XPCC_TCPIP__CLIENt_HPP
