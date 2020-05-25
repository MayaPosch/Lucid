//
// NamedMutex_WIN32.cpp
//
// Library: Foundation
// Package: Processes
// Module:  NamedMutex
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "lucid/NamedMutex_WIN32U.h"
#include "lucid/Exception.h"
#include "lucid/UnicodeConverter.h"


namespace Lucid {


NamedMutexImpl::NamedMutexImpl(const std::string& name):
	_name(name)
{
	UnicodeConverter::toUTF16(_name, _uname);
	_mutex = CreateMutexW(NULL, FALSE, _uname.c_str());
	if (!_mutex) 
		throw SystemException("cannot create named mutex", _name);
}


NamedMutexImpl::~NamedMutexImpl()
{
	CloseHandle(_mutex);
}


void NamedMutexImpl::lockImpl()
{
	switch (WaitForSingleObject(_mutex, INFINITE))
	{
	case WAIT_OBJECT_0:
		return;
	case WAIT_ABANDONED:
		throw SystemException("cannot lock named mutex (abadoned)", _name);
	default:
		throw SystemException("cannot lock named mutex", _name);
	}
}


bool NamedMutexImpl::tryLockImpl()
{
	switch (WaitForSingleObject(_mutex, 0))
	{
	case WAIT_OBJECT_0:
		return true;
	case WAIT_TIMEOUT:
		return false;
	case WAIT_ABANDONED:
		throw SystemException("cannot lock named mutex (abadoned)", _name);
	default:
		throw SystemException("cannot lock named mutex", _name);
	}
}


void NamedMutexImpl::unlockImpl()
{
	ReleaseMutex(_mutex);
}


} // namespace Lucid
