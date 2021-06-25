//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/TimerLogger.h"
#include "ArenaAllocator/Format.h"
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

TimerLogger::TimerLogger() noexcept : logLevel{LogLevel::NONE}
{
	TimerLogger::log(LogLevel::DEBUG, FormattingCallback{[&] { return Message("TimerLogger::TimerLogger() -> this:{}\n", this); }});
}

TimerLogger::~TimerLogger() noexcept
{
	TimerLogger::log(LogLevel::DEBUG, FormattingCallback{[&] { return Message("TimerLogger::~TimerLogger(this:{})\n", this); }});
}

bool TimerLogger::isLevel(LogLevel level) const noexcept
{
	return logLevel >= level;
}

void TimerLogger::setLevel(LogLevel level) noexcept
{
	logLevel = level;
}

void TimerLogger::log(Formatter const& formatter) const noexcept
{
	Message message = formatter();
	writeToBuffer(message.getResult());
}

void TimerLogger::log(std::chrono::nanoseconds duration, OperationType operationType, Formatter const& formatter) const noexcept
{
	Static::BasicLogger::writeLine(
		Message{"[pid:{}]\tTimerLogger:{},{}", ::getpid(), to_string(operationType), duration.count()}.getResult());
}

void TimerLogger::log(LogLevel level, Formatter const& formatter) const noexcept
{
	if (isLevel(level)) {
		Message message = formatter();
		writeToBuffer(message.getResult());
	}
}

} // namespace ArenaAllocator
