//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/PoolStatistics.h"
#include "ArenaAllocator/Chunk.h"
#include <limits>

namespace ArenaAllocator {

PoolStatistics::PoolStatistics(SizeRange const& range, std::size_t nChunks, Logger const& log) noexcept :
	range{range},
	nChunkLimit{nChunks},
	allocations{0},
	minSize{std::numeric_limits<std::size_t>::max()},
	maxSize{0},
	hwm{0},
	log{log}
{
	log(LogLevel::DEBUG, "\tPoolStatistics::PoolStatistics([%lu, %lu], %lu)\n", range.first, range.last, nChunks);
}

PoolStatistics::~PoolStatistics() noexcept
{
}

void PoolStatistics::registerAllocate(std::size_t size) noexcept
{
	++allocations;
	minSize = std::min(size, minSize);
	maxSize = std::max(size, maxSize);
	hwm = std::max(allocations, hwm);
	if (nChunkLimit && allocations > nChunkLimit && hwm == nChunkLimit + 1) {
		log(LogLevel::TRACE,
			"\tPoolStatistics::registerAllocate(%lu) {range: [%lu, %lu], ...} exceeded nChunkLimit: %lu\n",
			size,
			range.first,
			range.last,
			nChunkLimit);
	}
}

void PoolStatistics::registerDeallocate() noexcept
{
	--allocations;
}

SizeRange const& PoolStatistics::getRange() const noexcept
{
	return range;
}

void PoolStatistics::dump() const noexcept
{
	log("\t[%lu, %lu]: {nChunkLimit: %lu, allocations: %lu, minSize: %lu, maxSize: %lu, hwm: %lu}\n",
		range.first,
		range.last,
		nChunkLimit,
		allocations,
		hwm ? minSize : 0,
		maxSize,
		hwm);
}

} // namespace ArenaAllocator
