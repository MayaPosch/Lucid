//
// XMLException.cpp
//
// Library: XML
// Package: XML
// Module:  XMLException
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "lucid/XML/XMLException.h"
#include <typeinfo>


using Lucid::RuntimeException;


namespace Lucid {
namespace XML {


POCO_IMPLEMENT_EXCEPTION(XMLException, RuntimeException, "XML Exception")


} } // namespace Lucid::XML
