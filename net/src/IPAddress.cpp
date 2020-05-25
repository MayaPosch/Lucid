//
// IPAddress.cpp
//
// Library: Net
// Package: NetCore
// Module:  IPAddress
//
// Copyright (c) 2005-2011, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "lucid/Net/IPAddress.h"
#include "lucid/Net/NetException.h"
#include "lucid/RefCountedObject.h"
#include "lucid/NumberFormatter.h"
#include "lucid/BinaryReader.h"
#include "lucid/BinaryWriter.h"
#include "lucid/String.h"
#include "lucid/Types.h"


using Lucid::RefCountedObject;
using Lucid::NumberFormatter;
using Lucid::BinaryReader;
using Lucid::BinaryWriter;
using Lucid::toLower;
using Lucid::trim;
using Lucid::UInt8;
using Lucid::UInt16;
using Lucid::UInt32;
using Lucid::Net::Impl::IPAddressImpl;
using Lucid::Net::Impl::IPv4AddressImpl;
#if defined(POCO_HAVE_IPv6)
using Lucid::Net::Impl::IPv6AddressImpl;
#endif


namespace Lucid {
namespace Net {


#if !defined(_MSC_VER) || defined(__STDC__)
// Go home MSVC, you're drunk...
// See http://stackoverflow.com/questions/5899857/multiple-definition-error-for-static-const-class-members
const IPAddress::Family IPAddress::IPv4;
#if defined(POCO_HAVE_IPv6)
const IPAddress::Family IPAddress::IPv6;
#endif
#endif


IPAddress::IPAddress()
{
	newIPv4();
}


IPAddress::IPAddress(const IPAddress& addr)
{
	if (addr.family() == IPv4)
		newIPv4(addr.addr());
#if defined(POCO_HAVE_IPv6)
	else
		newIPv6(addr.addr(), addr.scope());
#endif
}


IPAddress::IPAddress(Family family)
{
	if (family == IPv4)
		newIPv4();
#if defined(POCO_HAVE_IPv6)
	else if (family == IPv6)
		newIPv6();
#endif
	else throw Lucid::InvalidArgumentException("Invalid or unsupported address family passed to IPAddress()");
}


IPAddress::IPAddress(const std::string& addr)
{
	IPv4AddressImpl empty4 = IPv4AddressImpl();
	if (addr.empty() || trim(addr) == "0.0.0.0")
	{
		newIPv4(empty4.addr());
		return;
	}

	IPv4AddressImpl addr4(IPv4AddressImpl::parse(addr));
	if (addr4 != empty4)
	{
		newIPv4(addr4.addr());
		return;
	}

#if defined(POCO_HAVE_IPv6)
	IPv6AddressImpl empty6 = IPv6AddressImpl();
	if (addr.empty() || trim(addr) == "::")
	{
		newIPv6(empty6.addr());
		return;
	}

	IPv6AddressImpl addr6(IPv6AddressImpl::parse(addr));
	if (addr6 != IPv6AddressImpl())
	{
		newIPv6(addr6.addr(), addr6.scope());
		return;
	}
#endif

	throw InvalidAddressException(addr);
}


IPAddress::IPAddress(const std::string& addr, Family family)
{
	if (family == IPv4)
	{
		IPv4AddressImpl addr4(IPv4AddressImpl::parse(addr));
		newIPv4(addr4.addr());
		return;
	}
#if defined(POCO_HAVE_IPv6)
	else if (family == IPv6)
	{
		IPv6AddressImpl addr6(IPv6AddressImpl::parse(addr));
		newIPv6(addr6.addr(), addr6.scope());
		return;
	}
#endif
	else throw Lucid::InvalidArgumentException("Invalid or unsupported address family passed to IPAddress()");
}


IPAddress::IPAddress(const void* addr, poco_socklen_t length)
	: _pImpl(0)
{
	if (length == sizeof(struct in_addr))
		newIPv4(addr);
#if defined(POCO_HAVE_IPv6)
	else if (length == sizeof(struct in6_addr))
		newIPv6(addr);
#endif
	else throw Lucid::InvalidArgumentException("Invalid address length passed to IPAddress()");
}


IPAddress::IPAddress(const void* addr, poco_socklen_t length, Lucid::UInt32 scope)
{
	if (length == sizeof(struct in_addr))
		newIPv4(addr);
#if defined(POCO_HAVE_IPv6)
	else if (length == sizeof(struct in6_addr))
		newIPv6(addr, scope);
#endif
	else throw Lucid::InvalidArgumentException("Invalid address length passed to IPAddress()");
}


IPAddress::IPAddress(unsigned prefix, Family family)
{
	if (family == IPv4)
	{
		if (prefix <= 32)
			newIPv4(prefix);
		else
			throw Lucid::InvalidArgumentException("Invalid prefix length passed to IPAddress()");
	}
#if defined(POCO_HAVE_IPv6)
	else if (family == IPv6)
	{
		if (prefix <= 128)
			newIPv6(prefix);
		else
			throw Lucid::InvalidArgumentException("Invalid prefix length passed to IPAddress()");
	}
#endif
	else throw Lucid::InvalidArgumentException("Invalid or unsupported address family passed to IPAddress()");
}


#if defined(_WIN32)
IPAddress::IPAddress(const SOCKET_ADDRESS& socket_address)
	: _pImpl(0)
{
	ADDRESS_FAMILY family = socket_address.lpSockaddr->sa_family;
	if (family == AF_INET)
		newIPv4(&reinterpret_cast<const struct sockaddr_in*>(socket_address.lpSockaddr)->sin_addr);
#if defined(POCO_HAVE_IPv6)
	else if (family == AF_INET6)
		newIPv6(&reinterpret_cast<const struct sockaddr_in6*>(socket_address.lpSockaddr)->sin6_addr,
			reinterpret_cast<const struct sockaddr_in6*>(socket_address.lpSockaddr)->sin6_scope_id);
#endif
	else throw Lucid::InvalidArgumentException("Invalid or unsupported address family passed to IPAddress()");
}
#endif


IPAddress::IPAddress(const struct sockaddr& sockaddr)
{
	unsigned short family = sockaddr.sa_family;
	if (family == AF_INET)
		newIPv4(&reinterpret_cast<const struct sockaddr_in*>(&sockaddr)->sin_addr);
#if defined(POCO_HAVE_IPv6)
	else if (family == AF_INET6)
		newIPv6(&reinterpret_cast<const struct sockaddr_in6*>(&sockaddr)->sin6_addr,
			reinterpret_cast<const struct sockaddr_in6*>(&sockaddr)->sin6_scope_id);
#endif
	else throw Lucid::InvalidArgumentException("Invalid or unsupported address family passed to IPAddress()");
}


IPAddress::~IPAddress()
{
}


IPAddress& IPAddress::operator = (const IPAddress& addr)
{
	if (&addr != this)
	{
		if (addr.family() == IPAddress::IPv4)
			newIPv4(addr.addr());
#if defined(POCO_HAVE_IPv6)
		else if (addr.family() == IPAddress::IPv6)
			newIPv6(addr.addr(), addr.scope());
#endif
		else 
			throw Lucid::InvalidArgumentException("Invalid or unsupported address family");
	}
	return *this;
}


IPAddress::Family IPAddress::family() const
{
	return pImpl()->family();
}


Lucid::UInt32 IPAddress::scope() const
{
	return pImpl()->scope();
}

	
std::string IPAddress::toString() const
{
	return pImpl()->toString();
}


bool IPAddress::isWildcard() const
{
	return pImpl()->isWildcard();
}


bool IPAddress::isBroadcast() const
{
	return pImpl()->isBroadcast();
}


bool IPAddress::isLoopback() const
{
	return pImpl()->isLoopback();
}


bool IPAddress::isMulticast() const
{
	return pImpl()->isMulticast();
}

	
bool IPAddress::isUnicast() const
{
	return !isWildcard() && !isBroadcast() && !isMulticast();
}

	
bool IPAddress::isLinkLocal() const
{
	return pImpl()->isLinkLocal();
}


bool IPAddress::isSiteLocal() const
{
	return pImpl()->isSiteLocal();
}


bool IPAddress::isIPv4Compatible() const
{
	return pImpl()->isIPv4Compatible();
}


bool IPAddress::isIPv4Mapped() const
{
	return pImpl()->isIPv4Mapped();
}


bool IPAddress::isWellKnownMC() const
{
	return pImpl()->isWellKnownMC();
}


bool IPAddress::isNodeLocalMC() const
{
	return pImpl()->isNodeLocalMC();
}


bool IPAddress::isLinkLocalMC() const
{
	return pImpl()->isLinkLocalMC();
}


bool IPAddress::isSiteLocalMC() const
{
	return pImpl()->isSiteLocalMC();
}


bool IPAddress::isOrgLocalMC() const
{
	return pImpl()->isOrgLocalMC();
}


bool IPAddress::isGlobalMC() const
{
	return pImpl()->isGlobalMC();
}


bool IPAddress::operator == (const IPAddress& a) const
{
	poco_socklen_t l1 = length();
	poco_socklen_t l2 = a.length();
	if (l1 == l2)
    {
#if defined(POCO_HAVE_IPv6)
        if ( scope() != a.scope() )
            return false;
#endif
		return std::memcmp(addr(), a.addr(), l1) == 0;
    }
	else return false;
}


bool IPAddress::operator != (const IPAddress& a) const
{
    return !(*this == a);
}


bool IPAddress::operator < (const IPAddress& a) const
{
	poco_socklen_t l1 = length();
	poco_socklen_t l2 = a.length();
	if (l1 == l2)
    {
#if defined(POCO_HAVE_IPv6)
        if ( scope() != a.scope() )
            return scope() < a.scope();
#endif
		return std::memcmp(addr(), a.addr(), l1) < 0;
    }
	else return l1 < l2;
}


bool IPAddress::operator <= (const IPAddress& a) const
{
    return !(a < *this);
}


bool IPAddress::operator > (const IPAddress& a) const
{
    return a < *this;
}


bool IPAddress::operator >= (const IPAddress& a) const
{
    return !(*this < a);
}


IPAddress IPAddress::operator & (const IPAddress& other) const
{
	if (family() == other.family())
	{
		if (family() == IPv4)
		{
			IPv4AddressImpl t(pImpl()->addr());
			IPv4AddressImpl o(other.pImpl()->addr());
			return IPAddress((t & o).addr(), sizeof(struct in_addr));
		}
#if defined(POCO_HAVE_IPv6)
		else if (family() == IPv6)
		{
			const IPv6AddressImpl t(pImpl()->addr(), pImpl()->scope());
			const IPv6AddressImpl o(other.pImpl()->addr(), other.pImpl()->scope());
			const IPv6AddressImpl r = t & o;
			return IPAddress(r.addr(), sizeof(struct in6_addr), r.scope());
		}
#endif
		else throw Lucid::InvalidArgumentException("Invalid or unsupported address family passed to IPAddress()");
	}
	else throw Lucid::InvalidArgumentException("Invalid or unsupported address family passed to IPAddress()");
}


IPAddress IPAddress::operator | (const IPAddress& other) const
{
	if (family() == other.family())
	{
		if (family() == IPv4)
		{
			IPv4AddressImpl t(pImpl()->addr());
			IPv4AddressImpl o(other.pImpl()->addr());
			return IPAddress((t | o).addr(), sizeof(struct in_addr));
		}
#if defined(POCO_HAVE_IPv6)
		else if (family() == IPv6)
		{
			const IPv6AddressImpl t(pImpl()->addr(), pImpl()->scope());
			const IPv6AddressImpl o(other.pImpl()->addr(), other.pImpl()->scope());
			const IPv6AddressImpl r = t | o;
			return IPAddress(r.addr(), sizeof(struct in6_addr), r.scope());
		}
#endif
		else throw Lucid::InvalidArgumentException("Invalid or unsupported address family passed to IPAddress()");
	}
	else throw Lucid::InvalidArgumentException("Invalid or unsupported address family passed to IPAddress()");
}


IPAddress IPAddress::operator ^ (const IPAddress& other) const
{
	if (family() == other.family())
	{
		if (family() == IPv4)
		{
			IPv4AddressImpl t(pImpl()->addr());
			IPv4AddressImpl o(other.pImpl()->addr());
			return IPAddress((t ^ o).addr(), sizeof(struct in_addr));
		}
#if defined(POCO_HAVE_IPv6)
		else if (family() == IPv6)
		{
			const IPv6AddressImpl t(pImpl()->addr(), pImpl()->scope());
			const IPv6AddressImpl o(other.pImpl()->addr(), other.pImpl()->scope());
			const IPv6AddressImpl r = t ^ o;
			return IPAddress(r.addr(), sizeof(struct in6_addr), r.scope());
		}
#endif
		else throw Lucid::InvalidArgumentException("Invalid or unsupported address family passed to IPAddress()");
	}
	else throw Lucid::InvalidArgumentException("Invalid or unsupported address family passed to IPAddress()");
}


IPAddress IPAddress::operator ~ () const
{
	if (family() == IPv4)
	{
		IPv4AddressImpl self(this->pImpl()->addr());
		return IPAddress((~self).addr(), sizeof(struct in_addr));
	}
#if defined(POCO_HAVE_IPv6)
	else if (family() == IPv6)
	{
		const IPv6AddressImpl self(pImpl()->addr(), pImpl()->scope());
		const IPv6AddressImpl r = ~self;
		return IPAddress(r.addr(), sizeof(struct in6_addr), r.scope());
	}
#endif
	else throw Lucid::InvalidArgumentException("Invalid or unsupported address family passed to IPAddress()");
}


poco_socklen_t IPAddress::length() const
{
	return pImpl()->length();
}

	
const void* IPAddress::addr() const
{
	return pImpl()->addr();
}


int IPAddress::af() const
{
	return pImpl()->af();
}


unsigned IPAddress::prefixLength() const
{
	return pImpl()->prefixLength();
}


IPAddress IPAddress::parse(const std::string& addr)
{
	return IPAddress(addr);
}


bool IPAddress::tryParse(const std::string& addr, IPAddress& result)
{
	IPv4AddressImpl impl4(IPv4AddressImpl::parse(addr));
	if (impl4 != IPv4AddressImpl() || trim(addr) == "0.0.0.0")
	{
		result.newIPv4(impl4.addr());
		return true;
	}
#if defined(POCO_HAVE_IPv6)
	IPv6AddressImpl impl6(IPv6AddressImpl::parse(addr));
	if (impl6 != IPv6AddressImpl())
	{
		result.newIPv6(impl6.addr(), impl6.scope());
		return true;
	}
#endif
	return false;
}


void IPAddress::mask(const IPAddress& mask)
{
	IPAddress null;
	pImpl()->mask(mask.pImpl(), null.pImpl());
}


void IPAddress::mask(const IPAddress& mask, const IPAddress& set)
{
	pImpl()->mask(mask.pImpl(), set.pImpl());
}


IPAddress IPAddress::wildcard(Family family)
{
	return IPAddress(family);
}


IPAddress IPAddress::broadcast()
{
	struct in_addr ia;
	ia.s_addr = INADDR_NONE;
	return IPAddress(&ia, sizeof(ia));
}


} } // namespace Lucid::Net


Lucid::BinaryWriter& operator << (Lucid::BinaryWriter& writer, const Lucid::Net::IPAddress& value)
{
	writer << static_cast<Lucid::UInt8>(value.length());
	writer.writeRaw(reinterpret_cast<const char*>(value.addr()), value.length());
	return writer;
}


Lucid::BinaryReader& operator >> (Lucid::BinaryReader& reader, Lucid::Net::IPAddress& value)
{
	char buf[Lucid::Net::IPAddress::MAX_ADDRESS_LENGTH];
	Lucid::UInt8 length;
	reader >> length;
	reader.readRaw(buf, length);
	value = Lucid::Net::IPAddress(buf, length);
	return reader;
}


std::ostream& operator << (std::ostream& ostr, const Lucid::Net::IPAddress& addr)
{
	ostr << addr.toString();
	return ostr;
}
