//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/Timer.h"
#include <cerrno>

namespace ArenaAllocator {

namespace {

// Ensure that errno is preserved across the Timer::ClockType::now() call.
std::chrono::time_point<Timer::ClockType> now()
{
	std::chrono::time_point<Timer::ClockType> result;
	int propagateErrno{errno};
	result = Timer::ClockType::now();
	errno = propagateErrno;
	return result;
}

} // namespace

Timer::Timer() noexcept : startTime{now()}
{
}

long Timer::getNanoseconds() const noexcept
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(now() - startTime).count();
}

} // namespace ArenaAllocator
