//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/Console.h"
#include <algorithm>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <string_view>
#include <unistd.h>

namespace ArenaAllocator {

Console::Console() noexcept : logLevel{LogLevel::NONE}
{
	Console::log(LogLevel::DEBUG, FormattingCallback{[&] { return Message("Console::Console() -> this:{}\n", this); }});
}

Console::~Console() noexcept
{
	Console::log(LogLevel::DEBUG, FormattingCallback{[&] { return Message("Console::~Console(this:{})\n", this); }});
}

bool Console::isLevel(LogLevel level) const noexcept
{
	return logLevel >= level;
}

void Console::setLevel(LogLevel level) noexcept
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

void Console::log(Formatter const& formatter) const noexcept
{
	Message message{formatter()};
	writeToBuffer(std::string_view{}, message.getResult());
}

void Console::log(std::chrono::nanoseconds duration, OperationType, Formatter const& formatter) const noexcept
{
	Message message = formatter();
	writeToBuffer(duration, message.getResult());
}

void Console::log(LogLevel level, Formatter const& formatter) const noexcept
{
	if (isLevel(level)) {
		Message message{formatter()};
		writeToBuffer(std::string_view{}, message.getResult());
	}
}

void Console::logAbort(Static::BasicLogger::Formatter const& formatter) noexcept
{
	Message message{formatter()};
	writeToBuffer(std::string_view{"ArenaAllocator abort:"}, message.getResult());
	std::abort();
}

void Console::logExit(Static::BasicLogger::Formatter const& formatter) noexcept
{
	Message message{formatter()};
	writeToBuffer(std::string_view{"ArenaAllocator exit:"}, message.getResult());
	std::exit(-1);
}

} // namespace ArenaAllocator
