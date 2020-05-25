//
// SocketNotification.h
//
// Library: Net
// Package: Reactor
// Module:  SocketNotification
//
// Definition of the SocketNotification class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_SocketNotification_INCLUDED
#define Net_SocketNotification_INCLUDED


#include "lucid/Net/Net.h"
#include "lucid/Net/Socket.h"
#include "lucid/Notification.h"


namespace Lucid {
namespace Net {


class SocketReactor;


class Net_API SocketNotification: public Lucid::Notification
	/// The base class for all notifications generated by
	/// the SocketReactor.
{
public:
	explicit SocketNotification(SocketReactor* pReactor);
		/// Creates the SocketNotification for the given SocketReactor.

	virtual ~SocketNotification();
		/// Destroys the SocketNotification.
		
	SocketReactor& source() const;
		/// Returns the SocketReactor that generated the notification.	
		
	Socket socket() const;
		/// Returns the socket that caused the notification.

private:
	void setSocket(const Socket& socket);
	
	SocketReactor* _pReactor;
	Socket         _socket;
	
	friend class SocketNotifier;
};


class Net_API ReadableNotification: public SocketNotification
	/// This notification is sent if a socket has become readable.
{
public:
	ReadableNotification(SocketReactor* pReactor);
		/// Creates the ReadableNotification for the given SocketReactor.

	~ReadableNotification();
		/// Destroys the ReadableNotification.
};


class Net_API WritableNotification: public SocketNotification
	/// This notification is sent if a socket has become writable.
{
public:
	WritableNotification(SocketReactor* pReactor);
		/// Creates the WritableNotification for the given SocketReactor.

	~WritableNotification();
		/// Destroys the WritableNotification.
};


class Net_API ErrorNotification: public SocketNotification
	/// This notification is sent if a socket has signalled an error.
{
public:
	ErrorNotification(SocketReactor* pReactor);
		/// Creates the ErrorNotification for the given SocketReactor.

	~ErrorNotification();
		/// Destroys the ErrorNotification.
};


class Net_API TimeoutNotification: public SocketNotification
	/// This notification is sent if no other event has occurred
	/// for a specified time.
{
public:
	TimeoutNotification(SocketReactor* pReactor);
		/// Creates the TimeoutNotification for the given SocketReactor.

	~TimeoutNotification();
		/// Destroys the TimeoutNotification.
};


class Net_API IdleNotification: public SocketNotification
	/// This notification is sent when the SocketReactor does
	/// not have any sockets to react to.
{
public:
	IdleNotification(SocketReactor* pReactor);
		/// Creates the IdleNotification for the given SocketReactor.

	~IdleNotification();
		/// Destroys the IdleNotification.
};


class Net_API ShutdownNotification: public SocketNotification
	/// This notification is sent when the SocketReactor is
	/// about to shut down.
{
public:
	ShutdownNotification(SocketReactor* pReactor);
		/// Creates the ShutdownNotification for the given SocketReactor.

	~ShutdownNotification();
		/// Destroys the ShutdownNotification.
};


//
// inlines
//
inline SocketReactor& SocketNotification::source() const
{
	return *_pReactor;
}

	
inline Socket SocketNotification::socket() const
{
	return _socket;
}


} } // namespace Lucid::Net


#endif // Net_SocketNotification_INCLUDED
