//
// StreamChannel.cpp
//
// Library: Foundation
// Package: Logging
// Module:  StreamChannel
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "lucid/StreamChannel.h"
#include "lucid/Message.h"


namespace Lucid {


StreamChannel::StreamChannel(std::ostream& str): _str(str)
{
}


StreamChannel::~StreamChannel()
{
}


void StreamChannel::log(const Message& msg)
{
	FastMutex::ScopedLock lock(_mutex);
	
	_str << msg.getText() << std::endl;
}


} // namespace Lucid
