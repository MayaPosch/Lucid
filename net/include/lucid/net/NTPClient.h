//
// NTPClient.h
//
// Library: Net
// Package: NTP
// Module:  NTPClient
//
// Definition of the NTPClient class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_NTPClient_INCLUDED
#define Net_NTPClient_INCLUDED


#include "lucid/Net/Net.h"
#include "lucid/Net/NTPEventArgs.h"
#include "lucid/Net/SocketAddress.h"
#include "lucid/BasicEvent.h"


namespace Lucid {
namespace Net {


class Net_API NTPClient
	/// This class provides NTP (Network Time Protocol) client functionality.
{
public:
	mutable Lucid::BasicEvent<NTPEventArgs> response;

	explicit NTPClient(SocketAddress::Family family, int timeout = 3000000);
		/// Creates an NTP client.

	~NTPClient();
		/// Destroys the NTP client.

	int request(SocketAddress& address) const;
		/// Request the time from the server at address.
		/// Notifications are posted for events.
		/// 
		/// Returns the number of valid replies.

	int request(const std::string& address) const;
		/// Request the time from the server at address.
		/// Notifications are posted for events.
		/// 
		/// Returns the number of valid replies.

private:
	mutable SocketAddress::Family _family;
	int _timeout;
};


} } // namespace Lucid::Net


#endif // Net_NTPClient_INCLUDED
