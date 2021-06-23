//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_Logger_h_INCLUDED
#define ArenaAllocator_Logger_h_INCLUDED

#include "ArenaAllocator/LogLevel.h"
#include "ArenaAllocator/OperationType.h"
#include <Static/BasicLogger.h>
#include <chrono>

namespace ArenaAllocator {

using Message = Static::Format::Buffer<Static::BasicLogger::maxLength>;

class Logger : public Static::BasicLogger
{
public:
	virtual ~Logger() = default;

	template<typename F>
	void operator()(F callback) const noexcept
	{
		Static::BasicLogger::log(FormattingCallback{callback});
	}

	template<typename F>
	void operator()(std::chrono::nanoseconds duration, OperationType operationType, F callback) const noexcept
	{
		log(duration, operationType, FormattingCallback{callback});
	}

	template<typename F>
	void operator()(LogLevel level, F callback) const noexcept
	{
		log(level, FormattingCallback{callback});
	}

	[[nodiscard]] virtual bool isLevel(LogLevel level) const noexcept = 0;
	virtual void setLevel(LogLevel level) noexcept = 0;

protected:
	virtual void log(std::chrono::nanoseconds duration, OperationType operationType, Formatter const& formatter) const noexcept = 0;
	virtual void log(LogLevel level, Formatter const& formatter) const noexcept = 0;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_Logger_h_INCLUDED
