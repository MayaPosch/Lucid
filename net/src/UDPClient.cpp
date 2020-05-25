//
// UDPClient.cpp
//
// Library: Net
// Package: UDP
// Module:  UDPClient
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "lucid/Net/UDPClient.h"
#include "lucid/ErrorHandler.h"


namespace Lucid {
namespace Net {


UDPClient::UDPClient(const std::string& address, Lucid::UInt16 port, bool listen):
	_address(address, port),
	_pThread(0),
	_stop(false)
{
	_socket.bind(SocketAddress(address, 0), true, true);
	_socket.setReuseAddress(true);
	_socket.setReusePort(true);
	_socket.connect(_address);
	_socket.setBlocking(true);
	if (listen)
	{
		_pThread = new Thread;
		_pThread->start(*this);
	}
}


UDPClient::~UDPClient()
{
	_stop = true;
	if (_pThread)
	{
		_pThread->join();
		delete _pThread;
	}
}


void UDPClient::run()
{
	Lucid::Timespan span(250000);
	while (!_stop)
	{
		if (_socket.poll(span, Socket::SELECT_READ))
		{
			try
			{
				char buffer[sizeof(Lucid::UInt32)*2];
				int n = _socket.receiveBytes(buffer, sizeof(buffer));
				if (n >= sizeof(Lucid::Int32)) handleResponse(buffer, n);
			}
			catch (Exception& exc)
			{
				ErrorHandler::handle(exc);
			}
			catch (std::exception& exc)
			{
				ErrorHandler::handle(exc);
			}
			catch (...)
			{
				ErrorHandler::handle();
			}
		}
	}
}


int UDPClient::handleResponse(char* buffer, int length)
{
	Lucid::Int32 count = *reinterpret_cast<Lucid::Int32*>(buffer);
	if (count > 0) _dataBacklog = count;
	else if (count < 0) _errorBacklog = count;
	return count;
}


} } // namespace Lucid::Net
