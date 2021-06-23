//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/ConsoleLogger.h"
#include "ArenaAllocator/Format.h"
#include <algorithm>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <string_view>
#include <unistd.h>

namespace ArenaAllocator {

ConsoleLogger::ConsoleLogger() noexcept : logLevel{LogLevel::NONE}
{
	ConsoleLogger::log(
		LogLevel::DEBUG, FormattingCallback{[&] { return Message("ConsoleLogger::ConsoleLogger() -> this:{}\n", this); }});
}

ConsoleLogger::~ConsoleLogger() noexcept
{
	ConsoleLogger::log(
		LogLevel::DEBUG, FormattingCallback{[&] { return Message("ConsoleLogger::~ConsoleLogger(this:{})\n", this); }});
}

bool ConsoleLogger::isLevel(LogLevel level) const noexcept
{
	return logLevel >= level;
}

void ConsoleLogger::setLevel(LogLevel level) noexcept
{
	logLevel = level;
}

namespace {

template<typename T>
void writeToBuffer(T prefix, Static::FormatResult message) noexcept
{
	Message out("[pid:{}]\t{}\t{}", ::getpid(), prefix, message);
	Static::BasicLogger::writeLine(out.getResult());
}

} // namespace

void ConsoleLogger::log(Formatter const& formatter) const noexcept
{
	Message message{formatter()};
	writeToBuffer(std::string_view{}, message.getResult());
}

void ConsoleLogger::log(std::chrono::nanoseconds duration, OperationType, Formatter const& formatter) const noexcept
{
	Message message = formatter();
	writeToBuffer(duration, message.getResult());
}

void ConsoleLogger::log(LogLevel level, Formatter const& formatter) const noexcept
{
	if (isLevel(level)) {
		Message message{formatter()};
		writeToBuffer(std::string_view{}, message.getResult());
	}
}

void ConsoleLogger::logAbort(Static::BasicLogger::Formatter const& formatter) noexcept
{
	Message message{formatter()};
	writeToBuffer(std::string_view{"ArenaAllocator abort:"}, message.getResult());
	std::abort();
}

void ConsoleLogger::logExit(Static::BasicLogger::Formatter const& formatter) noexcept
{
	Message message{formatter()};
	writeToBuffer(std::string_view{"ArenaAllocator exit:"}, message.getResult());
	std::exit(-1);
}

} // namespace ArenaAllocator
