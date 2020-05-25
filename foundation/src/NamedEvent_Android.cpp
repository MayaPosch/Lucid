//
// NamedEvent_Android.cpp
//
// Library: Foundation
// Package: Processes
// Module:  NamedEvent
//
// Copyright (c) 2004-2011, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "lucid/NamedEvent_Android.h"
#include "lucid/Exception.h"


namespace Lucid {


NamedEventImpl::NamedEventImpl(const std::string&)
{
}


NamedEventImpl::~NamedEventImpl()
{
}


void NamedEventImpl::setImpl()
{
	throw NotImplementedException("NamedEvent::set() not available on Android");
}


void NamedEventImpl::waitImpl()
{
	throw NotImplementedException("NamedEvent::wait() not available on Android");
}


} // namespace Lucid
