//
// Parser.cpp
//
// Library: JSON
// Package: JSON
// Module:  Parser
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "lucid/JSON/Parser.h"
#include "lucid/JSON/JSONException.h"
#include "lucid/Ascii.h"
#include "lucid/Token.h"
#include "lucid/UTF8Encoding.h"
#include "lucid/String.h"
#undef min
#undef max
#include <limits>
#include <clocale>
#include <istream>


namespace Lucid {
namespace JSON {


Parser::Parser(const Handler::Ptr& pHandler, std::size_t bufSize):
	ParserImpl(pHandler, bufSize)
{
}


Parser::~Parser()
{
}


void Parser::setHandler(const Handler::Ptr& pHandler)
{
	setHandlerImpl(pHandler);
}


} } // namespace Lucid::JSON
