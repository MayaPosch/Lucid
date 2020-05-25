//
// ParallelSocketReactor.h
//
// Library: Net
// Package: Reactor
// Module:  ParallelSocketReactor
//
// Definition of the ParallelSocketReactor class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_ParallelSocketReactor_INCLUDED
#define Net_ParallelSocketReactor_INCLUDED


#include "lucid/Net/SocketReactor.h"
#include "lucid/Net/SocketNotification.h"
#include "lucid/Net/StreamSocket.h"
#include "lucid/Net/ServerSocket.h"
#include "lucid/NObserver.h"
#include "lucid/Thread.h"
#include "lucid/SharedPtr.h"


using Lucid::Net::Socket;
using Lucid::Net::SocketReactor;
using Lucid::Net::ReadableNotification;
using Lucid::Net::ShutdownNotification;
using Lucid::Net::ServerSocket;
using Lucid::Net::StreamSocket;
using Lucid::NObserver;
using Lucid::AutoPtr;
using Lucid::Thread;


namespace Lucid {
namespace Net {


template <class SR>
class ParallelSocketReactor: public SR
{
public:
	using Ptr = Lucid::SharedPtr<ParallelSocketReactor>;

	ParallelSocketReactor()
	{
		_thread.start(*this);
	}
	
	ParallelSocketReactor(const Lucid::Timespan& timeout):
		SR(timeout)
	{
		_thread.start(*this);
	}
	
	~ParallelSocketReactor()
	{
		try
		{
			this->stop();
			_thread.join();
		}
		catch (...)
		{
			poco_unexpected();
		}
	}
	
protected:
	void onIdle()
	{
		SR::onIdle();
		Lucid::Thread::yield();
	}
	
private:
	Lucid::Thread _thread;
};


} } // namespace Lucid::Net


#endif // Net_ParallelSocketReactor_INCLUDED
