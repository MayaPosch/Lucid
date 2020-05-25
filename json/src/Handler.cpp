//
// Handler.cpp
//
// Library: JSON
// Package: JSON
// Module:  Handler
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "lucid/JSON/Handler.h"
#include "lucid/JSON/Object.h"


namespace Lucid {
namespace JSON {


Handler::Handler()
{
}


Handler::~Handler()
{
}


dynamic::Var Handler::asVar() const
{
	return dynamic::Var();
}


Lucid::dynamicStruct Handler::asStruct() const
{
	return Lucid::dynamicStruct();
}


} } // namespace Lucid::JSON
