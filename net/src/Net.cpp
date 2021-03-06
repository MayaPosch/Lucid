//
// Net.cpp
//
// Library: Net
// Package: NetCore
// Module:  NetCore
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "lucid/Net/Net.h"


#include "lucid/Net/SocketDefs.h"
#include "lucid/Net/NetException.h"


namespace Lucid {
namespace Net {


void Net_API initializeNetwork()
{
#if defined(POCO_OS_FAMILY_WINDOWS)
	WORD    version = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(version, &data) != 0)
		throw NetException("Failed to initialize network subsystem");
#endif
}


void Net_API uninitializeNetwork()
{
#if defined(POCO_OS_FAMILY_WINDOWS)
	WSACleanup();
#endif
}


std::string htmlize(const std::string& str)
{
	std::string::const_iterator it(str.begin());
	std::string::const_iterator end(str.end());
	std::string html;
	for (; it != end; ++it)
	{
		switch (*it)
		{
		case '<': html += "&lt;"; break;
		case '>': html += "&gt;"; break;
		case '"': html += "&quot;"; break;
		case '&': html += "&amp;"; break;
		default:  html += *it; break;
		}
	}
	return html;
}


} } // namespace Lucid::Net


#if defined(POCO_OS_FAMILY_WINDOWS) && !defined(POCO_NO_AUTOMATIC_LIB_INIT)

	struct NetworkInitializer
		/// Network initializer for windows statically
		/// linked library.
	{
		NetworkInitializer()
			/// Calls Lucid::Net::initializeNetwork();
		{
			Lucid::Net::initializeNetwork();
		}

		~NetworkInitializer()
			/// Calls Lucid::Net::uninitializeNetwork();
		{
			try
			{
				Lucid::Net::uninitializeNetwork();
			}
			catch (...)
			{
				poco_unexpected();
			}
		}
	};

	const NetworkInitializer pocoNetworkInitializer;

#endif
