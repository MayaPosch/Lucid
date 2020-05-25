//
// UDPServerParams.h
//
// Library: Net
// Package: UDP
// Module:  UDPServerParams
//
// Definition of the UDPServerParams class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_UDPServerParams_INCLUDED
#define Net_UDPServerParams_INCLUDED


#include "lucid/Net/Net.h"
#include "lucid/Net/SocketAddress.h"
#include "lucid/Timespan.h"


namespace Lucid {
namespace Net {


class Net_API UDPServerParams
	/// A class encapsulating UDP server parameters.
{
public:
	UDPServerParams(const Lucid::Net::SocketAddress& sa,
		int nSockets = 10,
		Lucid::Timespan timeout = 250000,
		std::size_t handlerBufListSize = 1000,
		bool notifySender = false,
		int  backlogThreshold = 10);
		/// Creates UDPServerParams.

	~UDPServerParams();
		/// Destroys UDPServerParams.

	const Lucid::Net::SocketAddress& address() const;
		/// Returns the server address.

	Lucid::Timespan timeout() const;
		/// Returns polling timeout.

	int numberOfSockets() const;
		/// Returns nuber of sockets for the server.

	std::size_t handlerBufListSize() const;
		/// Returns the number of handler buffers allocated
		/// before buffers start being reused.

	bool notifySender() const;
		/// Returns the flag inidcating whether server
		/// should send notifications back to the client.

	int backlogThreshold() const;
		/// Size of mesage or error backlogs at which server
		/// reports backlogs back to the client. Only meaningful
		/// if notifySender() is true.

private:
	UDPServerParams();

	Lucid::Net::SocketAddress _sa;
	int                      _nSockets;
	Lucid::Timespan           _timeout;
	std::size_t              _handlerBufListSize;
	bool                     _notifySender;
	int                      _backlogThreshold;
};


//
// inlines
//

inline const Lucid::Net::SocketAddress& UDPServerParams::address() const
{
	return _sa;
}


inline int UDPServerParams::numberOfSockets() const
{
	return _nSockets;
}


inline Lucid::Timespan UDPServerParams::timeout() const
{
	return _timeout;
}


inline std::size_t UDPServerParams::handlerBufListSize() const
{
	return _handlerBufListSize;
}


inline bool UDPServerParams::notifySender() const
{
	return _notifySender;
}


inline int UDPServerParams::backlogThreshold() const
{
	return _backlogThreshold;
}


} } // namespace Lucid::Net


#endif // Net_UDPServerParams_INCLUDED
