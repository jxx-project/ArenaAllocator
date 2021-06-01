//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/ConsoleLogger.h"
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
	va_start(argp, fmt);
	write(fmt, argp);
	va_end(argp);
}

void ConsoleLogger::error(char const* fmt, ...) const noexcept
{
	if (isLevel(LogLevel::ERROR)) {
		std::va_list argp;
		va_start(argp, fmt);
		write(fmt, argp);
		va_end(argp);
	}
}

void ConsoleLogger::info(char const* fmt, ...) const noexcept
{
	if (isLevel(LogLevel::INFO)) {
		std::va_list argp;
		va_start(argp, fmt);
		write(fmt, argp);
		va_end(argp);
	}
}

void ConsoleLogger::debug(char const* fmt, ...) const noexcept
{
	if (isLevel(LogLevel::DEBUG)) {
		std::va_list argp;
		va_start(argp, fmt);
		write(fmt, argp);
		va_end(argp);
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
	std::fprintf(stderr, "[pid: %u] ", getpid());
	std::vfprintf(stderr, fmt, argp);
	errno = propagateErrno;
}

} // namespace ArenaAllocator
