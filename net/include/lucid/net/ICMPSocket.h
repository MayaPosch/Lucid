//
// ICMPSocket.h
//
// Library: Net
// Package: ICMP
// Module:  ICMPSocket
//
// Definition of the ICMPSocket class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_ICMPSocket_INCLUDED
#define Net_ICMPSocket_INCLUDED


#include "lucid/Net/Net.h"
#include "lucid/Net/Socket.h"


namespace Lucid {
namespace Net {


class Net_API ICMPSocket: public Socket
	/// This class provides an interface to an
	/// ICMP client socket.
{
public:
	ICMPSocket(SocketAddress::Family family, int dataSize = 48, int ttl = 128, int timeout = 5000000);
		/// Creates an unconnected ICMP socket.
		///
		/// The socket will be created for the
		/// given address family.

	ICMPSocket(const Socket& socket);
		/// Creates the ICMPSocket with the SocketImpl
		/// from another socket. The SocketImpl must be
		/// a ICMPSocketImpl, otherwise an InvalidArgumentException
		/// will be thrown.

	~ICMPSocket();
		/// Destroys the ICMPSocket.

	ICMPSocket& operator = (const Socket& socket);
		/// Assignment operator.
		///
		/// Releases the socket's SocketImpl and
		/// attaches the SocketImpl from the other socket and
		/// increments the reference count of the SocketImpl.	

	int sendTo(const SocketAddress& address, int flags = 0);
		/// Sends an ICMP request through
		/// the socket to the given address.
		///
		/// Returns the number of bytes sent.

	int receiveFrom(SocketAddress& address, int flags = 0);
		/// Receives data from the socket.
		/// Stores the address of the sender in address.
		///
		/// Returns the time elapsed since the originating 
		/// request was sent.

	int dataSize() const;
		/// Returns the data size in bytes.

	int packetSize() const;
		/// Returns the packet size in bytes.

	int ttl() const;
		/// Returns the Time-To-Live value.

	int timeout() const;
		/// Returns the socket timeout value.

	static Lucid::UInt16 mtu(const SocketAddress& address, Lucid::UInt16 sz);
		/// Returns minimum payload path MTU size for the destination,
		/// or 0 if MTU can not be determined.

protected:
	ICMPSocket(SocketImpl* pImpl);
		/// Creates the Socket and attaches the given SocketImpl.
		/// The socket takes ownership of the SocketImpl.
		///
		/// The SocketImpl must be a ICMPSocketImpl, otherwise
		/// an InvalidArgumentException will be thrown.
};


} } // namespace Lucid::Net


#endif // Net_ICMPSocket_INCLUDED
