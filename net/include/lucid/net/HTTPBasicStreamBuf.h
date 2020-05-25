//
// HTTPBasicStreamBuf.h
//
// Library: Net
// Package: HTTP
// Module:  HTTPBasicStreamBuf
//
// Definition of the HTTPBasicStreamBuf class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_HTTPBasicStreamBuf_INCLUDED
#define Net_HTTPBasicStreamBuf_INCLUDED


#include "lucid/Net/Net.h"
#include "lucid/BufferedStreamBuf.h"
#include "lucid/Net/HTTPBufferAllocator.h"


namespace Lucid {
namespace Net {


using HTTPBasicStreamBuf = Lucid::BasicBufferedStreamBuf<char, std::char_traits<char>, HTTPBufferAllocator>;


} } // namespace Lucid::Net


#endif // Net_HTTPBasicStreamBuf_INCLUDED
