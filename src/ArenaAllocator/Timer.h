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
	Timer() noexcept;
	long getNanoseconds() const noexcept;

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
};

} // namespace ArenaAllocator

#endif // ARENA_ALLOCATOR_TIMER_H_INCLUDED
