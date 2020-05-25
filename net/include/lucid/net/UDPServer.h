//
// UDPServer.h
//
// Library: Net
// Package: UDP
// Module:  UDPServer
//
// Definition of the UDPServer class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_UDPServer_INCLUDED
#define Net_UDPServer_INCLUDED


#include "lucid/Net/Net.h"
#include "lucid/Net/DatagramSocket.h"
#include "lucid/Net/PollSet.h"
#include "lucid/Net/UDPHandler.h"
#include "lucid/Net/UDPServerParams.h"
#include "lucid/Net/UDPSocketReader.h"
#include "lucid/Net/SingleSocketPoller.h"
#include "lucid/Net/MultiSocketPoller.h"
#include <map>


namespace Lucid {
namespace Net {


template <std::size_t S = POCO_UDP_BUF_SIZE,
		typename P = SingleSocketPoller<S>>
class UDPServerImpl: public Lucid::Runnable
	/// UDP server, runs in its own thread and owns a poller, to which
	/// data arrival and discovery is delegated. See SingleSocketPoller and
	/// MultipleSocketPoller for more information.
{
public:
	UDPServerImpl(typename UDPHandlerImpl<S>::List& handlers, const Lucid::Net::SocketAddress& sa):
		_poller(handlers, sa),
		_thread("UDPServer"),
		_stop(false)
		/// Creates the UDPServer and binds it to
		/// the given address.
	{
		_thread.start(*this);
	}

	UDPServerImpl(typename UDPHandlerImpl<S>::List& handlers, const UDPServerParams& params):
		_poller(handlers, params),
		_thread("UDPServer"),
		_stop(false)
		/// Creates the UDPServer and binds it to
		/// the given address.
	{
		_thread.start(*this);
	}

	~UDPServerImpl()
		/// Destroys the UDPServer.
	{
		_stop = true;
		_poller.stop();
		while (!_poller.done()) Thread::sleep(10);
		_thread.join();
	}

	Lucid::UInt16 port() const
		/// Returns the port the server is
		/// listening on.
	{
		return _poller.port();
	}

	Lucid::Net::SocketAddress address() const
		/// Returns the address of the server.
	{
		return _poller.address();
	}

	void run()
		/// Does the work.
	{
		while (!_stop) _poller.poll();
	}

private:
	P            _poller;
	Lucid::Thread _thread;
	bool         _stop;
};


using UDPServer = UDPServerImpl<POCO_UDP_BUF_SIZE, SingleSocketPoller<POCO_UDP_BUF_SIZE>>;
using UDPMultiServer = UDPServerImpl<POCO_UDP_BUF_SIZE, MultiSocketPoller<POCO_UDP_BUF_SIZE>>;


} } // namespace Lucid::Net


#endif // Net_UDPServer_INCLUDED
