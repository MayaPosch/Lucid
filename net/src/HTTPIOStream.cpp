//
// HTTPIOStream.cpp
//
// Library: Net
// Package: HTTP
// Module:  HTTPIOStream
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "lucid/Net/HTTPIOStream.h"
#include "lucid/Net/HTTPClientSession.h"


using Lucid::UnbufferedStreamBuf;


namespace Lucid {
namespace Net {


HTTPResponseStreamBuf::HTTPResponseStreamBuf(std::istream& istr):
	_istr(istr)
{
	// make sure exceptions from underlying string propagate
	_istr.exceptions(std::ios::badbit);
}


HTTPResponseStreamBuf::~HTTPResponseStreamBuf()
{
}


HTTPResponseIOS::HTTPResponseIOS(std::istream& istr):
	_buf(istr)
{
	poco_ios_init(&_buf);
}


HTTPResponseIOS::~HTTPResponseIOS()
{
}


HTTPResponseStream::HTTPResponseStream(std::istream& istr, HTTPClientSession* pSession):
	HTTPResponseIOS(istr),
	std::istream(&_buf),
	_pSession(pSession)
{
}


HTTPResponseStream::~HTTPResponseStream()
{
	delete _pSession;
}


} } // namespace Lucid::Net
