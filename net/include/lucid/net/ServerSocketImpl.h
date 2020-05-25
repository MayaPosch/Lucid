//
// ServerSocketImpl.h
//
// Library: Net
// Package: Sockets
// Module:  ServerSocketImpl
//
// Definition of the ServerSocketImpl class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_ServerSocketImpl_INCLUDED
#define Net_ServerSocketImpl_INCLUDED


#include "lucid/Net/Net.h"
#include "lucid/Net/SocketImpl.h"


namespace Lucid {
namespace Net {


class Net_API ServerSocketImpl: public SocketImpl
	/// This class implements a TCP server socket.
{
public:
	ServerSocketImpl();
		/// Creates the ServerSocketImpl.

protected:
	virtual ~ServerSocketImpl();
		/// Destroys the ServerSocketImpl.
};


} } // namespace Lucid::Net


#endif // Net_ServerSocketImpl_INCLUDED
