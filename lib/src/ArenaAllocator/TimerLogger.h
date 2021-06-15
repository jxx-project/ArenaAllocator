//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_TimerLogger_h_INCLUDED
#define ArenaAllocator_TimerLogger_h_INCLUDED

#include "ArenaAllocator/Logger.h"
#include <cstdarg>
#include <string_view>

namespace ArenaAllocator {

class TimerLogger : public Logger
{
public:
	TimerLogger() noexcept;
	TimerLogger(Logger const&) = delete;
	TimerLogger& operator=(TimerLogger const&) = delete;
	~TimerLogger() noexcept override;

	void operator()(char const* fmt, ...) const noexcept override;
	void operator()(std::chrono::nanoseconds duration, OperationType, char const* fmt, ...) const noexcept override;
	void operator()(LogLevel level, char const* fmt, ...) const noexcept override;
	[[nodiscard]] bool isLevel(LogLevel level) const noexcept override;
	void setLevel(LogLevel level) noexcept override;

	static constexpr std::string_view className{"TimerLogger"};
	static constexpr std::size_t bufferSize{1024};

private:
	LogLevel logLevel;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_TimerLogger_h_INCLUDED
