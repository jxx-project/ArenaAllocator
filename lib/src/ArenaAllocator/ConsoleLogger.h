//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_ConsoleLogger_h_INCLUDED
#define ArenaAllocator_ConsoleLogger_h_INCLUDED

#include "ArenaAllocator/Logger.h"
#include <cstdarg>
#include <iostream> //DEBUG

namespace ArenaAllocator {

class ConsoleLogger : public Logger
{
public:
	ConsoleLogger() noexcept;
	ConsoleLogger(Logger const&) = delete;
	ConsoleLogger& operator=(ConsoleLogger const&) = delete;
	~ConsoleLogger() noexcept override;

	template<typename F>
	[[noreturn]] static void abort(F callback) noexcept
	{
		logAbort(FormattingCallback{callback});
	}

	template<typename F>
	[[noreturn]] static void exit(F callback) noexcept
	{
		logExit(FormattingCallback{callback});
	}

	void operator()(char const* fmt, ...) const noexcept override;
	void operator()(std::chrono::nanoseconds duration, OperationType, char const* fmt, ...) const noexcept override;
	void operator()(LogLevel level, char const* fmt, ...) const noexcept override;
	[[nodiscard]] bool isLevel(LogLevel level) const noexcept override;
	void setLevel(LogLevel level) noexcept override;

	static constexpr char const* className{"ConsoleLogger"};
	static constexpr std::size_t bufferSize{1024};

protected:
	void log(Formatter const& formatter) const noexcept override;
	void log(std::chrono::nanoseconds duration, OperationType, Formatter const& formatter) const noexcept override;
	void log(LogLevel level, Formatter const& formatter) const noexcept override;

private:
	[[noreturn]] static void logAbort(Formatter const& formatter) noexcept;
	[[noreturn]] static void logExit(Formatter const& formatter) noexcept;

	LogLevel logLevel;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_ConsoleLogger_h_INCLUDED
