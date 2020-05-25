//
// RemoteSyslogListener.cpp
//
// Library: Net
// Package: Logging
// Module:  RemoteSyslogListener
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "lucid/Net/RemoteSyslogListener.h"
#include "lucid/Net/RemoteSyslogChannel.h"
#include "lucid/Net/DatagramSocket.h"
#include "lucid/Net/SocketAddress.h"
#include "lucid/Runnable.h"
#include "lucid/Notification.h"
#include "lucid/AutoPtr.h"
#include "lucid/NumberParser.h"
#include "lucid/NumberFormatter.h"
#include "lucid/DateTimeParser.h"
#include "lucid/Message.h"
#include "lucid/LoggingFactory.h"
#include "lucid/Buffer.h"
#include "lucid/Ascii.h"
#include <cstddef>


namespace Lucid {
namespace Net {


//
// MessageNotification
//


class MessageNotification: public Lucid::Notification
{
public:
	MessageNotification(const char* buffer, std::size_t length, const Lucid::Net::SocketAddress& sourceAddress):
		_message(buffer, length),
		_sourceAddress(sourceAddress)
	{
	}

	MessageNotification(const std::string& message, const Lucid::Net::SocketAddress& sourceAddress):
		_message(message),
		_sourceAddress(sourceAddress)
	{
	}
		
	~MessageNotification()
	{
	}
	
	const std::string& message() const
	{
		return _message;
	}
	
	const Lucid::Net::SocketAddress& sourceAddress() const
	{
		return _sourceAddress;
	}
	
private:
	std::string _message;
	Lucid::Net::SocketAddress _sourceAddress;
};


//
// RemoteUDPListener
//


class RemoteUDPListener: public Lucid::Runnable
{
public:
	enum
	{
		WAITTIME_MILLISEC = 1000,
		BUFFER_SIZE = 65536
	};
	
	RemoteUDPListener(Lucid::NotificationQueue& queue, Lucid::UInt16 port);
	~RemoteUDPListener();

	void run();
	void safeStop();

private:
	Lucid::NotificationQueue& _queue;
	DatagramSocket           _socket;
	bool                     _stopped;
};


RemoteUDPListener::RemoteUDPListener(Lucid::NotificationQueue& queue, Lucid::UInt16 port):
	_queue(queue),
	_socket(Lucid::Net::SocketAddress(Lucid::Net::IPAddress(), port)),
	_stopped(false)
{
}


RemoteUDPListener::~RemoteUDPListener()
{
}


void RemoteUDPListener::run()
{
	Lucid::Buffer<char> buffer(BUFFER_SIZE);
	Lucid::Timespan waitTime(WAITTIME_MILLISEC* 1000);
	while (!_stopped)
	{
		try
		{
			if (_socket.poll(waitTime, Socket::SELECT_READ))
			{
				Lucid::Net::SocketAddress sourceAddress;
				int n = _socket.receiveFrom(buffer.begin(), BUFFER_SIZE, sourceAddress);
				if (n > 0)
				{
					_queue.enqueueNotification(new MessageNotification(buffer.begin(), n, sourceAddress));
				}
			}
		}
		catch (...)
		{
			// lazy exception catching
		}
	}
}


void RemoteUDPListener::safeStop()
{
	_stopped = true;
}


//
// SyslogParser
//


class SyslogParser: public Lucid::Runnable
{
public:
	static const std::string NILVALUE;

	enum
	{
		WAITTIME_MILLISEC = 1000
	};

	SyslogParser(Lucid::NotificationQueue& queue, RemoteSyslogListener* pListener);
	~SyslogParser();

	void parse(const std::string& line, Lucid::Message& message);
	void run();
	void safeStop();

	static Lucid::Message::Priority convert(RemoteSyslogChannel::Severity severity);

private:
	void parsePrio(const std::string& line, std::size_t& pos, RemoteSyslogChannel::Severity& severity, RemoteSyslogChannel::Facility& fac);
	void parseNew(const std::string& line, RemoteSyslogChannel::Severity severity, RemoteSyslogChannel::Facility fac, std::size_t& pos, Lucid::Message& message);
	void parseBSD(const std::string& line, RemoteSyslogChannel::Severity severity, RemoteSyslogChannel::Facility fac, std::size_t& pos, Lucid::Message& message);

	static std::string parseUntilSpace(const std::string& line, std::size_t& pos);
		/// Parses until it encounters the next space char, returns the string from pos, excluding space
		/// pos will point past the space char

	static std::string parseStructuredData(const std::string& line, std::size_t& pos);
		/// Parses the structured data field.

