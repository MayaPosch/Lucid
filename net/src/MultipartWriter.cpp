//
// MultipartWriter.cpp
//
// Library: Net
// Package: Messages
// Module:  MultipartWriter
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "lucid/Net/MultipartWriter.h"
#include "lucid/Net/MessageHeader.h"
#include "lucid/Random.h"
#include "lucid/NumberFormatter.h"


using Lucid::Random;
using Lucid::NumberFormatter;


namespace Lucid {
namespace Net {


MultipartWriter::MultipartWriter(std::ostream& ostr):
	_ostr(ostr),
	_boundary(createBoundary()),
	_firstPart(true)
{
}


MultipartWriter::MultipartWriter(std::ostream& ostr, const std::string& boundary):
	_ostr(ostr),
	_boundary(boundary),
	_firstPart(true)
{
	if (_boundary.empty())
		_boundary = createBoundary();
}


MultipartWriter::~MultipartWriter()
{
}


void MultipartWriter::nextPart(const MessageHeader& header)
{
	if (_firstPart)
		_firstPart = false;
	else
		_ostr << "\r\n";
	_ostr << "--" << _boundary << "\r\n";
	header.write(_ostr);
	_ostr << "\r\n";
}


void MultipartWriter::close()
{
	_ostr << "\r\n--" << _boundary << "--\r\n";
}


const std::string& MultipartWriter::boundary() const
{
	return _boundary;
}


std::string MultipartWriter::createBoundary()
{
	std::string boundary("MIME_boundary_");
	Random rnd;
	rnd.seed();
	NumberFormatter::appendHex(boundary, rnd.next(), 8);
	NumberFormatter::appendHex(boundary, rnd.next(), 8);
	return boundary;
}


} } // namespace Lucid::Net
