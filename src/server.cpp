/**********************************************************************
 * Copyright 2013 Georg Rudoy 0xd34df00d@gmail.com
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 **********************************************************************/

#include "server.h"
#include <thread>
#include <iostream>
#include "clientconnection.h"

namespace Laretz
{
	namespace ip = boost::asio::ip;

	Server::Server ()
	: m_acceptor (m_io)
	{
		std::string address = "127.0.0.1";
		ip::tcp::resolver resolver (m_io);
		ip::tcp::endpoint ep = *resolver.resolve (ip::tcp::resolver::query (address, "54093"));

		m_acceptor.open (ep.protocol());
		m_acceptor.set_option (ip::tcp::acceptor::reuse_address (true));
		m_acceptor.bind (ep);
		m_acceptor.listen ();

		startAccept ();
	}

	void Server::run ()
	{
		std::vector<std::thread> threads;
		for (size_t i = 0; i < std::max<size_t> (std::thread::hardware_concurrency (), 2); ++i)
			threads.emplace_back([this] () { m_io.run (); });

		for (auto& t : threads)
			t.join ();
	}

	void Server::startAccept ()
	{
		m_conn.reset (new ClientConnection (m_io));
		m_acceptor.async_accept (m_conn->getSocket (),
				[this] (const boost::system::error_code& ec) { handleAccept (ec); });
	}

	void Server::handleAccept (const boost::system::error_code& ec)
	{
		if (!ec)
			m_conn->start ();

		startAccept ();
	}
}
