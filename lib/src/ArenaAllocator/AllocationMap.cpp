//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/AllocationMap.h"

namespace ArenaAllocator {

AllocationMap::AllocationMap(PoolMap<PoolStatistics>& pools, PoolStatistics& delegatePool, Logger const& log) noexcept :
	log{log}, pools{pools}, delegatePool{delegatePool}
{
}

void AllocationMap::registerAllocate(std::size_t size, void* result) noexcept
{
	log(LogLevel::DEBUG, "AllocationMap::registerAllocate(%lu, %p)", size, result);
	PoolStatistics* pool{pools.at(size)};
	if (!pool) {
		pool = &delegatePool;
	}
	insertAllocation(result, {pool, size});
}

void AllocationMap::registerAllocate(std::size_t size, void* result, std::size_t alignment) noexcept
{
	log(LogLevel::DEBUG, "AllocationMap::registerAllocate(%lu, %p)", size, result);
	PoolStatistics* pool{alignment <= sizeof(std::max_align_t) ? pools.at(size) : &delegatePool};
	if (!pool) {
		pool = &delegatePool;
	}
	insertAllocation(result, {pool, size});
}

void AllocationMap::registerDeallocate(void* ptr) noexcept
{
	log(LogLevel::DEBUG, "AllocationMap::registerDeallocate(%p)", ptr);
	if (ptr) {
		AggregateType::iterator it{allocations.find(ptr)};
		if (it != allocations.end()) {
			eraseAllocation(it);
		} else {
			log(LogLevel::ERROR, "AllocationMap::registerDeallocate(%p) allocation not found", ptr);
			if (log.isLevel(LogLevel::DEBUG)) {
				dump();
			}
		}
	}
}

void AllocationMap::registerReallocate(void* ptr, std::size_t size, void* result) noexcept
{
	log(LogLevel::DEBUG, "AllocationMap::registerReallocate(%p, %lu, %p)", ptr, size, result);
	if (ptr) {
		AggregateType::iterator it{allocations.find(ptr)};
		if (it != allocations.end()) {
			if (size) {
				PoolStatistics* destinationPool{pools.at(size)};
				if (!destinationPool) {
					destinationPool = &delegatePool;
					if (it->second.pool != &delegatePool) {
						log(LogLevel::ERROR,
							"AllocationMap::registerReallocate(%p, %lu, %p) allocation moved out of arena pools",
							ptr,
							size,
							result);
					}
				}
				updateAllocation(it, result, {destinationPool, size});
			} else {
				eraseAllocation(it);
			}
		} else {
			log(LogLevel::ERROR, "AllocationMap::registerReallocate(%p, %lu, %p): allocation not found", ptr, size, result);
			if (log.isLevel(LogLevel::DEBUG)) {
				dump();
			}
			registerAllocate(size, result);
		}
	} else {
		registerAllocate(size, result);
	}
}

void AllocationMap::insertAllocation(void* ptr, Allocation const& allocation) noexcept
{
	if (allocations.emplace(ptr, allocation).second) {
		log(LogLevel::DEBUG,
			"AllocationMap::insertAllocation(%p, {[%lu, %lu], %lu})",
			ptr,
			allocation.pool->getRange().first,
			allocation.pool->getRange().last,
			allocation.size);
		allocation.pool->registerAllocate(allocation.size);
	} else {
		log(LogLevel::ERROR,
			"AllocationMap::insertAllocation(%p, {[%lu, %lu], %lu}): pointer already registered",
			ptr,
			allocation.pool->getRange().first,
			allocation.pool->getRange().last,
			allocation.size);
	}
}

void AllocationMap::eraseAllocation(AggregateType::iterator it) noexcept
{
	log(LogLevel::DEBUG,
		"AllocationMap::eraseAllocation(%p, {[%lu, %lu], %lu})",
		it->first,
		it->second.pool->getRange().first,
		it->second.pool->getRange().last,
		it->second.size);
	it->second.pool->registerDeallocate();
	allocations.erase(it);
}

void AllocationMap::updateAllocation(AggregateType::iterator it, void* ptr, Allocation const& allocation) noexcept
{
	if (it->first == ptr) {
		log(LogLevel::DEBUG,
			"AllocationMap::updateAllocation(%p, {[%lu, %lu], %lu})",
			ptr,
			allocation.pool->getRange().first,
			allocation.pool->getRange().last,
			allocation.size);
		it->second.pool->registerDeallocate();
		allocation.pool->registerAllocate(allocation.size);
		it->second = allocation;
	} else {
		eraseAllocation(it);
		insertAllocation(ptr, allocation);
	}
}

void AllocationMap::dump() const noexcept
{
	for (typename AggregateType::value_type const& allocation : allocations) {
		log("%p: {pool: [%lu, %lu], size: %lu}",
			allocation.first,
			allocation.second.pool->getRange().first,
			allocation.second.pool->getRange().last,
			allocation.second.size);
	}
}

} // namespace ArenaAllocator