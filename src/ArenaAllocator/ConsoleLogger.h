//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ARENA_ALLOCATOR_CONSOLE_LOGGER_H_INCLUDED
#define ARENA_ALLOCATOR_CONSOLE_LOGGER_H_INCLUDED

#include "ArenaAllocator/Logger.h"
#include <cstdarg>

namespace ArenaAllocator {

class ConsoleLogger : public Logger
{
public:
	ConsoleLogger() noexcept;
	ConsoleLogger(Logger const&) = delete;
	ConsoleLogger& operator=(ConsoleLogger const&) = delete;
	virtual ~ConsoleLogger() noexcept;

	virtual void operator()(char const* fmt, ...) const noexcept override;
	virtual void operator()(std::chrono::nanoseconds duration, char const* fmt, ...) const noexcept override;
	virtual void operator()(LogLevel level, char const* fmt, ...) const noexcept override;
	virtual bool isLevel(LogLevel level) const noexcept override;
	virtual void setLevel(LogLevel level) noexcept override;

private:
	static void write(const char* fmt, std::va_list argp) noexcept;
	static void write(std::chrono::nanoseconds duration, const char* fmt, std::va_list argp) noexcept;
	static void write(char buffer[], std::size_t length) noexcept;
	LogLevel logLevel;
};

} // namespace ArenaAllocator

#endif // ARENA_ALLOCATOR_CONSOLE_LOGGER_H_INCLUDED
