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
	ConsoleLogger::operator()(LogLevel::DEBUG, "\tConsoleLogger::ConsoleLogger()\n");
}

ConsoleLogger::~ConsoleLogger() noexcept
{
	ConsoleLogger::operator()(LogLevel::DEBUG, "\tConsoleLogger::~ConsoleLogger()\n");
}

void ConsoleLogger::operator()(char const* fmt, ...) const noexcept
{
	std::va_list argp;
	::va_start(argp, fmt);
	write(fmt, argp);
	::va_end(argp);
}

void ConsoleLogger::operator()(std::chrono::nanoseconds duration, OperationType, char const* fmt, ...) const noexcept
{
	std::va_list argp;
	::va_start(argp, fmt);
	write(duration, fmt, argp);
	::va_end(argp);
}

void ConsoleLogger::operator()(LogLevel level, char const* fmt, ...) const noexcept
{
	if (isLevel(level)) {
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

void ConsoleLogger::write(char const* fmt, std::va_list argp) noexcept
{
	char buffer[1024];
	::ssize_t prefixLengthOrError{std::snprintf(buffer, sizeof(buffer) - 1, "[pid:%u]\t\t", ::getpid())};
	std::size_t prefixLength{static_cast<std::size_t>(prefixLengthOrError >= 0 ? prefixLengthOrError : 0)};
	::ssize_t messageLengthOrError{std::vsnprintf(buffer + prefixLength, sizeof(buffer) - 1 - prefixLength, fmt, argp)};
	write(buffer, std::min(prefixLength + (messageLengthOrError >= 0 ? messageLengthOrError : 0), sizeof(buffer) - 1));
}

void ConsoleLogger::write(std::chrono::nanoseconds duration, char const* fmt, std::va_list argp) noexcept
{
	char buffer[1024];
	::ssize_t prefixLengthOrError{std::snprintf(buffer, sizeof(buffer) - 1, "[pid:%u]\t%luns\t", ::getpid(), duration.count())};
	std::size_t prefixLength{static_cast<std::size_t>(prefixLengthOrError >= 0 ? prefixLengthOrError : 0)};
	::ssize_t messageLengthOrError{std::vsnprintf(buffer + prefixLength, sizeof(buffer) - 1 - prefixLength, fmt, argp)};
	write(buffer, std::min(prefixLength + (messageLengthOrError >= 0 ? messageLengthOrError : 0), sizeof(buffer) - 1));
}

void ConsoleLogger::write(char buffer[], std::size_t length) noexcept
{
	int propagateErrno{errno};

	buffer[length++] = '\n';
	std::size_t totalBytesWritten{0};
	::ssize_t bytesWritten;
	while ((bytesWritten = ::write(STDERR_FILENO, &buffer[totalBytesWritten], length - totalBytesWritten)) > 0) {
		totalBytesWritten += bytesWritten;
	}

	errno = propagateErrno;
}

} // namespace ArenaAllocator
