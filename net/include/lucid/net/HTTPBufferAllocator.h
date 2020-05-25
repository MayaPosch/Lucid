//
// HTTPBufferAllocator.h
//
// Library: Net
// Package: HTTP
// Module:  HTTPBufferAllocator
//
// Definition of the HTTPBufferAllocator class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_HTTPBufferAllocator_INCLUDED
#define Net_HTTPBufferAllocator_INCLUDED


#include "lucid/Net/Net.h"
#include "lucid/MemoryPool.h"
#include <ios>


namespace Lucid {
namespace Net {


class Net_API HTTPBufferAllocator
	/// A BufferAllocator for HTTP streams.
{
public:
	static char* allocate(std::streamsize size);
	static void deallocate(char* ptr, std::streamsize size);

	enum
	{
		BUFFER_SIZE = 4096
	};

private:
	static Lucid::MemoryPool _pool;
};


} } // namespace Lucid::Net


#endif // Net_HTTPBufferAllocator_INCLUDED
