//
// NullPartHandler.cpp
//
// Library: Net
// Package: Messages
// Module:  NullPartHandler
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "lucid/Net/NullPartHandler.h"
#include "lucid/Net/MessageHeader.h"
#include "lucid/NullStream.h"
#include "lucid/StreamCopier.h"


using Lucid::NullOutputStream;
using Lucid::StreamCopier;


namespace Lucid {
namespace Net {


NullPartHandler::NullPartHandler()
{
}


NullPartHandler::~NullPartHandler()
{
}


void NullPartHandler::handlePart(const MessageHeader& header, std::istream& stream)
{
	NullOutputStream ostr;
	StreamCopier::copyStream(stream, ostr);
}


} } // namespace Lucid::Net
