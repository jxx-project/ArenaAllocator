//
// Copyright (C) 2018 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ARENA_ALLOCATOR_TIMER_H_INCLUDED
#define ARENA_ALLOCATOR_TIMER_H_INCLUDED

#include <chrono>

namespace ArenaAllocator {

class Timer
{
public:
	using ClockType = std::chrono::high_resolution_clock;

	Timer() noexcept;

	std::chrono::nanoseconds getNanoseconds() const noexcept;

private:
	std::chrono::time_point<ClockType> startTime;
};

} // namespace ArenaAllocator

#endif // ARENA_ALLOCATOR_TIMER_H_INCLUDED
