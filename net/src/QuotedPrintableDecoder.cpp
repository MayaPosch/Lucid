//
// QuotedPrintableDecoder.cpp
//
// Library: Net
// Package: Messages
// Module:  QuotedPrintableDecoder
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "lucid/Net/QuotedPrintableDecoder.h"
#include "lucid/NumberParser.h"
#include "lucid/Exception.h"
#include "lucid/Ascii.h"


using Lucid::UnbufferedStreamBuf;
using Lucid::NumberParser;
using Lucid::DataFormatException;


namespace Lucid {
namespace Net {


QuotedPrintableDecoderBuf::QuotedPrintableDecoderBuf(std::istream& istr): 
	_buf(*istr.rdbuf())
{
}


QuotedPrintableDecoderBuf::~QuotedPrintableDecoderBuf()
{
}


int QuotedPrintableDecoderBuf::readFromDevice()
{
	int ch = _buf.sbumpc();
	while (ch == '=')
	{
		ch = _buf.sbumpc();
		if (ch == '\r')
		{
			_buf.sbumpc(); // read \n
		}
		else if (Lucid::Ascii::isHexDigit(ch))
		{
			std::string hex = "0x";
			hex += (char) ch;
			ch = _buf.sbumpc();
			if (Lucid::Ascii::isHexDigit(ch))
			{
				hex += (char) ch;
				return NumberParser::parseHex(hex);
			}
			throw DataFormatException("Incomplete hex number in quoted-printable encoded stream");
		}
		else if (ch != '\n')
		{
			throw DataFormatException("Invalid occurrence of '=' in quoted-printable encoded stream");
		}
		ch = _buf.sbumpc();
	}
	return ch;
}


QuotedPrintableDecoderIOS::QuotedPrintableDecoderIOS(std::istream& istr): _buf(istr)
{
	poco_ios_init(&_buf);
}


QuotedPrintableDecoderIOS::~QuotedPrintableDecoderIOS()
{
}


QuotedPrintableDecoderBuf* QuotedPrintableDecoderIOS::rdbuf()
{
	return &_buf;
}


QuotedPrintableDecoder::QuotedPrintableDecoder(std::istream& istr): 
	QuotedPrintableDecoderIOS(istr), 
	std::istream(&_buf)
{
}


QuotedPrintableDecoder::~QuotedPrintableDecoder()
{
}


} } // namespace Lucid::Net
