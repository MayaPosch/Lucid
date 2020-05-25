//
// NamedEvent.cpp
//
// Library: Foundation
// Package: Processes
// Module:  NamedEvent
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "lucid/NamedEvent.h"


#if defined(POCO_OS_FAMILY_WINDOWS)
#include "NamedEvent_WIN32U.cpp"
#elif POCO_OS == POCO_OS_ANDROID
#include "NamedEvent_Android.cpp"
#elif defined(POCO_OS_FAMILY_UNIX)
#include "NamedEvent_UNIX.cpp"
#endif


namespace Lucid {


NamedEvent::NamedEvent(const std::string& name):
	NamedEventImpl(name)
{
}


NamedEvent::~NamedEvent()
{
}


} // namespace Lucid
