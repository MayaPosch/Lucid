//
// JSONException.cpp
//
// Library: JSON
// Package: JSON
// Module:  JSONException
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "lucid/JSON/JSONException.h"
#include <typeinfo>


namespace Lucid {
namespace JSON {


POCO_IMPLEMENT_EXCEPTION(JSONException, Exception, "JSON Exception")


} } // namespace Lucid::JSON
