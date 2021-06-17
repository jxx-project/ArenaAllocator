//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_TimerLogger_h_INCLUDED
#define ArenaAllocator_TimerLogger_h_INCLUDED

#include "ArenaAllocator/Logger.h"

namespace ArenaAllocator {

class TimerLogger : public Logger
{
public:
	TimerLogger() noexcept;
	TimerLogger(Logger const&) = delete;
	TimerLogger& operator=(TimerLogger const&) = delete;
	~TimerLogger() noexcept override;

	[[nodiscard]] bool isLevel(LogLevel level) const noexcept override;
	void setLevel(LogLevel level) noexcept override;

	static constexpr char const* className{"TimerLogger"};
	static constexpr std::size_t bufferSize{1024};

protected:
	void log(Formatter const& formatter) const noexcept override;
	void log(std::chrono::nanoseconds duration, OperationType operationType, Formatter const& formatter) const noexcept override;
	void log(LogLevel level, Formatter const& formatter) const noexcept override;

private:
	LogLevel logLevel;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_TimerLogger_h_INCLUDED
