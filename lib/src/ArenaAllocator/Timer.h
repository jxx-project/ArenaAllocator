//
// Copyright (C) 2018 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_Timer_h_INCLUDED
#define ArenaAllocator_Timer_h_INCLUDED

#include <chrono>

namespace ArenaAllocator {

class Timer
{
public:
	using ClockType = std::chrono::high_resolution_clock;

	Timer() noexcept;

	[[nodiscard]] std::chrono::nanoseconds getNanoseconds() const noexcept;

private:
	std::chrono::time_point<ClockType> startTime;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_Timer_h_INCLUDED
