//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/Timer.h"
#include <stm/stm.h>

namespace ArenaAllocator {

Timer::Timer(OperationType operationType) noexcept : operationType{operationType}, terminated{false}
{
	stm_trace[static_cast<unsigned>(operationType)] = 0x1;
}

Timer::~Timer() noexcept
{
	if (!terminated) {
		stm_trace[static_cast<unsigned>(operationType)] = 0x3;
	}
}

std::chrono::nanoseconds Timer::getNanoseconds() noexcept
{
	stm_trace[static_cast<unsigned>(operationType)] = 0x2;
	terminated = true;
	return std::chrono::nanoseconds{0};
}

} // namespace ArenaAllocator
