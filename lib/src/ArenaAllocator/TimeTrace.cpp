//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/TimeTrace.h"
#include <algorithm>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <string_view>
#include <unistd.h>

namespace ArenaAllocator {

namespace {

void writeToBuffer(Static::FormatResult message) noexcept
{
	Message out("[pid:{}]\t\t{}", ::getpid(), message);
	Static::BasicLogger::writeLine(out.getResult());
}

} // namespace

TimeTrace::TimeTrace() noexcept : logLevel{LogLevel::NONE}
{
	TimeTrace::log(LogLevel::DEBUG, FormattingCallback{[&] { return Message("TimeTrace::TimeTrace() -> this:{}\n", this); }});
}

TimeTrace::~TimeTrace() noexcept
{
	TimeTrace::log(LogLevel::DEBUG, FormattingCallback{[&] { return Message("TimeTrace::~TimeTrace(this:{})\n", this); }});
}

bool TimeTrace::isLevel(LogLevel level) const noexcept
{
	return logLevel >= level;
}

void TimeTrace::setLevel(LogLevel level) noexcept
{
	logLevel = level;
}

void TimeTrace::log(Formatter const& formatter) const noexcept
{
	Message message = formatter();
	writeToBuffer(message.getResult());
}

void TimeTrace::log(std::chrono::nanoseconds duration, OperationType operationType, Formatter const& formatter) const noexcept
{
	Static::BasicLogger::writeLine(
		Message{"[pid:{}]\tTimeTrace:{},{}", ::getpid(), to_string(operationType), duration.count()}.getResult());
}

void TimeTrace::log(LogLevel level, Formatter const& formatter) const noexcept
{
	if (isLevel(level)) {
		Message message = formatter();
		writeToBuffer(message.getResult());
	}
}

} // namespace ArenaAllocator
