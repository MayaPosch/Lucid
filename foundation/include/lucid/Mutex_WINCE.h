//
// Mutex_WIN32.h
//
// Library: Foundation
// Package: Threading
// Module:  Mutex
//
// Definition of the MutexImpl and FastMutexImpl classes for WIN32.
//
// Copyright (c) 2004-2010, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Mutex_WINCE_INCLUDED
#define Foundation_Mutex_WINCE_INCLUDED


#include "lucid/Foundation.h"
#include "lucid/Exception.h"
#include "lucid/UnWindows.h"


namespace Lucid {


class Foundation_API MutexImpl
{
protected:
	MutexImpl();
	~MutexImpl();
	void lockImpl();
	bool tryLockImpl();
	bool tryLockImpl(long milliseconds);
	void unlockImpl();
	
private:
	HANDLE _mutex;
};


typedef MutexImpl FastMutexImpl;


} // namespace Lucid


#endif // Foundation_Mutex_WINCE_INCLUDED
