//
// EventChannel.h
//
// Library: Foundation
// Package: Logging
// Module:  EventChannel
//
// Definition of the EventChannel class.
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_EventChannel_INCLUDED
#define Foundation_EventChannel_INCLUDED


#include "lucid/Foundation.h"
#include "lucid/Channel.h"
#include "lucid/Message.h"
#include "lucid/BasicEvent.h"


namespace Lucid {


class Foundation_API EventChannel: public Channel
	/// The EventChannel fires the messageLogged event for every log message
	/// received. This can be used to hook custom log message processing into
	/// the logging framework. 
{
public:
	using Ptr = AutoPtr<EventChannel>;

	Lucid::BasicEvent<const Message> messageLogged;
		/// Fired when a message is logged by calling the log() method.

	EventChannel();
		/// Creates the EventChannel.

	void log(const Message& msg);
		/// Fires the messageLogged event.

protected:
	~EventChannel();
		/// Destroys the EventChannel.
};


} // namespace Lucid


#endif // Foundation_EventChannel_INCLUDED
