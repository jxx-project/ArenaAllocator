//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/AllocationMap.h"

namespace ArenaAllocator {

AllocationMap::AllocationMap(Logger const& log) noexcept : log{log}
{
}

std::optional<AllocationMap::AggregateType::iterator> AllocationMap::find(void* ptr) noexcept
{
	std::optional<iterator> result;
	iterator it{aggregate.find(ptr)};
	if (it != aggregate.end()) {
		result.emplace(it);
	}
	return result;
}

void AllocationMap::registerAllocation(void* ptr, Allocation const& allocation) noexcept
{
	if (aggregate.emplace(ptr, allocation).second) {
		log(LogLevel::DEBUG,
			"\tAllocationMap::registerAllocation(%p, {[%lu, %lu], %lu})\n",
			ptr,
			allocation.pool->getRange().first,
			allocation.pool->getRange().last,
			allocation.size);
		allocation.pool->registerAllocate(allocation.size);
	} else {
		log(LogLevel::ERROR,
			"\tAllocationMap::registerAllocation(%p, {[%lu, %lu], %lu}): pointer already registered\n",
			ptr,
			allocation.pool->getRange().first,
			allocation.pool->getRange().last,
			allocation.size);
	}
}

void AllocationMap::unregisterAllocation(AggregateType::iterator it) noexcept
{
	log(LogLevel::DEBUG,
		"\tAllocationMap::unregisterAllocation(%p, {[%lu, %lu], %lu})\n",
		it->first,
		it->second.pool->getRange().first,
		it->second.pool->getRange().last,
		it->second.size);
	it->second.pool->registerDeallocate();
	aggregate.erase(it);
}

void AllocationMap::updateAllocation(AggregateType::iterator it, void* ptr, Allocation const& allocation) noexcept
{
	if (it->first == ptr) {
		log(LogLevel::DEBUG,
			"\tAllocationMap::updateAllocation(%p, {[%lu, %lu], %lu})\n",
			ptr,
			allocation.pool->getRange().first,
			allocation.pool->getRange().last,
			allocation.size);
		it->second.pool->registerDeallocate();
		allocation.pool->registerAllocate(allocation.size);
		it->second = allocation;
	} else {
		unregisterAllocation(it);
		registerAllocation(ptr, allocation);
	}
}

void AllocationMap::dump() const noexcept
{
	for (typename AggregateType::value_type const& element : aggregate) {
		log("\t%p: {pool: [%lu, %lu], size: %lu}\n",
			element.first,
			element.second.pool->getRange().first,
			element.second.pool->getRange().last,
			element.second.size);
	}
}

} // namespace ArenaAllocator
