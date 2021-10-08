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
	stm_trace[0] = (static_cast<unsigned>(operationType) + 1) * 3;
}

Timer::~Timer() noexcept
{
	if (!terminated) {
		stm_trace[0] = (static_cast<unsigned>(operationType) + 1) * 3 + 2;
	}
}

std::chrono::nanoseconds Timer::getNanoseconds() noexcept
{
	stm_trace[0] = (static_cast<unsigned>(operationType) + 1) * 3 + 1;
	terminated = true;
	return std::chrono::nanoseconds{0};
}

} // namespace ArenaAllocator
