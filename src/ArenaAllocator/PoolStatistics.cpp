//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/PoolStatistics.h"
#include "ArenaAllocator/Chunk.h"
#include <limits>

namespace ArenaAllocator {

PoolStatistics::PoolStatistics(SizeRange const& range, std::size_t nChunks, Logger const& logger) noexcept :
	range{range},
	nChunkLimit{nChunks},
	allocations{0},
	minSize{std::numeric_limits<std::size_t>::max()},
	maxSize{0},
	hwm{0},
	logger{logger}
{
	logger.debug("PoolStatistics::PoolStatistics(SizeRange{%lu, %lu}, %lu)\n", range.first, range.last, nChunks);
}

PoolStatistics::~PoolStatistics() noexcept
{
	logger.info(
		"PoolStatistics::~PoolStatistics() {range: [%lu, %lu], nChunkLimit: %lu, allocations: %lu, minSize: %lu, maxSize: %lu, "
		"hwm: %lu}\n",
		range.first,
		range.last,
		nChunkLimit,
		allocations,
		minSize,
		maxSize,
		hwm);
}

void PoolStatistics::allocate(std::size_t size) noexcept
{
	++allocations;
	minSize = std::min(size, minSize);
	maxSize = std::max(size, maxSize);
	hwm = std::max(allocations, hwm);
	if (allocations > nChunkLimit && hwm == nChunkLimit + 1) {
		logger.info(
			"PoolStatistics::allocate() {range: [%lu, %lu], ...} exceeded nChunkLimit: %lu\n",
			size,
			range.first,
			range.last,
			nChunkLimit);
	}
}

void PoolStatistics::reallocate(std::size_t size) noexcept
{
	maxSize = std::max(size, maxSize);
}

void PoolStatistics::deallocate() noexcept
{
	--allocations;
}

std::size_t PoolStatistics::nChunks() const noexcept
{
	return nChunkLimit;
}

} // namespace ArenaAllocator
