//
// Stringifier.h
//
// Library: JSON
// Package: JSON
// Module:  Stringifier
//
// Definition of the Stringifier class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef JSON_JSONStringifier_INCLUDED
#define JSON_JSONStringifier_INCLUDED


#include "lucid/JSON/JSON.h"
#include "lucid/JSONString.h"
#include "lucid/dynamic/Var.h"
#include <ostream>


namespace Lucid {
namespace JSON {


class JSON_API Stringifier
	/// Helper class for creating a string from a JSON object or array.
{
public:
	static void condense(const dynamic::Var& any, std::ostream& out, int options = Lucid::JSON_WRAP_STRINGS);
		/// Writes a condensed string representation of the value to the output stream while preserving
		/// the insertion order.
		///
		/// If JSON_ESCAPE_UNICODE is in options, all unicode characters will be escaped, otherwise
		/// only the compulsory ones.
		///
		/// This is just a "shortcut" to stringify(any, out) with name indicating the function effect.

	static void stringify(const dynamic::Var& any, std::ostream& out,
			unsigned int indent = 0, int step = -1, int options = Lucid::JSON_WRAP_STRINGS);
		/// Writes a string representation of the value to the output stream.
		///
		/// When indent is 0, the string will be created as small as possible.
		/// Indentation is increased/decreased using number of spaces defined in step.
		/// The default value -1 for step indicates that step will be equal to the
		/// indent size.
		///
		/// If JSON_ESCAPE_UNICODE is in options, all unicode characters will be escaped, otherwise
		/// only the compulsory ones.

	static void formatString(const std::string& value, std::ostream& out, int options = Lucid::JSON_WRAP_STRINGS);
		/// Formats the JSON string and streams it into ostream.
		///
		/// If JSON_ESCAPE_UNICODE is in options, all unicode characters will be escaped, otherwise
		/// only the compulsory ones.
};


inline void Stringifier::condense(const dynamic::Var& any, std::ostream& out, int options)
{
	stringify(any, out, 0, -1, options);
}


} } // namespace Lucid::JSON


#endif // JSON_JSONStringifier_INCLUDED
