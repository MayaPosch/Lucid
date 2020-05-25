//
// JSONException.h
//
// Library: JSON
// Package: JSON
// Module:  JSONException
//
// Definition of the JSONException class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef JSON_JSONException_INCLUDED
#define JSON_JSONException_INCLUDED


#include "lucid/JSON/JSON.h"
#include "lucid/Exception.h"


namespace Lucid {
namespace JSON {


POCO_DECLARE_EXCEPTION(JSON_API, JSONException, Lucid::Exception)


} } // namespace Lucid::JSON


#endif // JSON_JSONException_INCLUDED
