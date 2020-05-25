//
// ExpirationDecorator.h
//
// Library: Foundation
// Package: Events
// Module:  ExpirationDecorator
//
// Implementation of the ExpirationDecorator template.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_ExpirationDecorator_INCLUDED
#define Foundation_ExpirationDecorator_INCLUDED


#include "lucid/Timestamp.h"
#include "lucid/Timespan.h"


namespace Lucid {


template <typename TArgs>
class ExpirationDecorator
	/// ExpirationDecorator adds an expiration method to values so that they can be used
	/// with the UniqueExpireCache.
{
public:
	ExpirationDecorator():
		_value(),
		_expiresAt()
	{
	}

	ExpirationDecorator(const TArgs& p, const Lucid::Timespan::TimeDiff& diffInMs):
			/// Creates an element that will expire in diff milliseconds
		_value(p),
		_expiresAt()
	{
		_expiresAt += (diffInMs*1000);
	}

	ExpirationDecorator(const TArgs& p, const Lucid::Timespan& timeSpan):
		/// Creates an element that will expire after the given timeSpan
		_value(p),
		_expiresAt()
	{
		_expiresAt += timeSpan.totalMicroseconds();
	}

	ExpirationDecorator(const TArgs& p, const Lucid::Timestamp& timeStamp):
		/// Creates an element that will expire at the given time point
		_value(p),
		_expiresAt(timeStamp)
	{
	}


	~ExpirationDecorator()
	{
	}
	
	const Lucid::Timestamp& getExpiration() const
	{
		return _expiresAt;
	}

	const TArgs& value() const
	{
		return _value;
	}

	TArgs& value()
	{
		return _value;
	}

private:
	TArgs     _value;
	Timestamp _expiresAt;
};


} // namespace Lucid


#endif // Foundation_ExpirationDecorator_INCLUDED
