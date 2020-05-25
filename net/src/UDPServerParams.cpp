//
// UDPServerParams.cpp
//
// Library: Net
// Package: UDP
// Module:  UDPServerParams
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "lucid/Net/UDPServerParams.h"


namespace Lucid {
namespace Net {


UDPServerParams::UDPServerParams(const Lucid::Net::SocketAddress& sa,
	int nSockets,
	Lucid::Timespan timeout,
	std::size_t handlerBufListSize,
	bool notifySender,
	int  backlogThreshold): _sa(sa),
		_nSockets(nSockets),
		_timeout(timeout),
		_handlerBufListSize(handlerBufListSize),
		_notifySender(notifySender),
		_backlogThreshold(backlogThreshold)
{
}


UDPServerParams::~UDPServerParams()
{
}


} } // namespace Lucid::Net
