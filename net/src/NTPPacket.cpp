//
// NTPPacket.cpp
//
// Library: Net
// Package: NTP
// Module:  NTPPacket
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "lucid/Net/NTPPacket.h"
#include "lucid/Net/NetException.h"
#include "lucid/Timestamp.h"
#include "lucid/ByteOrder.h"


namespace Lucid {
namespace Net {


#if !defined(POCO_COMPILER_SUN)
#pragma pack(push, 1)
#else
#pragma pack(1)
#endif
struct NTPPacketData 
{
	Lucid::Int8 mode:3;
	Lucid::Int8 vn:3;
	Lucid::Int8 li:2;
	Lucid::Int8 stratum;
	Lucid::Int8 pool;
	Lucid::Int8 prec;
	Lucid::Int32 rootdelay;
	Lucid::Int32 rootdisp;
	Lucid::Int32 refid;
	Lucid::Int64 rts;
	Lucid::Int64 ots;
	Lucid::Int64 vts;
	Lucid::Int64 tts;
};
#if !defined(POCO_COMPILER_SUN)
#pragma pack(pop)
#else
#pragma pack()
#endif


NTPPacket::NTPPacket() :
	// the next 3 fields must be in reverse order from spec
	_leapIndicator(3),
	_version(4),
	_mode(3),

	_stratum(0),
	_pool(6),
	_precision(-18),
	_rootDelay(0),
	_rootDispersion(0),
	_referenceId(0),
	_referenceTimestamp(0),
	_receiveTimestamp(0),
	_transmitTimestamp(0)
{
	Lucid::Timestamp ts;
	_originateTimestamp = ts.utcTime() - 2874597888;
}


NTPPacket::NTPPacket(Lucid::UInt8 *packet)
{
	setPacket(packet);
}


NTPPacket::~NTPPacket()
{
}


void NTPPacket::packet(Lucid::UInt8 *packet) const
{
	NTPPacketData *p = (NTPPacketData*)packet;

	p->li = _leapIndicator;
	p->vn = _version;
	p->mode = _mode;
	p->stratum = _stratum;
	p->pool = _pool;
	p->prec = _precision;
	p->rootdelay = Lucid::ByteOrder::toNetwork(_rootDelay);
	p->rootdisp = Lucid::ByteOrder::toNetwork(_rootDispersion);
	p->refid = Lucid::ByteOrder::toNetwork(_referenceId);
	p->rts = Lucid::ByteOrder::toNetwork(_referenceTimestamp);
	p->ots = Lucid::ByteOrder::toNetwork(_originateTimestamp);
	p->vts = Lucid::ByteOrder::toNetwork(_receiveTimestamp);
	p->tts = Lucid::ByteOrder::toNetwork(_transmitTimestamp);
}


void NTPPacket::setPacket(Lucid::UInt8 *packet)
{
	NTPPacketData *p = (NTPPacketData*)packet;

	_leapIndicator = p->li;
	_version = p->vn;
	_mode = p->mode;
	_stratum = p->stratum;
	_pool = p->pool;
	_precision = p->prec;
	_rootDelay = Lucid::ByteOrder::fromNetwork(p->rootdelay);
	_rootDispersion = Lucid::ByteOrder::fromNetwork(p->rootdisp);
	_referenceId = Lucid::ByteOrder::fromNetwork(p->refid);
	_referenceTimestamp = Lucid::ByteOrder::fromNetwork(p->rts);
	_originateTimestamp = Lucid::ByteOrder::fromNetwork(p->ots);
	_receiveTimestamp = Lucid::ByteOrder::fromNetwork(p->vts);
	_transmitTimestamp = Lucid::ByteOrder::fromNetwork(p->tts);
}


Lucid::Timestamp NTPPacket::referenceTime() const
{
	return convertTime(_referenceTimestamp);
}


Lucid::Timestamp NTPPacket::originateTime() const
{
	return convertTime(_originateTimestamp);
}


Lucid::Timestamp NTPPacket::receiveTime() const
{
	return convertTime(_receiveTimestamp);
}


Lucid::Timestamp NTPPacket::transmitTime() const
{
	return convertTime(_transmitTimestamp);
}


Lucid::Timestamp NTPPacket::convertTime(Lucid::Int64 tm) const
{
	const unsigned long seventyYears = 2208988800UL;
	Lucid::UInt32 secsSince1900 = UInt32(Lucid::ByteOrder::toLittleEndian(tm) >> 32);
	unsigned long epoch = secsSince1900 - seventyYears;
	return Lucid::Timestamp::fromEpochTime(epoch);
}


} } // namespace Lucid::Net
