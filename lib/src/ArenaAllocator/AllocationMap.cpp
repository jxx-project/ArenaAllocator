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
	log(LogLevel::DEBUG, [&] { return Message("AllocationMap::registerAllocate({}, {})", size, result); });
	PoolStatistics* pool{pools.at(size)};
	if (pool == nullptr) {
		pool = &delegatePool;
	}
	insertAllocation(result, {pool, size});
}

void AllocationMap::registerAllocate(std::size_t size, void* result, std::size_t alignment) noexcept
{
	log(LogLevel::DEBUG, [&] { return Message("AllocationMap::registerAllocate({}, {})", size, result); });
	PoolStatistics* pool{alignment <= sizeof(std::max_align_t) ? pools.at(size) : &delegatePool};
	if (pool == nullptr) {
		pool = &delegatePool;
	}
	insertAllocation(result, {pool, size});
}

void AllocationMap::registerDeallocate(void* ptr) noexcept
{
	log(LogLevel::DEBUG, [&] { return Message("AllocationMap::registerDeallocate({})", ptr); });
	if (ptr != nullptr) {
		AggregateType::iterator it{allocations.find(ptr)};
		if (it != allocations.end()) {
			eraseAllocation(it);
		} else {
			log(LogLevel::ERROR, [&] { return Message("AllocationMap::registerDeallocate({}) allocation not found", ptr); });
			if (log.isLevel(LogLevel::DEBUG)) {
				dump();
			}
		}
	}
}

void AllocationMap::registerReallocate(void* ptr, std::size_t size, void* result) noexcept
{
	log(LogLevel::DEBUG, [&] { return Message("AllocationMap::registerReallocate({}, {}, {})", ptr, size, result); });
	if (ptr != nullptr) {
		AggregateType::iterator it{allocations.find(ptr)};
		if (it != allocations.end()) {
			if (size > 0) {
				PoolStatistics* destinationPool{pools.at(size)};
				if (destinationPool == nullptr) {
					destinationPool = &delegatePool;
					if (it->second.pool != &delegatePool) {
						log(LogLevel::ERROR, [&] {
							return Message(
								"AllocationMap::registerReallocate({}, {}, {}) allocation moved out of arena pools",
								ptr,
								size,
								result);
						});
					}
				}
				updateAllocation(it, result, {destinationPool, size});
			} else {
				eraseAllocation(it);
			}
		} else {
			log(LogLevel::ERROR,
				[&] { return Message("AllocationMap::registerReallocate({}, {}, {}): allocation not found", ptr, size, result); });
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
		log(LogLevel::DEBUG, [&] {
			return Message(
				"AllocationMap::insertAllocation({}, {[{}, {}], {}})",
				ptr,
				allocation.pool->getRange().first,
				allocation.pool->getRange().last,
				allocation.size);
		});
		allocation.pool->registerAllocate(allocation.size);
	} else {
		log(LogLevel::ERROR, [&] {
			return Message(
				"AllocationMap::insertAllocation({}, {[{}, {}], {}}): pointer already registered",
				ptr,
				allocation.pool->getRange().first,
				allocation.pool->getRange().last,
				allocation.size);
		});
	}
}

void AllocationMap::eraseAllocation(AggregateType::iterator it) noexcept
{
	log(LogLevel::DEBUG, [&] {
		return Message(
			"AllocationMap::eraseAllocation({}, {[{}, {}], {}})",
			it->first,
			it->second.pool->getRange().first,
			it->second.pool->getRange().last,
			it->second.size);
	});
	it->second.pool->registerDeallocate();
	allocations.erase(it);
}

void AllocationMap::updateAllocation(AggregateType::iterator it, void* ptr, Allocation const& allocation) noexcept
{
	if (it->first == ptr) {
		log(LogLevel::DEBUG, [&] {
			return Message(
				"AllocationMap::updateAllocation({}, {[{}, {}], {}})",
				ptr,
				allocation.pool->getRange().first,
				allocation.pool->getRange().last,
				allocation.size);
		});
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
		log([&] {
			return Message(
				"{}: {pool: [{}, {}], size: {}}",
				allocation.first,
				allocation.second.pool->getRange().first,
				allocation.second.pool->getRange().last,
				allocation.second.size);
		});
	}
}

} // namespace ArenaAllocator
