//
// SingleSocketPoller.h
//
// Library: Net
// Package: UDP
// Module:  SingleSocketPoller
//
// Definition of the SingleSocketPoller class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_SingleSocketPoller_INCLUDED
#define Net_SingleSocketPoller_INCLUDED


#include "lucid/Net/Net.h"
#include "lucid/Net/DatagramSocket.h"
#include "lucid/Net/UDPHandler.h"
#include "lucid/Net/PollSet.h"


namespace Lucid {
namespace Net {


template <std::size_t S = POCO_UDP_BUF_SIZE>
class SingleSocketPoller
	/// SinlgeSocketPoller, as its name indicates, repeatedly polls a single
	/// socket for readability; if the socket is readable, the reading action
	/// is delegated to the reader.
{
public:
	SingleSocketPoller(typename UDPHandlerImpl<S>::List& handlers, const Lucid::Net::SocketAddress& sa, Lucid::Timespan timeout = 250000): _reader(handlers), _timeout(timeout)
		/// Creates the SingleSocketPoller and binds it to
		/// the given address.
	{
		_socket.bind(sa, false, false);
		_socket.setBlocking(false);
	}

	SingleSocketPoller(typename UDPHandlerImpl<S>::List& handlers, const UDPServerParams& serverParams): _reader(handlers, serverParams), _timeout(serverParams.timeout())
		/// Creates the SingleSocketPoller and binds it to
		/// the given address.
	{
		_socket.bind(serverParams.address(), false, false);
		_socket.setBlocking(false);
	}

	~SingleSocketPoller()
		/// Destroys SingleSocketPoller
	{
	}

	Lucid::UInt16 port() const
		/// Returns the port the socket is
		/// listening on.
	{
		return _socket.address().port();
	}

	Lucid::Net::SocketAddress address() const
		/// Returns the address of the server.
	{
		return _socket.address();
	}

	void poll()
		/// Poll the socket and read if readable.
	{
		if (_reader.handlerStopped()) return;
		if (_socket.poll(_timeout, Socket::SELECT_READ))
		{
			_reader.read(_socket);
		}
	}

	void stop()
		/// Stops the handler.
	{
		_reader.stopHandler();
	}

	bool done() const
		/// Returns tru if handler is done.
	{
		return _reader.handlerDone();
	}

private:
	Lucid::Net::DatagramSocket _socket;
	UDPSocketReader<S>        _reader;
	Lucid::Timespan            _timeout;
};


} } // namespace Lucid::Net


#endif // Net_SingleSocketPoller_INCLUDED
