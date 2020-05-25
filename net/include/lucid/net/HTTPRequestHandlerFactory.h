//
// HTTPRequestHandlerFactory.h
//
// Library: Net
// Package: HTTPServer
// Module:  HTTPRequestHandlerFactory
//
// Definition of the HTTPRequestHandlerFactory class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_HTTPRequestHandlerFactory_INCLUDED
#define Net_HTTPRequestHandlerFactory_INCLUDED


#include "lucid/Net/Net.h"
#include "lucid/SharedPtr.h"
#include "lucid/BasicEvent.h"


namespace Lucid {
namespace Net {


class HTTPServerRequest;
class HTTPServerResponse;
class HTTPRequestHandler;


class Net_API HTTPRequestHandlerFactory
	/// A factory for HTTPRequestHandler objects.
	/// Subclasses must override the createRequestHandler()
	/// method.
{
public:
	using Ptr = Lucid::SharedPtr<HTTPRequestHandlerFactory>;
	
	HTTPRequestHandlerFactory();
		/// Creates the HTTPRequestHandlerFactory.

	virtual ~HTTPRequestHandlerFactory();
		/// Destroys the HTTPRequestHandlerFactory.

	virtual HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request) = 0;
		/// Must be overridden by subclasses.
		///
		/// Creates a new request handler for the given HTTP request.
		///
		/// The method should inspect the given HTTPServerRequest object (e.g., method
		/// and URI) and create an appropriate HTTPRequestHandler object to handle the
		/// request.
		///
		/// If the request contains a "Expect: 100-continue" header, it's possible
		/// to prevent the server from sending the default 100 Continue response 
		/// by setting the status of the response object that can be obtained through 
		/// the request object (request.response()) to something other than 200 OK.

protected:
	Lucid::BasicEvent<const bool> serverStopped;

private:
	HTTPRequestHandlerFactory(const HTTPRequestHandlerFactory&);
	HTTPRequestHandlerFactory& operator = (const HTTPRequestHandlerFactory&);
	
	friend class HTTPServer;
	friend class HTTPServerConnection;
};


} } // namespace Lucid::Net


#endif // Net_HTTPRequestHandlerFactory_INCLUDED
