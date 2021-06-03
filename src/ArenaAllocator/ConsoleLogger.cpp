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
	::ssize_t prefixLengthOrError{std::snprintf(buffer, sizeof(buffer), "[pid:%u] ", ::getpid())};
	std::size_t prefixLength{static_cast<std::size_t>(prefixLengthOrError >= 0 ? prefixLengthOrError : 0)};
	::ssize_t messageLengthOrError{std::vsnprintf(buffer + prefixLength, sizeof(buffer) - prefixLength, fmt, argp)};
	std::size_t totalLength{std::min(prefixLength + (messageLengthOrError >= 0 ? messageLengthOrError : 0), sizeof(buffer))};

	std::size_t totalBytesWritten{0};
	::ssize_t bytesWritten;
	while ((bytesWritten = ::write(STDERR_FILENO, buffer + totalBytesWritten, totalLength - totalBytesWritten)) > 0) {
		totalBytesWritten += bytesWritten;
	}

	errno = propagateErrno;
}

} // namespace ArenaAllocator
