//
// TCPServerConnection.cpp
//
// Library: Net
// Package: TCPServer
// Module:  TCPServerConnection
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "lucid/Net/TCPServerConnection.h"
#include "lucid/Exception.h"
#include "lucid/ErrorHandler.h"


using Lucid::Exception;
using Lucid::ErrorHandler;


namespace Lucid {
namespace Net {


TCPServerConnection::TCPServerConnection(const StreamSocket& socket):
	_socket(socket)
{
}


TCPServerConnection::~TCPServerConnection()
{
}


void TCPServerConnection::start()
{
	try
	{
		run();
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


} } // namespace Lucid::Net
