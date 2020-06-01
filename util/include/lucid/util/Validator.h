//
// Validator.h
//
// Library: Util
// Package: Options
// Module:  Validator
//
// Definition of the Validator class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Util_Validator_INCLUDED
#define Util_Validator_INCLUDED


#include "lucid/Util/Util.h"
#include "lucid/RefCountedObject.h"


namespace Lucid {
namespace Util {


class Option;


class Util_API Validator: public Lucid::RefCountedObject
	/// Validator specifies the interface for option validators.
	///
	/// Option validators provide a simple way for the automatic
	/// validation of command line argument values.
{
public:
	virtual void validate(const Option& option, const std::string& value) = 0;
		/// Validates the value for the given option.
		/// Does nothing if the value is valid.
		///
		/// Throws an OptionException otherwise.

protected:
	Validator();
		/// Creates the Validator.

	virtual ~Validator();
		/// Destroys the Validator.
};


} } // namespace Lucid::Util


#endif // Util_Validator_INCLUDED
