//
// TCPServerParams.cpp
//
// Library: Net
// Package: TCPServer
// Module:  TCPServerParams
//
// Copyright (c) 2005-2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "lucid/Net/TCPServerParams.h"


namespace Lucid {
namespace Net {


TCPServerParams::TCPServerParams():
	_threadIdleTime(10000000),
	_maxThreads(0),
	_maxQueued(64),
	_threadPriority(Lucid::Thread::PRIO_NORMAL)
{
}


TCPServerParams::~TCPServerParams()
{
}


void TCPServerParams::setThreadIdleTime(const Lucid::Timespan& milliseconds)
{
	_threadIdleTime = milliseconds;
}


void TCPServerParams::setMaxThreads(int count)
{
	poco_assert (count > 0);

	_maxThreads = count;
}


void TCPServerParams::setMaxQueued(int count)
{
	poco_assert (count >= 0);

	_maxQueued = count;
}


void TCPServerParams::setThreadPriority(Lucid::Thread::Priority prio)
{
	_threadPriority = prio;
}


} } // namespace Lucid::Net
