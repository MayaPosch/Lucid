//
// LoggingSubsystem.cpp
//
// Library: Util
// Package: Application
// Module:  LoggingSubsystem
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "lucid/Util/LoggingSubsystem.h"
#include "lucid/Util/LoggingConfigurator.h"
#include "lucid/Util/Application.h"
#include "lucid/Logger.h"


using Lucid::Logger;


namespace Lucid {
namespace Util {


LoggingSubsystem::LoggingSubsystem()
{
}


LoggingSubsystem::~LoggingSubsystem()
{
}


const char* LoggingSubsystem::name() const
{
	return "Logging Subsystem";
}

	
void LoggingSubsystem::initialize(Application& app)
{
	LoggingConfigurator configurator;
	configurator.configure(app.configPtr());
	std::string logger = app.config().getString("application.logger", "Application");
	app.setLogger(Logger::get(logger));
}


void LoggingSubsystem::uninitialize()
{
}


} } // namespace Lucid::Util
