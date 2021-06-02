//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/ConsoleLogger.h"
#include <algorithm>
#include <cerrno>
#include <cstdio>
#include <unistd.h>

namespace ArenaAllocator {

ConsoleLogger::ConsoleLogger() noexcept : logLevel{LogLevel::NONE}
{
	debug("ConsoleLogger::ConsoleLogger()\n");
}

ConsoleLogger::~ConsoleLogger() noexcept
{
	debug("ConsoleLogger::~ConsoleLogger()\n");
}

void ConsoleLogger::log(char const* fmt, ...) const noexcept
{
	std::va_list argp;
	::va_start(argp, fmt);
	write(fmt, argp);
	::va_end(argp);
}

void ConsoleLogger::error(char const* fmt, ...) const noexcept
{
	if (isLevel(LogLevel::ERROR)) {
		std::va_list argp;
		::va_start(argp, fmt);
		write(fmt, argp);
		::va_end(argp);
	}
}

void ConsoleLogger::info(char const* fmt, ...) const noexcept
{
	if (isLevel(LogLevel::INFO)) {
		std::va_list argp;
		::va_start(argp, fmt);
		write(fmt, argp);
		::va_end(argp);
	}
}

void ConsoleLogger::debug(char const* fmt, ...) const noexcept
{
	if (isLevel(LogLevel::DEBUG)) {
		std::va_list argp;
		::va_start(argp, fmt);
		write(fmt, argp);
		::va_end(argp);
	}
}

bool ConsoleLogger::isLevel(LogLevel level) const noexcept
{
	return logLevel >= level;
}

void ConsoleLogger::setLevel(LogLevel level) noexcept
{
	logLevel = level;
}

void ConsoleLogger::write(char const* fmt, std::va_list argp) const noexcept
{
	int propagateErrno{errno};
	char buffer[1024];
	::ssize_t prefixLenOrError{std::snprintf(buffer, sizeof(buffer), "[pid: %u] ", ::getpid())};
	std::size_t prefixLen{prefixLenOrError >= 0 ? static_cast<std::size_t>(prefixLenOrError) : 0};
	::ssize_t messageLenOrError{std::vsnprintf(buffer + prefixLen, sizeof(buffer) - prefixLen, fmt, argp)};
	std::size_t messageLen{messageLenOrError >= 0 ? static_cast<std::size_t>(messageLenOrError) : 0};
	::write(STDERR_FILENO, buffer, std::min(prefixLen + messageLen, sizeof(buffer)));
	errno = propagateErrno;
}

} // namespace ArenaAllocator
