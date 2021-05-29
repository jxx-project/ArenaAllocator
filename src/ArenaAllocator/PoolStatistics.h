//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ARENA_ALLOCATOR_POOL_STATISTICS_H_INCLUDED
#define ARENA_ALLOCATOR_POOL_STATISTICS_H_INCLUDED

#include "ArenaAllocator/Allocator.h"
#include "ArenaAllocator/Chunk.h"
#include "ArenaAllocator/Logger.h"
#include "ArenaAllocator/PassThroughCXXAllocator.h"
#include "ArenaAllocator/SizeRange.h"
#include <cstdint>
#include <list>
#include <mutex>
#include <vector>

namespace ArenaAllocator {

class PoolStatistics
{
public:
	PoolStatistics(SizeRange const& range, std::size_t nChunks, Logger const& logger) noexcept;
	PoolStatistics(PoolStatistics const& other) = delete;
	PoolStatistics& operator=(PoolStatistics const& other) = delete;
	~PoolStatistics() noexcept;

	void allocate(std::size_t size) noexcept;
	void reallocate(std::size_t size) noexcept;
	void deallocate() noexcept;
	std::size_t nChunks() const noexcept;

private:
	const SizeRange range;
	const std::size_t nChunkLimit;
	std::size_t allocations;
	std::size_t minSize;
	std::size_t maxSize;
	std::size_t hwm;
	Logger const& logger;
};

} // namespace ArenaAllocator

#endif // ARENA_ALLOCATOR_POOL_STATISTICS_H_INCLUDED
