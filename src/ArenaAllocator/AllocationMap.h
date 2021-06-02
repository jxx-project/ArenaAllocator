//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ARENA_ALLOCATOR_ALLOCATION_MAP_H_INCLUDED
#define ARENA_ALLOCATOR_ALLOCATION_MAP_H_INCLUDED

#include "ArenaAllocator/Allocation.h"
#include "ArenaAllocator/Logger.h"
#include "ArenaAllocator/PassThroughCXXAllocator.h"
#include <optional>
#include <unordered_map>

namespace ArenaAllocator {

class AllocationMap
{
public:
	using AggregateType = std::unordered_map<
		void*,
		Allocation,
		std::hash<void*>,
		std::equal_to<void*>,
		PassThroughCXXAllocator<std::pair<void* const, Allocation>>>;
	using value_type = AggregateType::value_type;
	using iterator = AggregateType::iterator;
	using const_iterator = AggregateType::const_iterator;

	AllocationMap(Logger const& logger) noexcept;

	std::optional<const_iterator> find(void* ptr) const noexcept;
	void registerAllocation(void* ptr, Allocation const& allocation) noexcept;
	void unregisterAllocation(AggregateType::const_iterator it) noexcept;
	void dump() const noexcept;

private:
	Logger const& logger;
	AggregateType aggregate;
};

} // namespace ArenaAllocator

#endif // ARENA_ALLOCATOR_ALLOCATION_MAP_H_INCLUDED
