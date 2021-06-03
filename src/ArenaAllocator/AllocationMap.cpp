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

void AllocationMap::registerAllocation(void* ptr, Allocation const& allocation) noexcept
{
	if (aggregate.emplace(ptr, allocation).second) {
		logger.debug(
			"AllocationMap::registerAllocation(%p, {[%lu, %lu], %lu})\n",
			ptr,
			allocation.pool->getRange().first,
			allocation.pool->getRange().last,
			allocation.size);
		allocation.pool->registerAllocate(allocation.size);
	} else {
		logger.error(
			"AllocationMap::registerAllocation(%p, {[%lu, %lu], %lu}): pointer already registered\n",
			ptr,
			allocation.pool->getRange().first,
			allocation.pool->getRange().last,
			allocation.size);
	}
}

void AllocationMap::unregisterAllocation(AggregateType::const_iterator it) noexcept
{
	logger.debug(
		"AllocationMap::unregisterAllocation(({%p, {[%lu, %lu], %lu}})\n",
		it->first,
		it->second.pool->getRange().first,
		it->second.pool->getRange().last,
		it->second.size);
	it->second.pool->registerDeallocate();
	aggregate.erase(it);
}

void AllocationMap::updateAllocation(AggregateType::const_iterator it, void* ptr, Allocation const& allocation) noexcept
{
	unregisterAllocation(it);
	registerAllocation(ptr, allocation);
}

void AllocationMap::dump() const noexcept
{
	for (typename AggregateType::value_type const& element : aggregate) {
		logger.log(
			"%p: {pool: [%lu, %lu], size: %lu}\n",
			element.first,
			element.second.pool->getRange().first,
			element.second.pool->getRange().last,
			element.second.size);
	}
}

} // namespace ArenaAllocator
