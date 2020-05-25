//
// RotateStrategy.cpp
//
// Library: Foundation
// Package: Logging
// Module:  FileChannel
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "lucid/RotateStrategy.h"
#include "lucid/FileStream.h"
#include "lucid/DateTimeParser.h"
#include "lucid/DateTimeFormatter.h"
#include "lucid/DateTimeFormat.h"
#include "lucid/LineEndingConverter.h"


namespace Lucid {


//
// RotateStrategy
//


RotateStrategy::RotateStrategy()
{
}


RotateStrategy::~RotateStrategy()
{
}


//
// RotateByIntervalStrategy
//


const std::string RotateByIntervalStrategy::ROTATE_TEXT("# Log file created/rotated ");


RotateByIntervalStrategy::RotateByIntervalStrategy(const Timespan& span): 
	_span(span),
	_lastRotate(0)
{
	if (span.totalMicroseconds() <= 0) throw InvalidArgumentException("time span must be greater than zero");
}


RotateByIntervalStrategy::~RotateByIntervalStrategy()
{
}


bool RotateByIntervalStrategy::mustRotate(LogFile* pFile)
{
	if (_lastRotate == 0 || pFile->size() == 0)
	{
		if (pFile->size() != 0)
		{
			Lucid::FileInputStream istr(pFile->path());
			Lucid::InputLineEndingConverter converter(istr, Lucid::LineEnding::NEWLINE_LF);
			std::string tag;
			std::getline(converter, tag);
			if (tag.compare(0, ROTATE_TEXT.size(), ROTATE_TEXT) == 0)
			{
				std::string timestamp(tag, ROTATE_TEXT.size());
				int tzd;
				_lastRotate = DateTimeParser::parse(DateTimeFormat::RFC1036_FORMAT, timestamp, tzd).timestamp();
			}
			else _lastRotate = pFile->creationDate();
		}
		else
		{
			_lastRotate.update();
			std::string tag(ROTATE_TEXT);
			DateTimeFormatter::append(tag, _lastRotate, DateTimeFormat::RFC1036_FORMAT);
			pFile->write(tag);
		}
	}
	Timestamp now;
	return _span <= now - _lastRotate;
}


//
// RotateBySizeStrategy
//


RotateBySizeStrategy::RotateBySizeStrategy(UInt64 size): _size(size)
{
	if (size == 0) throw InvalidArgumentException("size must be greater than zero");
}


RotateBySizeStrategy::~RotateBySizeStrategy()
{
}


bool RotateBySizeStrategy::mustRotate(LogFile* pFile)
{
	return pFile->size() >= _size;
}


} // namespace Lucid
