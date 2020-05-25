//
// NTPPacket.h
//
// Library: Net
// Package: NTP
// Module:  NTPPacket
//
// Definition of the NTPPacket class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_NTPPacket_INCLUDED
#define Net_NTPPacket_INCLUDED


#include "lucid/Foundation.h"
#include "lucid/Net/Net.h"
#include "lucid/Timestamp.h"

namespace Lucid {
namespace Net {


class Net_API NTPPacket
	/// This class is the NTP packet abstraction. 
{
public:
	NTPPacket();
		/// Creates an NTPPacket.

	NTPPacket(Lucid::UInt8 *packet);
		/// Creates an NTPPacket.
		///
		/// Assumed to have at least 48 bytes.

	~NTPPacket();
		/// Destroys the NTPPacket.

	void packet(Lucid::UInt8 *packet) const;
		/// Returns the NTP packet.
		///
		/// Assumed to have at least 48 bytes.

	void setPacket(Lucid::UInt8 *packet);
		/// Returns the NTP packet.
		///
		/// Assumed to have exactly 48 bytes.

	Lucid::Int8 leapIndicator() const;
		/// Returns the leap indicator.

	Lucid::Int8 version() const;
		/// Returns the version.

	Lucid::Int8 mode() const;
		/// Returns the mode.

	Lucid::Int8 stratum() const;
		/// Returns the stratum.

	Lucid::Int8 pool() const;
		/// Returns the pool.

	Lucid::Int8 precision() const;
		/// Returns the precision

	Lucid::Int32 rootDelay() const;
		/// Returns the root delay

	Lucid::Int32 rootDispersion() const;
		/// Returns the root dispersion

	Lucid::Int32 referenceId() const;
		/// Returns the reference id

	Lucid::Int64 referenceTimestamp() const;
		/// Returns the reference timestamp

	Lucid::Int64 originateTimestamp() const;
		/// Returns the originate timestamp

	Lucid::Int64 receiveTimestamp() const;
		/// Returns the receive timestamp

	Lucid::Int64 transmitTimestamp() const;
		/// Returns the transmit timestamp

	Lucid::Timestamp referenceTime() const;
		/// Returns the reference time

	Lucid::Timestamp originateTime() const;
		/// Returns the originate time

	Lucid::Timestamp receiveTime() const;
		/// Returns the receive time

	Lucid::Timestamp transmitTime() const;
		/// Returns the transmit time
private:
	Lucid::Timestamp convertTime(Lucid::Int64 tm) const;

	Lucid::Int8 _leapIndicator;
	Lucid::Int8 _version;
	Lucid::Int8 _mode;
	Lucid::Int8 _stratum;
	Lucid::Int8 _pool;
	Lucid::Int8 _precision;
	Lucid::Int32 _rootDelay;
	Lucid::Int32 _rootDispersion;
	Lucid::Int32 _referenceId;
	Lucid::Int64 _referenceTimestamp;
	Lucid::Int64 _originateTimestamp;
	Lucid::Int64 _receiveTimestamp;
	Lucid::Int64 _transmitTimestamp;
};


//
// inlines
//
inline Lucid::Int8 NTPPacket::leapIndicator() const
{
	return _leapIndicator;
}


inline Lucid::Int8 NTPPacket::version() const
{
	return _version;
}


inline Lucid::Int8 NTPPacket::mode() const
{
	return _mode;
}


inline Lucid::Int8 NTPPacket::stratum() const
{
	return _stratum;
}


inline Lucid::Int8 NTPPacket::pool() const
{
	return _pool;
}


inline Lucid::Int8 NTPPacket::precision() const
{
	return _precision;
}


inline Lucid::Int32 NTPPacket::rootDelay() const
{
	return _rootDelay;
}


inline Lucid::Int32 NTPPacket::rootDispersion() const
{
	return _rootDispersion;
}


inline Lucid::Int32 NTPPacket::referenceId() const
{
	return _referenceId;
}


inline Lucid::Int64 NTPPacket::referenceTimestamp() const
{
	return _referenceTimestamp;
}


inline Lucid::Int64 NTPPacket::originateTimestamp() const
{
	return _originateTimestamp;
}


inline Lucid::Int64 NTPPacket::receiveTimestamp() const
{
	return _receiveTimestamp;
}


inline Lucid::Int64 NTPPacket::transmitTimestamp() const
{
	return _transmitTimestamp;
}


} } // namespace Lucid::Net


#endif // Net_NTPPacket_INCLUDED
