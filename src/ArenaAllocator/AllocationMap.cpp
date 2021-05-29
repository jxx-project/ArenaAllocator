//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/AllocationMap.h"

namespace ArenaAllocator {

AllocationMap::AllocationMap(Logger const& logger) noexcept : logger{logger}
{
}

std::optional<AllocationMap::AggregateType::const_iterator> AllocationMap::find(void* ptr) const noexcept
{
	std::optional<AggregateType::const_iterator> result;
	AggregateType::const_iterator it{aggregate.find(ptr)};
	if (it != aggregate.end()) {
		result.emplace(it);
	}
	return result;
}

bool AllocationMap::emplace(void* ptr, Allocation const& allocation) noexcept
{
	return aggregate.emplace(ptr, allocation).second;
}

void AllocationMap::erase(AggregateType::const_iterator it) noexcept
{
	aggregate.erase(it);
}

} // namespace ArenaAllocator