	static std::string parseStructuredDataToken(const std::string& line, std::size_t& pos);
	/// Parses a token from the structured data field.

private:
	Lucid::NotificationQueue& _queue;
	bool                     _stopped;
	RemoteSyslogListener*    _pListener;
};


const std::string SyslogParser::NILVALUE("-");


SyslogParser::SyslogParser(Lucid::NotificationQueue& queue, RemoteSyslogListener* pListener):
	_queue(queue),
	_stopped(false),
	_pListener(pListener)
{
	poco_check_ptr (_pListener);
}


SyslogParser::~SyslogParser()
{
}


void SyslogParser::run()
{
	while (!_stopped)
	{
		try
		{
			Lucid::AutoPtr<Lucid::Notification> pNf(_queue.waitDequeueNotification(WAITTIME_MILLISEC));
			if (pNf)
			{
				Lucid::AutoPtr<MessageNotification> pMsgNf = pNf.cast<MessageNotification>();
				Lucid::Message message;
				parse(pMsgNf->message(), message);
				message["addr"] =pMsgNf->sourceAddress().host().toString();
				_pListener->log(message);
			}
		}
		catch (Lucid::Exception&)
		{
			// parsing exception, what should we do?
		}
		catch (...)
		{
		}
	}
}


void SyslogParser::safeStop()
{
	_stopped = true;
}


void SyslogParser::parse(const std::string& line, Lucid::Message& message)
{
	// <int> -> int: lower 3 bits severity, upper bits: facility
	std::size_t pos = 0;
	RemoteSyslogChannel::Severity severity;
	RemoteSyslogChannel::Facility fac;
	parsePrio(line, pos, severity, fac);

	// the next field decide if we parse an old BSD message or a new syslog message
	// BSD: expects a month value in string form: Jan, Feb...
	// SYSLOG expects a version number: 1
	
	if (Lucid::Ascii::isDigit(line[pos]))
	{
		parseNew(line, severity, fac, pos, message);
	}
	else
	{
		parseBSD(line, severity, fac, pos, message);
	}
	poco_assert (pos == line.size());
}


void SyslogParser::parsePrio(const std::string& line, std::size_t& pos, RemoteSyslogChannel::Severity& severity, RemoteSyslogChannel::Facility& fac)
{
	poco_assert (pos < line.size());
	poco_assert (line[pos] == '<');
	++pos;
	std::size_t start = pos;
	
	while (pos < line.size() && Lucid::Ascii::isDigit(line[pos]))
		++pos;
	
	poco_assert (line[pos] == '>');
	poco_assert (pos - start > 0);
	std::string valStr = line.substr(start, pos - start);
	++pos; // skip the >

	int val = Lucid::NumberParser::parse(valStr);
	poco_assert (val >= 0 && val <= (RemoteSyslogChannel::SYSLOG_LOCAL7 + RemoteSyslogChannel::SYSLOG_DEBUG));
	
	Lucid::UInt16 pri = static_cast<Lucid::UInt16>(val);
	// now get the lowest 3 bits
	severity = static_cast<RemoteSyslogChannel::Severity>(pri & 0x0007u);
	fac = static_cast<RemoteSyslogChannel::Facility>(pri & 0xfff8u);
}


void SyslogParser::parseNew(const std::string& line, RemoteSyslogChannel::Severity severity, RemoteSyslogChannel::Facility fac, std::size_t& pos, Lucid::Message& message)
{
	Lucid::Message::Priority prio = convert(severity);
	// rest of the unparsed header is:
	// VERSION SP TIMESTAMP SP HOSTNAME SP APP-NAME SP PROCID SP MSGID
	std::string versionStr(parseUntilSpace(line, pos));
	std::string timeStr(parseUntilSpace(line, pos)); // can be the nilvalue!
	std::string hostName(parseUntilSpace(line, pos));
	std::string appName(parseUntilSpace(line, pos));
	std::string procId(parseUntilSpace(line, pos));
	std::string msgId(parseUntilSpace(line, pos));
	std::string sd(parseStructuredData(line, pos));
	std::string messageText(line.substr(pos));
	pos = line.size();
	Lucid::DateTime date;
	int tzd = 0;
	bool hasDate = Lucid::DateTimeParser::tryParse(RemoteSyslogChannel::SYSLOG_TIMEFORMAT, timeStr, date, tzd);
	Lucid::Message logEntry(msgId, messageText, prio);
	logEntry[RemoteSyslogListener::LOG_PROP_HOST] = hostName;
	logEntry[RemoteSyslogListener::LOG_PROP_APP] = appName;
	logEntry[RemoteSyslogListener::LOG_PROP_STRUCTURED_DATA] = sd;
	
	if (hasDate)
		logEntry.setTime(date.timestamp());
	int lval(0);
	Lucid::NumberParser::tryParse(procId, lval);
	logEntry.setPid(lval);
	message.swap(logEntry);
}


void SyslogParser::parseBSD(const std::string& line, RemoteSyslogChannel::Severity severity, RemoteSyslogChannel::Facility fac, std::size_t& pos, Lucid::Message& message)
{
	Lucid::Message::Priority prio = convert(severity);
	// rest of the unparsed header is:
	// "%b %f %H:%M:%S" SP hostname|ipaddress
	// detect three spaces
	int spaceCnt = 0;
	std::size_t start = pos;
	while (spaceCnt < 3 && pos < line.size())
	{
		if (line[pos] == ' ')
		{
			spaceCnt++;
			if (spaceCnt == 1)
			{
				// size must be 3 chars for month
				if (pos - start != 3)
				{
					// probably a shortened time value, or the hostname
					// assume hostName
					Lucid::Message logEntry(line.substr(start, pos-start), line.substr(pos+1), prio);
					message.swap(logEntry);
					return;
				}
			}
			else if (spaceCnt == 2)
			{
				// a day value!
				if (!(Lucid::Ascii::isDigit(line[pos-1]) && (Lucid::Ascii::isDigit(line[pos-2]) || Lucid::Ascii::isSpace(line[pos-2]))))
				{
					// assume the next field is a hostname
					spaceCnt = 3;
				}
			}
			if (pos + 1 < line.size() && line[pos+1] == ' ')
			{
				// we have two spaces when the day value is smaller than 10!
				++pos; // skip one
			}
		}
		++pos;
	}
	std::string timeStr(line.substr(start, pos-start-1));
	int tzd(0);
	Lucid::DateTime date;
	int year = date.year(); // year is not included, use the current one
	bool hasDate = Lucid::DateTimeParser::tryParse(RemoteSyslogChannel::BSD_TIMEFORMAT, timeStr, date, tzd);
	if (hasDate)
	{
		int m = date.month();
		int d = date.day();
		int h = date.hour();
		int min = date.minute();
		int sec = date.second();
		date = Lucid::DateTime(year, m, d, h, min, sec);
	}
	// next entry is host SP
	std::string hostName(parseUntilSpace(line, pos));

	// TAG: at most 32 alphanumeric chars, ANY non alphannumeric indicates start of message content
	// ignore: treat everything as content
	std::string messageText(line.substr(pos));
	pos = line.size();
	Lucid::Message logEntry(hostName, messageText, prio);
	logEntry.setTime(date.timestamp());
	message.swap(logEntry);
}


std::string SyslogParser::parseUntilSpace(const std::string& line, std::size_t& pos)
{
	std::size_t start = pos;
	while (pos < line.size() && !Lucid::Ascii::isSpace(line[pos]))
		++pos;
	// skip space
	++pos;
	return line.substr(start, pos-start-1);
}


std::string SyslogParser::parseStructuredData(const std::string& line, std::size_t& pos)
{
	std::string sd;
	if (pos < line.size())
	{
		if (line[pos] == '-') 
		{
			++pos;
		}
		else if (line[pos] == '[')
		{
			std::string tok = parseStructuredDataToken(line, pos);
			while (tok == "[")
			{
				sd += tok;
				tok = parseStructuredDataToken(line, pos);
				while (tok != "]" && !tok.empty())
				{
					sd += tok;
					tok = parseStructuredDataToken(line, pos);
				}
				sd += tok;
				if (pos < line.size() && line[pos] == '[') tok = parseStructuredDataToken(line, pos);
			}
		}
		if (pos < line.size() && Lucid::Ascii::isSpace(line[pos])) ++pos;
	}
	return sd;
}


std::string SyslogParser::parseStructuredDataToken(const std::string& line, std::size_t& pos)
{
	std::string tok;
	if (pos < line.size())
	{
		if (Lucid::Ascii::isSpace(line[pos]) || line[pos] == '=' || line[pos] == '[' || line[pos] == ']')
		{
			tok += line[pos++];
		}
		else if (line[pos] == '"')
		{
			tok += line[pos++];
			while (pos < line.size() && line[pos] != '"')
			{
				tok += line[pos++];
			}
			tok += '"';
			if (pos < line.size()) pos++;
		}
		else
		{
			while (pos < line.size() && !Lucid::Ascii::isSpace(line[pos]) && line[pos] != '=')
			{
				tok += line[pos++];
			}
		}
	}
	return tok;
}

Lucid::Message::Priority SyslogParser::convert(RemoteSyslogChannel::Severity severity)
{
	switch (severity)
	{
	case RemoteSyslogChannel::SYSLOG_EMERGENCY:
		return Lucid::Message::PRIO_FATAL;
	case RemoteSyslogChannel::SYSLOG_ALERT:
		return Lucid::Message::PRIO_FATAL;
	case RemoteSyslogChannel::SYSLOG_CRITICAL:
		return Lucid::Message::PRIO_CRITICAL;
	case RemoteSyslogChannel::SYSLOG_ERROR:
		return Lucid::Message::PRIO_ERROR;
	case RemoteSyslogChannel::SYSLOG_WARNING:
		return Lucid::Message::PRIO_WARNING;
	case RemoteSyslogChannel::SYSLOG_NOTICE:
		return Lucid::Message::PRIO_NOTICE;
	case RemoteSyslogChannel::SYSLOG_INFORMATIONAL:
		return Lucid::Message::PRIO_INFORMATION;
	case RemoteSyslogChannel::SYSLOG_DEBUG:
		return Lucid::Message::PRIO_DEBUG;
	}
	throw Lucid::LogicException("Illegal severity value in message");
}


//
// RemoteSyslogListener
//


const std::string RemoteSyslogListener::PROP_PORT("port");
const std::string RemoteSyslogListener::PROP_THREADS("threads");

const std::string RemoteSyslogListener::LOG_PROP_APP("app");
const std::string RemoteSyslogListener::LOG_PROP_HOST("host");
const std::string RemoteSyslogListener::LOG_PROP_STRUCTURED_DATA("structured-data");


RemoteSyslogListener::RemoteSyslogListener():
	_pListener(0),
	_pParser(0),
	_port(RemoteSyslogChannel::SYSLOG_PORT),
	_threads(1)
{
}


RemoteSyslogListener::RemoteSyslogListener(Lucid::UInt16 port):
	_pListener(0),
	_pParser(0),
	_port(port),
	_threads(1)
{
}


RemoteSyslogListener::RemoteSyslogListener(Lucid::UInt16 port, int threads):
	_pListener(0),
	_pParser(0),
	_port(port),
	_threads(threads)
{
}


RemoteSyslogListener::~RemoteSyslogListener()
{
}


void RemoteSyslogListener::processMessage(const std::string& messageText)
{
	Lucid::Message message;
	_pParser->parse(messageText, message);
	log(message);
}


void RemoteSyslogListener::enqueueMessage(const std::string& messageText, const Lucid::Net::SocketAddress& senderAddress)
{
	_queue.enqueueNotification(new MessageNotification(messageText, senderAddress));
}


void RemoteSyslogListener::setProperty(const std::string& name, const std::string& value)
{
	if (name == PROP_PORT)
	{
		int val = Lucid::NumberParser::parse(value);
		if (val >= 0 && val < 65536)
			_port = static_cast<Lucid::UInt16>(val);
		else
			throw Lucid::InvalidArgumentException("Not a valid port number", value);
	}
	else if (name == PROP_THREADS)
	{
		int val = Lucid::NumberParser::parse(value);
		if (val > 0 && val < 16)
			_threads = val;
		else
			throw Lucid::InvalidArgumentException("Invalid number of threads", value);
	}
	else 
	{
		SplitterChannel::setProperty(name, value);
	}
}


std::string RemoteSyslogListener::getProperty(const std::string& name) const
{
	if (name == PROP_PORT)
		return Lucid::NumberFormatter::format(_port);
	else if (name == PROP_THREADS)
		return Lucid::NumberFormatter::format(_threads);
	else	
		return SplitterChannel::getProperty(name);
}


void RemoteSyslogListener::open()
{
	SplitterChannel::open();
	_pParser = new SyslogParser(_queue, this);
	if (_port > 0)
	{
		_pListener = new RemoteUDPListener(_queue, _port);
	}
	for (int i = 0; i < _threads; i++)
	{
		_threadPool.start(*_pParser);
	}
	if (_pListener)
	{
		_threadPool.start(*_pListener);
	}
}


void RemoteSyslogListener::close()
{
	if (_pListener)
	{
		_pListener->safeStop();
	}
	if (_pParser)
	{
		_pParser->safeStop();
	}
	_queue.clear();
	_threadPool.joinAll();
	delete _pListener;
	delete _pParser;
	_pListener = 0;
	_pParser = 0;
	SplitterChannel::close();
}


void RemoteSyslogListener::registerChannel()
{
	Lucid::LoggingFactory::defaultFactory().registerChannelClass("RemoteSyslogListener", new Lucid::Instantiator<RemoteSyslogListener, Lucid::Channel>);
}


} } // namespace Lucid::Net
