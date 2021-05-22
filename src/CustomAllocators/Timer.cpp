//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "CustomAllocators/Timer.h"

namespace CustomAllocators {

Timer::Timer() noexcept : startTime{std::chrono::high_resolution_clock::now()}
{
}

long Timer::getNanoseconds() const noexcept
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - startTime).count();
}

} // namespace CustomAllocators
