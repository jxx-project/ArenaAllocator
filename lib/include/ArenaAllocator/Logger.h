//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_Logger_h_INCLUDED
#define ArenaAllocator_Logger_h_INCLUDED

#include "ArenaAllocator/LogLevel.h"
#include "ArenaAllocator/OperationType.h"
#include <chrono>

namespace ArenaAllocator {

class Logger
{
public:
	virtual ~Logger() = default;

	virtual void operator()(char const* fmt, ...) const noexcept = 0;
	virtual void operator()(std::chrono::nanoseconds duration, OperationType opType, char const* fmt, ...) const noexcept = 0;
	virtual void operator()(LogLevel level, char const* fmt, ...) const noexcept = 0;
	virtual bool isLevel(LogLevel level) const noexcept = 0;
	virtual void setLevel(LogLevel level) noexcept = 0;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_Logger_h_INCLUDED
