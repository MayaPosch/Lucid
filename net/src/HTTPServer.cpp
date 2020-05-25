//
// HTTPServer.cpp
//
// Library: Net
// Package: HTTPServer
// Module:  HTTPServer
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "lucid/Net/HTTPServer.h"
#include "lucid/Net/HTTPServerConnectionFactory.h"


namespace Lucid {
namespace Net {


HTTPServer::HTTPServer(HTTPRequestHandlerFactory::Ptr pFactory, Lucid::UInt16 portNumber, HTTPServerParams::Ptr pParams):
	TCPServer(new HTTPServerConnectionFactory(pParams, pFactory), portNumber, pParams),
	_pFactory(pFactory)
{
}


HTTPServer::HTTPServer(HTTPRequestHandlerFactory::Ptr pFactory, const ServerSocket& socket, HTTPServerParams::Ptr pParams):
	TCPServer(new HTTPServerConnectionFactory(pParams, pFactory), socket, pParams),
	_pFactory(pFactory)
{
}


HTTPServer::HTTPServer(HTTPRequestHandlerFactory::Ptr pFactory, Lucid::ThreadPool& threadPool, const ServerSocket& socket, HTTPServerParams::Ptr pParams):
	TCPServer(new HTTPServerConnectionFactory(pParams, pFactory), threadPool, socket, pParams),
	_pFactory(pFactory)
{
}


HTTPServer::~HTTPServer()
{
}


void HTTPServer::stopAll(bool abortCurrent)
{
	stop();
	_pFactory->serverStopped(this, abortCurrent);
}


} } // namespace Lucid::Net
