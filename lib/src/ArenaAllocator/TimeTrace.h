//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_TimeTrace_h_INCLUDED
#define ArenaAllocator_TimeTrace_h_INCLUDED

#include "ArenaAllocator/Logger.h"

namespace ArenaAllocator {

class TimeTrace : public Logger
{
public:
	TimeTrace() noexcept;
	TimeTrace(Logger const&) = delete;
	TimeTrace& operator=(TimeTrace const&) = delete;
	~TimeTrace() noexcept override;

	[[nodiscard]] bool isLevel(LogLevel level) const noexcept override;
	void setLevel(LogLevel level) noexcept override;

	static constexpr char const* className{"TimeTrace"};
	static constexpr std::size_t bufferSize{1024};

protected:
	void log(Formatter const& formatter) const noexcept override;
	void log(std::chrono::nanoseconds duration, OperationType operationType, Formatter const& formatter) const noexcept override;
	void log(LogLevel level, Formatter const& formatter) const noexcept override;

private:
	LogLevel logLevel;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_TimeTrace_h_INCLUDED
