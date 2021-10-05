//
// Copyright (C) 2018 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_Timer_h_INCLUDED
#define ArenaAllocator_Timer_h_INCLUDED

#include "ArenaAllocator/OperationType.h"
#include <chrono>

namespace ArenaAllocator {

class Timer
{
public:
	using ClockType = std::chrono::steady_clock;

	Timer(OperationType operationType) noexcept;
	~Timer() noexcept;

	[[nodiscard]] std::chrono::nanoseconds getNanoseconds() const noexcept;

private:
	OperationType operationType;
	std::chrono::time_point<ClockType> startTime;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_Timer_h_INCLUDED
