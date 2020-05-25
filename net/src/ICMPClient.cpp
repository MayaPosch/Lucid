//
// ICMPClient.cpp
//
// Library: Net
// Package: ICMP
// Module:  ICMPClient
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "lucid/Net/SocketAddress.h"
#include "lucid/Net/ICMPClient.h"
#include "lucid/Net/NetException.h"
#include "lucid/Channel.h"
#include "lucid/Message.h"
#include "lucid/Format.h"
#include <sstream>


using Lucid::Channel;
using Lucid::Message;
using Lucid::InvalidArgumentException;
using Lucid::NotImplementedException;
using Lucid::TimeoutException;
using Lucid::Exception;


namespace Lucid {
namespace Net {


ICMPClient::ICMPClient(SocketAddress::Family family, int dataSize, int ttl, int timeout):
	_family(family),
	_dataSize(dataSize),
	_ttl(ttl),
	_timeout(timeout)
{
}


ICMPClient::~ICMPClient()
{
}


int ICMPClient::ping(const std::string& address, int repeat) const
{
	if (repeat <= 0) return 0;

	SocketAddress addr(address, 0);
	return ping(addr, repeat);
}


int ICMPClient::ping(SocketAddress& address, int repeat) const
{
	if (repeat <= 0) return 0;

	ICMPSocket icmpSocket(_family, _dataSize, _ttl, _timeout);

	ICMPEventArgs eventArgs(address, repeat, icmpSocket.dataSize(), icmpSocket.ttl());
	pingBegin.notify(this, eventArgs);

	for (int i = 0; i < repeat; ++i)
	{
		try
		{
			int sent = icmpSocket.sendTo(address);
			if (icmpSocket.packetSize() == sent)
			{
				SocketAddress requestAddress(address);
				++eventArgs;
				int t = icmpSocket.receiveFrom(address);
				poco_assert (address.host() == requestAddress.host());
				eventArgs.setReplyTime(i, t);
				pingReply.notify(this, eventArgs);
			}
			else
				throw ICMPException(Lucid::format("Error sending ICMP packet "
					"(sent=%d, expected=%d)", sent, icmpSocket.packetSize()));
		}
		catch (TimeoutException&)
		{
			std::ostringstream os;
			os << address.host().toString() << ": Request timed out.";
			eventArgs.setError(i, os.str());
			pingError.notify(this, eventArgs);
			continue;
		}
		catch (ICMPException& ex)
		{
			std::ostringstream os;
			os << address.host().toString() << ": " << ex.displayText();
			eventArgs.setError(i, os.str());
			pingError.notify(this, eventArgs);
			continue;
		}
		catch (Exception& ex)
		{
			eventArgs.setError(i, ex.displayText());
			pingError.notify(this, eventArgs);
			continue;
		}
	}
	pingEnd.notify(this, eventArgs);
	return eventArgs.received();
}


int ICMPClient::pingIPv4(const std::string& address, int repeat,
	int dataSize, int ttl, int timeout)
{
	if (repeat <= 0) return 0;

	SocketAddress a(address, 0);
	return ping(a, IPAddress::IPv4, repeat, dataSize, ttl, timeout);
}


int ICMPClient::ping(SocketAddress& address,
	IPAddress::Family family, int repeat,
	int dataSize, int ttl, int timeout)
{
	if (repeat <= 0) return 0;

	ICMPSocket icmpSocket(family, dataSize, ttl, timeout);
	int received = 0;

	for (int i = 0; i < repeat; ++i)
	{
		try
		{
			SocketAddress requestAddress(address);
			if (icmpSocket.sendTo(address) == icmpSocket.packetSize())
			{
				icmpSocket.receiveFrom(address);
				poco_assert (address.host() == requestAddress.host());
				++received;
			}
		}
		catch (Exception&) { }
	}
	return received;
}


} } // namespace Lucid::Net
