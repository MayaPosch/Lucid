//
// HTTPServerConnection.h
//
// Library: Net
// Package: HTTPServer
// Module:  HTTPServerConnection
//
// Definition of the HTTPServerConnection class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_HTTPServerConnection_INCLUDED
#define Net_HTTPServerConnection_INCLUDED


#include "lucid/Net/Net.h"
#include "lucid/Net/TCPServerConnection.h"
#include "lucid/Net/HTTPResponse.h"
#include "lucid/Net/HTTPRequestHandlerFactory.h"
#include "lucid/Net/HTTPServerParams.h"
#include "lucid/Mutex.h"


namespace Lucid {
namespace Net {


class HTTPServerSession;


class Net_API HTTPServerConnection: public TCPServerConnection
	/// This subclass of TCPServerConnection handles HTTP
	/// connections.
{
public:
	HTTPServerConnection(const StreamSocket& socket, HTTPServerParams::Ptr pParams, HTTPRequestHandlerFactory::Ptr pFactory);
		/// Creates the HTTPServerConnection.

	virtual ~HTTPServerConnection();
		/// Destroys the HTTPServerConnection.
		
	void run();
		/// Handles all HTTP requests coming in.

protected:
	void sendErrorResponse(HTTPServerSession& session, HTTPResponse::HTTPStatus status);
	void onServerStopped(const bool& abortCurrent);

private:
	HTTPServerParams::Ptr          _pParams;
	HTTPRequestHandlerFactory::Ptr _pFactory;
	bool _stopped;
	Lucid::FastMutex _mutex;
};


} } // namespace Lucid::Net


#endif // Net_HTTPServerConnection_INCLUDED
