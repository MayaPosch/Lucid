//
// EntityResolverImpl.cpp
//
// Library: XML
// Package: SAX
// Module:  SAX
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "lucid/SAX/EntityResolverImpl.h"
#include "lucid/SAX/InputSource.h"
#include "lucid/XML/XMLString.h"
#include "lucid/URI.h"
#include "lucid/Path.h"
#include "lucid/Exception.h"


using Lucid::URIStreamOpener;
using Lucid::URI;
using Lucid::Path;
using Lucid::Exception;
using Lucid::IOException;
using Lucid::OpenFileException;


namespace Lucid {
namespace XML {


EntityResolverImpl::EntityResolverImpl():
	_opener(URIStreamOpener::defaultOpener())
{
}


EntityResolverImpl::EntityResolverImpl(const URIStreamOpener& opener):
	_opener(opener)
{
}


EntityResolverImpl::~EntityResolverImpl()
{
}


InputSource* EntityResolverImpl::resolveEntity(const XMLString* publicId, const XMLString& systemId)
{
	std::istream* pIstr = resolveSystemId(systemId);
	InputSource* pInputSource = new InputSource(systemId);
	if (publicId) pInputSource->setPublicId(*publicId);
	pInputSource->setByteStream(*pIstr);
	return pInputSource;
}

		
void EntityResolverImpl::releaseInputSource(InputSource* pSource)
{
	poco_check_ptr (pSource);

	delete pSource->getByteStream();
	delete pSource;
}


std::istream* EntityResolverImpl::resolveSystemId(const XMLString& systemId)
{
	std::string sid = fromXMLString(systemId);
	return _opener.open(sid);
}


} } // namespace Lucid::XML
