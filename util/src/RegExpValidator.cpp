//
// RegExpValidator.cpp
//
// Library: Util
// Package: Options
// Module:  RegExpValidator
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "lucid/Util/RegExpValidator.h"
#include "lucid/Util/Option.h"
#include "lucid/Util/OptionException.h"
#include "lucid/RegularExpression.h"
#include "lucid/Format.h"


using Lucid::format;


namespace Lucid {
namespace Util {


RegExpValidator::RegExpValidator(const std::string& regexp):
	_regexp(regexp)
{
}


RegExpValidator::~RegExpValidator()
{
}


void RegExpValidator::validate(const Option& option, const std::string& value)
{
	if (!RegularExpression::match(value, _regexp, RegularExpression::RE_ANCHORED | RegularExpression::RE_UTF8))
		throw InvalidArgumentException(format("argument for %s does not match regular expression %s", option.fullName(), _regexp));
}


} } // namespace Lucid::Util
