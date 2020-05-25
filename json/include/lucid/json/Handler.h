//
// Handler.h
//
// Library: JSON
// Package: JSON
// Module:  Handler
//
// Definition of the Handler class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef JSON_Handler_INCLUDED
#define JSON_Handler_INCLUDED


#include "lucid/JSON/JSON.h"
#include "lucid/SharedPtr.h"
#include "lucid/dynamic/Var.h"
#include "lucid/dynamic/Struct.h"


namespace Lucid {
namespace JSON {


class JSON_API Handler
	/// Interface for handling parsing events generated by the JSON Parser.
	///
	/// An application can implement a subclass of Handler to implement
	/// callback-based parsing of a JSON document, similar to how a SAX
	/// parser would handle XML.
{
public:
	using Ptr = SharedPtr<Handler>;

	Handler();
		/// Creates an empty Handler.
	
	virtual ~Handler();
		/// Destroys the Handler.

	virtual void reset() = 0;
		/// Resets the handler state.

	virtual void startObject() = 0;
		/// The parser has read a {, meaning a new object will be read.

	virtual void endObject() = 0;
		/// The parser has read a }, meaning the object is read.

	virtual void startArray() = 0;
		/// The parser has read a [, meaning a new array will be read.

	virtual void endArray() = 0;
		/// The parser has read a ], meaning the array is read.

	virtual void key(const std::string& k) = 0;
		/// A key of an object is read.

	virtual void null() = 0;
		/// A null value is read.

	virtual void value(int v) = 0;
		/// An integer value is read.

	virtual void value(unsigned v) = 0;
		/// An unsigned value is read. This will only be triggered if the
		/// value cannot fit into a signed int.

#if defined(POCO_HAVE_INT64)
	virtual void value(Int64 v) = 0;
		/// A 64-bit integer value is read.

	virtual void value(UInt64 v) = 0;
		/// An unsigned 64-bit integer value is read. This will only be
		/// triggered if the value cannot fit into a signed 64-bit integer.
#endif

	virtual void value(const std::string& value) = 0;
		/// A string value is read.

	virtual void value(double d) = 0;
		/// A double value is read.

	virtual void value(bool b) = 0;
		/// A boolean value is read.

	virtual Lucid::dynamic::Var asVar() const;
		/// Returns the result of the parser (an object, array or string),
		/// empty Var if there is no result.

	virtual Lucid::dynamicStruct asStruct() const;
		/// Returns the result of the parser (an object, array or string),
		/// empty Var if there is no result.
};


} } // namespace Lucid::JSON


#endif // JSON_Handler_INCLUDED