//
// Process_VX.cpp
//
// Library: Foundation
// Package: Processes
// Module:  Process
//
// Copyright (c) 2004-2011, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "lucid/Process_VX.h"
#include "lucid/Exception.h"


namespace Lucid {


//
// ProcessHandleImpl
//
ProcessHandleImpl::ProcessHandleImpl(int pid):
	_pid(pid)
{
}


ProcessHandleImpl::~ProcessHandleImpl()
{
}


int ProcessHandleImpl::id() const
{
	return _pid;
}


int ProcessHandleImpl::wait() const
{
	throw Lucid::NotImplementedException("Process::wait()");
}


int ProcessHandleImpl::tryWait() const
{
	throw Lucid::NotImplementedException("Process::tryWait()");
}


//
// ProcessImpl
//
ProcessImpl::PIDImpl ProcessImpl::idImpl()
{
	return 0;
}


void ProcessImpl::timesImpl(long& userTime, long& kernelTime)
{
	userTime   = 0;
	kernelTime = 0;
}


ProcessHandleImpl* ProcessImpl::launchImpl(const std::string& command, const ArgsImpl& args, const std::string& initialDirectory,Pipe* inPipe, Pipe* outPipe, Pipe* errPipe, const EnvImpl& env)
{
	throw Lucid::NotImplementedException("Process::launch()");
}


void ProcessImpl::killImpl(ProcessHandleImpl& handle)
{
	throw Lucid::NotImplementedException("Process::kill()");
}


void ProcessImpl::killImpl(PIDImpl pid)
{
	throw Lucid::NotImplementedException("Process::kill()");
}


bool ProcessImpl::isRunningImpl(const ProcessHandleImpl& handle)
{
	throw Lucid::NotImplementedException("Process::is_running()");
}


bool ProcessImpl::isRunningImpl(PIDImpl pid)
{
	throw Lucid::NotImplementedException("Process::is_running()");
}


void ProcessImpl::requestTerminationImpl(PIDImpl pid)
{
	throw Lucid::NotImplementedException("Process::requestTermination()");
}


} // namespace Lucid
