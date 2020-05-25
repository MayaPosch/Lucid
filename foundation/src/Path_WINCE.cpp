//
// Path_WIN32U.cpp
//
// Library: Foundation
// Package: Filesystem
// Module:  Path
//
// Copyright (c) 2006-2010, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "lucid/Path_WINCE.h"
#include "lucid/Environment_WINCE.h"
#include "lucid/UnicodeConverter.h"
#include "lucid/Buffer.h"
#include "lucid/Environment.h"
#include "lucid/Exception.h"
#include "lucid/UnWindows.h"


namespace Lucid {


std::string PathImpl::currentImpl()
{
	return("\\");
}

std::string PathImpl::homeImpl()
{
	return("\\");
}

std::string PathImpl::configHomeImpl()
{
  return homeImpl();
}

std::string PathImpl::dataHomeImpl()
{
  return homeImpl();
}

std::string PathImpl::cacheHomeImpl()
{
  return homeImpl();
}


std::string PathImpl::tempHomeImpl()
{
  return tempImpl();
}


std::string PathImpl::configImpl()
{
  return("\\");
}

std::string PathImpl::systemImpl()
{
	return("\\");
}


std::string PathImpl::nullImpl()
{
	return "NUL:";
}


std::string PathImpl::tempImpl()
{
	return "\\Temp\\";
}


std::string PathImpl::expandImpl(const std::string& path)
{
	std::string result;
	std::string::const_iterator it  = path.begin();
	std::string::const_iterator end = path.end();
	while (it != end)
	{
		if (*it == '%')
		{
			++it;
			if (it != end && *it == '%')
			{
				result += '%';
			}
			else
			{
				std::string var;
				while (it != end && *it != '%') var += *it++;
				if (it != end) ++it;
				result += Environment::get(var, "");
			}
		}
		else result += *it++;
	}
	return result;
}


void PathImpl::listRootsImpl(std::vector<std::string>& roots)
{
	roots.clear();
	roots.push_back("\\");

	WIN32_FIND_DATAW fd;
	HANDLE hFind = FindFirstFileW(L"\\*.*", &fd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
				(fd.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY))
			{
				std::wstring name(fd.cFileName);
				name += L"\\Vol:";
				HANDLE h = CreateFileW(name.c_str(), GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
				if (h != INVALID_HANDLE_VALUE)
				{
					// its a device volume
					CloseHandle(h);
					std::string name;
					UnicodeConverter::toUTF8(fd.cFileName, name);
					std::string root = "\\" + name;
					roots.push_back(root);
				}
			}
		} 
		while (FindNextFileW(hFind, &fd));
		FindClose(hFind);
	}
}


} // namespace Lucid
