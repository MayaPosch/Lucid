//
// PipeImpl.h
//
// Library: Foundation
// Package: Processes
// Module:  PipeImpl
//
// Definition of the PipeImpl class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_PipeImpl_INCLUDED
#define Foundation_PipeImpl_INCLUDED


#include "lucid/Foundation.h"


#if defined(POCO_OS_FAMILY_WINDOWS)
#if defined(_WIN32_WCE)
#include "PipeImpl_DUMMY.h"
#else
#include "lucid/PipeImpl_WIN32.h"
#endif
#elif defined(POCO_OS_FAMILY_UNIX)
#include "lucid/PipeImpl_POSIX.h"
#else
#include "lucid/PipeImpl_DUMMY.h"
#endif


#endif // Foundation_PipeImpl_INCLUDED
