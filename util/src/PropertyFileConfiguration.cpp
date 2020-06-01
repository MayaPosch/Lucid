//
// PropertyFileConfiguration.cpp
//
// Library: Util
// Package: Configuration
// Module:  PropertyFileConfiguration
//
// Copyright (c) 2004-2009, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "lucid/Util/PropertyFileConfiguration.h"
#include "lucid/Exception.h"
#include "lucid/String.h"
#include "lucid/Path.h"
#include "lucid/FileStream.h"
#include "lucid/LineEndingConverter.h"
#include "lucid/Ascii.h"


using Lucid::trim;
using Lucid::Path;


namespace Lucid {
namespace Util {


PropertyFileConfiguration::PropertyFileConfiguration()
{
}


PropertyFileConfiguration::PropertyFileConfiguration(std::istream& istr)
{
	load(istr);
}

	
PropertyFileConfiguration::PropertyFileConfiguration(const std::string& path)
{
	load(path);
}


PropertyFileConfiguration::~PropertyFileConfiguration()
{
}

	
void PropertyFileConfiguration::load(std::istream& istr)
{
	clear();
	while (!istr.eof())
	{
		parseLine(istr);
	}
}

	
void PropertyFileConfiguration::load(const std::string& path)
{
	Lucid::FileInputStream istr(path);
	if (istr.good())
		load(istr);
	else
		throw Lucid::OpenFileException(path);
}


void PropertyFileConfiguration::save(std::ostream& ostr) const
{
	MapConfiguration::iterator it = begin();
	MapConfiguration::iterator ed = end();
	while (it != ed)
	{
		ostr << it->first << ": ";
		for (auto ch: it->second)
		{
			switch (ch)
			{
			case '\t':
				ostr << "\\t";
				break;
			case '\r':
				ostr << "\\r";
				break;
			case '\n':
				ostr << "\\n";
				break;
			case '\f':
				ostr << "\\f";
				break;
			case '\\':
				ostr << "\\\\";
				break;
			default:
				ostr << ch;
				break;
			}
		}
		ostr << "\n";
		++it;
	}
}


void PropertyFileConfiguration::save(const std::string& path) const
{
	Lucid::FileOutputStream ostr(path);
	if (ostr.good())
	{
		Lucid::OutputLineEndingConverter lec(ostr);
		save(lec);
		lec.flush();
		ostr.flush();
		if (!ostr.good()) throw Lucid::WriteFileException(path);
	}
	else throw Lucid::CreateFileException(path);
}


void PropertyFileConfiguration::parseLine(std::istream& istr)
{
	static const int eof = std::char_traits<char>::eof(); 

	int c = istr.get();
	while (c != eof && Lucid::Ascii::isSpace(c)) c = istr.get();
	if (c != eof)
	{
		if (c == '#' || c == '!')
		{
			while (c != eof && c != '\n' && c != '\r') c = istr.get();
		}
		else
		{
			std::string key;
			while (c != eof && c != '=' && c != ':' && c != '\r' && c != '\n') { key += (char) c; c = istr.get(); }
			std::string value;
			if (c == '=' || c == ':')
			{
				c = readChar(istr);
				while (c != eof && c) { value += (char) c; c = readChar(istr); }
			}
			setRaw(trim(key), trim(value));
		}
	}
}


int PropertyFileConfiguration::readChar(std::istream& istr)
{
	for (;;)
	{
		int c = istr.get();
		if (c == '\\')
		{
			c = istr.get();
			switch (c)
			{
			case 't':
				return '\t';
			case 'r':
				return '\r';
			case 'n':
				return '\n';
			case 'f':
				return '\f';
			case '\r':
				if (istr.peek() == '\n')
					istr.get();
				continue;
			case '\n':
				continue;
			default:
				return c;
			}
		}
		else if (c == '\n' || c == '\r')
			return 0;
		else
			return c;
	}
}


} } // namespace Lucid::Util
