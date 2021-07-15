//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/Timer.h"
#include "ArenaAllocator/BuildConfiguration.h"
#include <cerrno>
#include <cstdio>
#include <cstdlib>

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

rusage getUsage()
{
	rusage result;
	if constexpr (BuildConfiguration::discardTimingAcrossContextSwitch) {
		if (getrusage(RUSAGE_SELF, &result) == -1) {
			std::perror("getrusage");
			std::exit(EXIT_FAILURE);
		}
	}
	return result;
}

} // namespace

Timer::Timer() noexcept : startUsage{getUsage()}, startTime{now()}
{
}

std::chrono::nanoseconds Timer::getNanoseconds() const noexcept
{
	std::chrono::nanoseconds result{std::chrono::duration_cast<std::chrono::nanoseconds>(now() - startTime)};
	if constexpr (BuildConfiguration::discardTimingAcrossContextSwitch) {
		rusage usage{getUsage()};
		if (usage.ru_nvcsw > startUsage.ru_nvcsw || usage.ru_nivcsw > startUsage.ru_nivcsw) {
			result = std::chrono::nanoseconds{0};
		}
	}
	return result;
}

} // namespace ArenaAllocator
