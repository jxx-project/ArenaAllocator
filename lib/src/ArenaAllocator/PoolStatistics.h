//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_PoolStatistics_h_INCLUDED
#define ArenaAllocator_PoolStatistics_h_INCLUDED

#include "ArenaAllocator/Allocator.h"
#include "ArenaAllocator/Chunk.h"
#include "ArenaAllocator/Logger.h"
#include "ArenaAllocator/PassThroughCXXAllocator.h"
#include "ArenaAllocator/SizeRange.h"
#include <cstddef>
#include <list>
#include <vector>

namespace ArenaAllocator {

class PoolStatistics
{
public:
	PoolStatistics(SizeRange const& range, std::size_t limit, Logger const& log) noexcept;
	PoolStatistics(PoolStatistics const& other) = delete;
	PoolStatistics& operator=(PoolStatistics const& other) = delete;
	~PoolStatistics() noexcept;

	void registerAllocate(std::size_t size) noexcept;
	void registerDeallocate() noexcept;
	SizeRange const& getRange() const noexcept;
	void dump() const noexcept;

private:
	const SizeRange range;
	const std::size_t limit;
	std::size_t allocations;
	std::size_t minSize;
	std::size_t maxSize;
	std::size_t hwm;
	Logger const& log;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_PoolStatistics_h_INCLUDED
