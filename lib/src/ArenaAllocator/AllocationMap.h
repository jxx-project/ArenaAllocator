//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_AllocationMap_h_INCLUDED
#define ArenaAllocator_AllocationMap_h_INCLUDED

#include "ArenaAllocator/Allocation.h"
#include "ArenaAllocator/Logger.h"
#include "ArenaAllocator/PassThroughCXXAllocator.h"
#include "ArenaAllocator/PoolMap.h"
#include "ArenaAllocator/PoolStatistics.h"
#include <mutex>
#include <optional>
#include <unordered_map>

namespace ArenaAllocator {

class AllocationMap
{
public:
	AllocationMap(PoolMap<PoolStatistics>& pools, PoolStatistics& delegatePool, Logger const& log) noexcept;

	void registerAllocate(std::size_t size, void* result) noexcept;
	void registerAllocate(std::size_t size, void* result, std::size_t alignment) noexcept;
	void registerDeallocate(void* ptr) noexcept;
	void registerReallocate(void* ptr, std::size_t size, void* result) noexcept;
	void registerReallocate(void* ptr, std::size_t nmemb, std::size_t size, void* result) noexcept;
	void dump() const noexcept;

private:
	using AggregateType = std::unordered_map<
		void*,
		Allocation,
		std::hash<void*>,
		std::equal_to<void*>,
		PassThroughCXXAllocator<std::pair<void* const, Allocation>>>;

	void insertAllocation(void* ptr, Allocation const& allocation) noexcept;
	void eraseAllocation(AggregateType::iterator it) noexcept;
	void updateAllocation(AggregateType::iterator it, void* ptr, Allocation const& allocation) noexcept;

	Logger const& log;
	PoolMap<PoolStatistics>& pools;
	PoolStatistics& delegatePool;
	AggregateType allocations;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_AllocationMap_h_INCLUDED
