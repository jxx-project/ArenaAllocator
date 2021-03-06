//
// Copyright (C) 2018 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_Timer_h_INCLUDED
#define ArenaAllocator_Timer_h_INCLUDED

#include <chrono>
#include <sys/resource.h>

namespace ArenaAllocator {

class Timer
{
public:
	using ClockType = std::chrono::steady_clock;

	Timer() noexcept;

	[[nodiscard]] std::chrono::nanoseconds getNanoseconds() const noexcept;

private:
	rusage startUsage;
	std::chrono::time_point<ClockType> startTime;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_Timer_h_INCLUDED
