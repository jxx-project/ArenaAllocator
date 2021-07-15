//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_Console_h_INCLUDED
#define ArenaAllocator_Console_h_INCLUDED

#include "ArenaAllocator/Logger.h"

namespace ArenaAllocator {

class Console : public Logger
{
public:
	Console() noexcept;
	Console(Logger const&) = delete;
	Console& operator=(Console const&) = delete;
	~Console() noexcept override;

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

	[[nodiscard]] bool isLevel(LogLevel level) const noexcept override;
	void setLevel(LogLevel level) noexcept override;

	static constexpr char const* className{"Console"};
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

#endif // ArenaAllocator_Console_h_INCLUDED
