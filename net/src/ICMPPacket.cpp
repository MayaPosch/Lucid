//
// ICMPPacket.cpp
//
// Library: Net
// Package: ICMP
// Module:  ICMPPacket
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "lucid/Net/ICMPPacket.h"
#include "lucid/Net/ICMPv4PacketImpl.h"
#include "lucid/Net/NetException.h"
#include "lucid/Timestamp.h"
#include "lucid/Timespan.h"
#include "lucid/NumberFormatter.h"
#include <sstream>


using Lucid::InvalidArgumentException;
using Lucid::NotImplementedException;
using Lucid::Timestamp;
using Lucid::Timespan;
using Lucid::NumberFormatter;
using Lucid::UInt8;
using Lucid::UInt16;
using Lucid::Int32;


namespace Lucid {
namespace Net {


ICMPPacket::ICMPPacket(IPAddress::Family family, int dataSize):_pImpl(0)
{
	if (family == IPAddress::IPv4)
		_pImpl = new ICMPv4PacketImpl(dataSize);
#if defined(POCO_HAVE_IPv6)
	else if (family == IPAddress::IPv6)
		throw NotImplementedException("ICMPv6 packets not implemented.");
#endif
	else throw InvalidArgumentException("Invalid or unsupported address family passed to ICMPPacket");
}


ICMPPacket::~ICMPPacket()
{
	delete _pImpl;
}


void ICMPPacket::setDataSize(int dataSize)
{
	_pImpl->setDataSize(dataSize);
}


int ICMPPacket::getDataSize() const
{
	return _pImpl->getDataSize();
}


int ICMPPacket::packetSize() const
{
	return _pImpl->packetSize();
}


int ICMPPacket::maxPacketSize() const
{
	return _pImpl->maxPacketSize();
}


const Lucid::UInt8* ICMPPacket::packet()
{
	return _pImpl->packet();
}


struct timeval ICMPPacket::time(Lucid::UInt8* buffer, int length) const
{
	return _pImpl->time(buffer, length);
}


bool ICMPPacket::validReplyID(Lucid::UInt8* buffer, int length) const
{
	return _pImpl->validReplyID(buffer, length);
}


std::string ICMPPacket::errorDescription(Lucid::UInt8* buffer, int length, int& type, int& code)
{
	return _pImpl->errorDescription(buffer, length, type, code);
}


std::string ICMPPacket::typeDescription(int typeId)
{
	return _pImpl->typeDescription(typeId);
}


} } // namespace Lucid::Net
