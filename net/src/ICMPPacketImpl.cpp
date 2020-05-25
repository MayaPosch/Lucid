//
// ICMPPacketImpl.cpp
//
// Library: Net
// Package: ICMP
// Module:  ICMPPacketImpl
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "lucid/Net/ICMPPacketImpl.h"
#include "lucid/Net/NetException.h"
#include "lucid/Timestamp.h"
#include "lucid/Timespan.h"
#include "lucid/NumberFormatter.h"
#include <sstream>


using Lucid::InvalidArgumentException;
using Lucid::Timestamp;
using Lucid::Timespan;
using Lucid::NumberFormatter;
using Lucid::UInt8;
using Lucid::UInt16;
using Lucid::Int32;


namespace Lucid {
namespace Net {


const UInt16 ICMPPacketImpl::MAX_PACKET_SIZE  = 65535;
const UInt16 ICMPPacketImpl::MAX_PAYLOAD_SIZE = 65507;
const UInt16 ICMPPacketImpl::MAX_SEQ_VALUE    = 65535;


ICMPPacketImpl::ICMPPacketImpl(int dataSize):
	_seq(0),
	_pPacket(new UInt8[MAX_PACKET_SIZE]),
	_dataSize(dataSize)
{
	if (_dataSize > MAX_PACKET_SIZE)
		throw InvalidArgumentException("Packet size must be <= " + NumberFormatter::format(MAX_PACKET_SIZE));
}


ICMPPacketImpl::~ICMPPacketImpl()
{
	delete [] _pPacket;
}


void ICMPPacketImpl::setDataSize(int dataSize)
{
	_dataSize = dataSize;
	initPacket();
}


int ICMPPacketImpl::getDataSize() const
{
	return _dataSize;
}


const Lucid::UInt8* ICMPPacketImpl::packet(bool init)
{
	if (init) initPacket();
	return _pPacket;
}


unsigned short ICMPPacketImpl::checksum(UInt16 *addr, Int32 len)
{
	Int32 nleft = len;
	UInt16* w   = addr;
	UInt16 answer;
	Int32 sum = 0;

	while (nleft > 1)  
	{
		sum   += *w++;
		nleft -= sizeof(UInt16);
	}

	if (nleft == 1) 
	{
		UInt16 u = 0;
		*(UInt8*) (&u) = *(UInt8*) w;
		sum += u;
	}

	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	answer = ~sum;
	return answer;
}


} } // namespace Lucid::Net
