//
// HTTPServerResponseImpl.cpp
//
// Library: Net
// Package: HTTPServer
// Module:  HTTPServerResponseImpl
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "lucid/Net/HTTPServerResponseImpl.h"
#include "lucid/Net/HTTPServerRequestImpl.h"
#include "lucid/Net/HTTPServerSession.h"
#include "lucid/Net/HTTPHeaderStream.h"
#include "lucid/Net/HTTPStream.h"
#include "lucid/Net/HTTPFixedLengthStream.h"
#include "lucid/Net/HTTPChunkedStream.h"
#include "lucid/File.h"
#include "lucid/Timestamp.h"
#include "lucid/NumberFormatter.h"
#include "lucid/StreamCopier.h"
#include "lucid/CountingStream.h"
#include "lucid/Exception.h"
#include "lucid/FileStream.h"
#include "lucid/DateTimeFormatter.h"
#include "lucid/DateTimeFormat.h"


using Lucid::File;
using Lucid::Timestamp;
using Lucid::NumberFormatter;
using Lucid::StreamCopier;
using Lucid::OpenFileException;
using Lucid::DateTimeFormatter;
using Lucid::DateTimeFormat;


namespace Lucid {
namespace Net {


HTTPServerResponseImpl::HTTPServerResponseImpl(HTTPServerSession& session):
	_session(session),
	_pRequest(0),
	_pStream(0)
{
}


HTTPServerResponseImpl::~HTTPServerResponseImpl()
{
	delete _pStream;
}


void HTTPServerResponseImpl::sendContinue()
{
	HTTPHeaderOutputStream hs(_session);
	hs << getVersion() << " 100 Continue\r\n\r\n";
}


std::ostream& HTTPServerResponseImpl::send()
{
	poco_assert (!_pStream);

	if ((_pRequest && _pRequest->getMethod() == HTTPRequest::HTTP_HEAD) ||
		getStatus() < 200 ||
		getStatus() == HTTPResponse::HTTP_NO_CONTENT ||
		getStatus() == HTTPResponse::HTTP_NOT_MODIFIED)
	{
		Lucid::CountingOutputStream cs;
		write(cs);
		_pStream = new HTTPFixedLengthOutputStream(_session, cs.chars());
		write(*_pStream);
	}
	else if (getChunkedTransferEncoding())
	{
		HTTPHeaderOutputStream hs(_session);
		write(hs);
		_pStream = new HTTPChunkedOutputStream(_session);
	}
	else if (hasContentLength())
	{
		Lucid::CountingOutputStream cs;
		write(cs);
#if defined(POCO_HAVE_INT64)	
		_pStream = new HTTPFixedLengthOutputStream(_session, getContentLength64() + cs.chars());
#else
		_pStream = new HTTPFixedLengthOutputStream(_session, getContentLength() + cs.chars());
#endif
		write(*_pStream);
	}
	else
	{
		_pStream = new HTTPOutputStream(_session);
		setKeepAlive(false);
		write(*_pStream);
	}
	return *_pStream;
}


void HTTPServerResponseImpl::sendFile(const std::string& path, const std::string& mediaType)
{
	poco_assert (!_pStream);

	File f(path);
	Timestamp dateTime    = f.getLastModified();
	File::FileSize length = f.getSize();
	set("Last-Modified", DateTimeFormatter::format(dateTime, DateTimeFormat::HTTP_FORMAT));
#if defined(POCO_HAVE_INT64)	
	setContentLength64(length);
#else
	setContentLength(static_cast<int>(length));
#endif
	setContentType(mediaType);
	setChunkedTransferEncoding(false);

	Lucid::FileInputStream istr(path);
	if (istr.good())
	{
		_pStream = new HTTPHeaderOutputStream(_session);
		write(*_pStream);
		if (_pRequest && _pRequest->getMethod() != HTTPRequest::HTTP_HEAD)
		{
			StreamCopier::copyStream(istr, *_pStream);
		}
	}
	else throw OpenFileException(path);
}


void HTTPServerResponseImpl::sendBuffer(const void* pBuffer, std::size_t length)
{
	poco_assert (!_pStream);

	setContentLength(static_cast<int>(length));
	setChunkedTransferEncoding(false);
	
	_pStream = new HTTPHeaderOutputStream(_session);
	write(*_pStream);
	if (_pRequest && _pRequest->getMethod() != HTTPRequest::HTTP_HEAD)
	{
		_pStream->write(static_cast<const char*>(pBuffer), static_cast<std::streamsize>(length));
	}
}


void HTTPServerResponseImpl::redirect(const std::string& uri, HTTPStatus status)
{
	poco_assert (!_pStream);

	setContentLength(0);
	setChunkedTransferEncoding(false);

	setStatusAndReason(status);
	set("Location", uri);

	_pStream = new HTTPHeaderOutputStream(_session);
	write(*_pStream);
}


void HTTPServerResponseImpl::requireAuthentication(const std::string& realm)
{
	poco_assert (!_pStream);
	
	setStatusAndReason(HTTPResponse::HTTP_UNAUTHORIZED);
	std::string auth("Basic realm=\"");
	auth.append(realm);
	auth.append("\"");
	set("WWW-Authenticate", auth);
}


} } // namespace Lucid::Net
