//
// SocketNotifier.h
//
// Library: Net
// Package: Reactor
// Module:  SocketNotifier
//
// Definition of the SocketNotifier class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_SocketNotifier_INCLUDED
#define Net_SocketNotifier_INCLUDED


#include "lucid/Net/Net.h"
#include "lucid/Net/Socket.h"
#include "lucid/RefCountedObject.h"
#include "lucid/NotificationCenter.h"
#include "lucid/Observer.h"
#include <set>


namespace Lucid {
namespace Net {


class Socket;
class SocketReactor;
class SocketNotification;


class Net_API SocketNotifier: public Lucid::RefCountedObject
	/// This class is used internally by SocketReactor
	/// to notify registered event handlers of socket events.
{
public:
	explicit SocketNotifier(const Socket& socket);
		/// Creates the SocketNotifier for the given socket.
		
	void addObserver(SocketReactor* pReactor, const Lucid::AbstractObserver& observer);
		/// Adds the given observer. 
		
	void removeObserver(SocketReactor* pReactor, const Lucid::AbstractObserver& observer);
		/// Removes the given observer. 
		
	bool hasObserver(const Lucid::AbstractObserver& observer) const;
		/// Returns true if the given observer is registered.
		
	bool accepts(SocketNotification* pNotification);
		/// Returns true if there is at least one observer for the given notification.
		
	void dispatch(SocketNotification* pNotification);
		/// Dispatches the notification to all observers.
		
	bool hasObservers() const;
		/// Returns true if there are subscribers.
		
	std::size_t countObservers() const;
		/// Returns the number of subscribers;

protected:
	~SocketNotifier();
		/// Destroys the SocketNotifier.

private:
	typedef std::multiset<SocketNotification*> EventSet;
	typedef Lucid::FastMutex                    MutexType;
	typedef MutexType::ScopedLock              ScopedLock;

	EventSet                 _events;
	Lucid::NotificationCenter _nc;
	Socket                   _socket;
	MutexType                _mutex;
};


//
// inlines
//
inline bool SocketNotifier::accepts(SocketNotification* pNotification)
{
	ScopedLock l(_mutex);
	return _events.find(pNotification) != _events.end();
}


inline bool SocketNotifier::hasObserver(const Lucid::AbstractObserver& observer) const
{
	return _nc.hasObserver(observer);
}


inline bool SocketNotifier::hasObservers() const
{
	return _nc.hasObservers();
}


inline std::size_t SocketNotifier::countObservers() const
{
	return _nc.countObservers();
}


} } // namespace Lucid::Net


#endif // Net_SocketNotifier_INCLUDED
