//
// NotificationCenter.cpp
//
// Library: Foundation
// Package: Notifications
// Module:  NotificationCenter
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "lucid/NotificationCenter.h"
#include "lucid/Notification.h"
#include "lucid/Observer.h"
#include "lucid/AutoPtr.h"
#include "lucid/SingletonHolder.h"


namespace Lucid {


NotificationCenter::NotificationCenter()
{
}


NotificationCenter::~NotificationCenter()
{
}


void NotificationCenter::addObserver(const AbstractObserver& observer)
{
	Mutex::ScopedLock lock(_mutex);
	_observers.push_back(observer.clone());
}


void NotificationCenter::removeObserver(const AbstractObserver& observer)
{
	Mutex::ScopedLock lock(_mutex);
	for (ObserverList::iterator it = _observers.begin(); it != _observers.end(); ++it)
	{
		if (observer.equals(**it))
		{
			(*it)->disable();
			_observers.erase(it);
			return;
		}
	}
}


bool NotificationCenter::hasObserver(const AbstractObserver& observer) const
{
	Mutex::ScopedLock lock(_mutex);
	for (const auto& p: _observers)
		if (observer.equals(*p)) return true;

	return false;
}


void NotificationCenter::postNotification(Notification::Ptr pNotification)
{
	poco_check_ptr (pNotification);

	ScopedLockWithUnlock<Mutex> lock(_mutex);
	ObserverList observersToNotify(_observers);
	lock.unlock();
	for (auto& p: observersToNotify)
	{
		p->notify(pNotification);
	}
}


bool NotificationCenter::hasObservers() const
{
	Mutex::ScopedLock lock(_mutex);

	return !_observers.empty();
}


std::size_t NotificationCenter::countObservers() const
{
	Mutex::ScopedLock lock(_mutex);

	return _observers.size();
}


namespace
{
	static SingletonHolder<NotificationCenter> sh;
}


NotificationCenter& NotificationCenter::defaultCenter()
{
	return *sh.get();
}


} // namespace Lucid
