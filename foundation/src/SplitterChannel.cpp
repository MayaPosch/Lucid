//
// SplitterChannel.cpp
//
// Library: Foundation
// Package: Logging
// Module:  SplitterChannel
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "lucid/SplitterChannel.h"
#include "lucid/LoggingRegistry.h"
#include "lucid/StringTokenizer.h"


namespace Lucid {


SplitterChannel::SplitterChannel()
{
}


SplitterChannel::~SplitterChannel()
{
	try
	{
		close();
	}
	catch (...)
	{
		poco_unexpected();
	}
}


void SplitterChannel::addChannel(Channel::Ptr pChannel)
{
	poco_check_ptr (pChannel);

	FastMutex::ScopedLock lock(_mutex);
	_channels.push_back(pChannel);
}


void SplitterChannel::removeChannel(Channel::Ptr pChannel)
{
	FastMutex::ScopedLock lock(_mutex);

	for (ChannelVec::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		if (*it == pChannel)
		{
			_channels.erase(it);
			break;
		}
	}
}


void SplitterChannel::setProperty(const std::string& name, const std::string& value)
{
	if (name.compare(0, 7, "channel") == 0)
	{
		StringTokenizer tokenizer(value, ",;", StringTokenizer::TOK_IGNORE_EMPTY | StringTokenizer::TOK_TRIM);
		for (StringTokenizer::Iterator it = tokenizer.begin(); it != tokenizer.end(); ++it)
		{
			addChannel(LoggingRegistry::defaultRegistry().channelForName(*it));
		}
	}
	else Channel::setProperty(name, value);
}


void SplitterChannel::log(const Message& msg)
{
	FastMutex::ScopedLock lock(_mutex);

	for (auto& p: _channels)
	{
		p->log(msg);
	}
}


void SplitterChannel::close()
{
	FastMutex::ScopedLock lock(_mutex);
	_channels.clear();
}


int SplitterChannel::count() const
{
	FastMutex::ScopedLock lock(_mutex);
	
	return (int) _channels.size();
}


} // namespace Lucid