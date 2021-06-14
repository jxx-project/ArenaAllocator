//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_ConsoleLogger_h_INCLUDED
#define ArenaAllocator_ConsoleLogger_h_INCLUDED

#include "ArenaAllocator/Logger.h"
#include <cstdarg>

namespace ArenaAllocator {

class ConsoleLogger : public Logger
{
public:
	ConsoleLogger() noexcept;
	ConsoleLogger(Logger const&) = delete;
	ConsoleLogger& operator=(ConsoleLogger const&) = delete;
	~ConsoleLogger() noexcept override;

	[[noreturn]] static void abort(char const* fmt, ...) noexcept;
	[[noreturn]] static void exit(char const* fmt, ...) noexcept;
	void operator()(char const* fmt, ...) const noexcept override;
	void operator()(std::chrono::nanoseconds duration, OperationType, char const* fmt, ...) const noexcept override;
	void operator()(LogLevel level, char const* fmt, ...) const noexcept override;
	[[nodiscard]] bool isLevel(LogLevel level) const noexcept override;
	void setLevel(LogLevel level) noexcept override;

	static constexpr std::size_t bufferSize{1024};

private:
	LogLevel logLevel;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_ConsoleLogger_h_INCLUDED
