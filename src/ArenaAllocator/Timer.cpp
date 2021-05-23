//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/Timer.h"

namespace ArenaAllocator {

Timer::Timer() noexcept : startTime{std::chrono::high_resolution_clock::now()}
{
}

long Timer::getNanoseconds() const noexcept
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - startTime).count();
}

} // namespace ArenaAllocator
