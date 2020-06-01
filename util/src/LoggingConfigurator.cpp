//
// LoggingConfigurator.cpp
//
// Library: Util
// Package: Configuration
// Module:  LoggingConfigurator
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "lucid/Util/LoggingConfigurator.h"
#include "lucid/AutoPtr.h"
#include "lucid/Channel.h"
#include "lucid/FormattingChannel.h"
#include "lucid/Formatter.h"
#include "lucid/PatternFormatter.h"
#include "lucid/Logger.h"
#include "lucid/LoggingRegistry.h"
#include "lucid/LoggingFactory.h"
#include <map>


using Lucid::AutoPtr;
using Lucid::Formatter;
using Lucid::PatternFormatter;
using Lucid::Channel;
using Lucid::FormattingChannel;
using Lucid::Logger;
using Lucid::LoggingRegistry;
using Lucid::LoggingFactory;


namespace Lucid {
namespace Util {


LoggingConfigurator::LoggingConfigurator()
{
}


LoggingConfigurator::~LoggingConfigurator()
{
}


void LoggingConfigurator::configure(AbstractConfiguration::Ptr pConfig)
{
	poco_check_ptr (pConfig);

	AbstractConfiguration::Ptr pFormattersConfig(pConfig->createView("logging.formatters"));
	configureFormatters(pFormattersConfig);

	AbstractConfiguration::Ptr pChannelsConfig(pConfig->createView("logging.channels"));
	configureChannels(pChannelsConfig);

	AbstractConfiguration::Ptr pLoggersConfig(pConfig->createView("logging.loggers"));
	configureLoggers(pLoggersConfig);
}


void LoggingConfigurator::configureFormatters(AbstractConfiguration::Ptr pConfig)
{
	AbstractConfiguration::Keys formatters;
	pConfig->keys(formatters);
	for (const auto& f: formatters)
	{
		AutoPtr<AbstractConfiguration> pFormatterConfig(pConfig->createView(f));
		AutoPtr<Formatter> pFormatter(createFormatter(pFormatterConfig));
		LoggingRegistry::defaultRegistry().registerFormatter(f, pFormatter);
	}
}


void LoggingConfigurator::configureChannels(AbstractConfiguration::Ptr pConfig)
{
	AbstractConfiguration::Keys channels;
	pConfig->keys(channels);
	for (const auto& c: channels)
	{
		AutoPtr<AbstractConfiguration> pChannelConfig(pConfig->createView(c));
		AutoPtr<Channel> pChannel = createChannel(pChannelConfig);
		LoggingRegistry::defaultRegistry().registerChannel(c, pChannel);
	}
	for (const auto& c: channels)
	{
		AutoPtr<AbstractConfiguration> pChannelConfig(pConfig->createView(c));
		Channel::Ptr pChannel = LoggingRegistry::defaultRegistry().channelForName(c);
		configureChannel(pChannel, pChannelConfig);
	}
}


void LoggingConfigurator::configureLoggers(AbstractConfiguration::Ptr pConfig)
{
	using LoggerMap = std::map<std::string, AutoPtr<AbstractConfiguration>>;

	AbstractConfiguration::Keys loggers;
	pConfig->keys(loggers);
	// use a map to sort loggers by their name, ensuring initialization in correct order (parents before children)
	LoggerMap loggerMap; 
	for (const auto& l: loggers)
	{
		AutoPtr<AbstractConfiguration> pLoggerConfig(pConfig->createView(l));
		loggerMap[pLoggerConfig->getString("name", "")] = pLoggerConfig;
	}
	for (const auto& p: loggerMap)
	{
		configureLogger(p.second);
	}
}


Formatter::Ptr LoggingConfigurator::createFormatter(AbstractConfiguration::Ptr pConfig)
{
	Formatter::Ptr pFormatter(LoggingFactory::defaultFactory().createFormatter(pConfig->getString("class")));
	AbstractConfiguration::Keys props;
	pConfig->keys(props);
	for (const auto& p: props)
	{
		if (p != "class")
			pFormatter->setProperty(p, pConfig->getString(p));		
	}
	return pFormatter;
}


Channel::Ptr LoggingConfigurator::createChannel(AbstractConfiguration::Ptr pConfig)
{
	Channel::Ptr pChannel(LoggingFactory::defaultFactory().createChannel(pConfig->getString("class")));
	Channel::Ptr pWrapper(pChannel);
	AbstractConfiguration::Keys props;
	pConfig->keys(props);
	for (const auto& p: props)
	{
		if (p == "pattern")
		{
			AutoPtr<Formatter> pPatternFormatter(new PatternFormatter(pConfig->getString(p)));
			pWrapper = new FormattingChannel(pPatternFormatter, pChannel);
		}
		else if (p == "formatter")
		{
			AutoPtr<FormattingChannel> pFormattingChannel(new FormattingChannel(0, pChannel));
			if (pConfig->hasProperty("formatter.class"))
			{
				AutoPtr<AbstractConfiguration> pFormatterConfig(pConfig->createView(p));	
				AutoPtr<Formatter> pFormatter(createFormatter(pFormatterConfig));
				pFormattingChannel->setFormatter(pFormatter);
			}
			else pFormattingChannel->setProperty(p, pConfig->getString(p));
			pWrapper = pFormattingChannel;
		}
	}
	return pWrapper;
}


void LoggingConfigurator::configureChannel(Channel::Ptr pChannel, AbstractConfiguration::Ptr pConfig)
{
	AbstractConfiguration::Keys props;
	pConfig->keys(props);
	for (const auto& p: props)
	{
		if (p != "pattern" && p != "formatter" && p != "class")
		{
			pChannel->setProperty(p, pConfig->getString(p));
		}
	}
}


void LoggingConfigurator::configureLogger(AbstractConfiguration::Ptr pConfig)
{
	Logger& logger = Logger::get(pConfig->getString("name", ""));
	AbstractConfiguration::Keys props;
	pConfig->keys(props);
	for (const auto& p: props)
	{
		if (p == "channel" && pConfig->hasProperty("channel.class"))
		{
			AutoPtr<AbstractConfiguration> pChannelConfig(pConfig->createView(p));	
			AutoPtr<Channel> pChannel(createChannel(pChannelConfig));
			configureChannel(pChannel, pChannelConfig);
			Logger::setChannel(logger.name(), pChannel);
		}
		else if (p != "name")
		{
			Logger::setProperty(logger.name(), p, pConfig->getString(p));
		}
	}
}


} } // namespace Lucid::Util
