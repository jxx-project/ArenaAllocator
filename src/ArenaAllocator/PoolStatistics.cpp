//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/PoolStatistics.h"
#include "ArenaAllocator/Chunk.h"
#include <limits>

namespace ArenaAllocator {

PoolStatistics::PoolStatistics(SizeRange const& range, std::size_t limit, Logger const& log) noexcept :
	range{range}, limit{limit}, allocations{0}, minSize{std::numeric_limits<std::size_t>::max()}, maxSize{0}, hwm{0}, log{log}
{
	log(LogLevel::DEBUG, "PoolStatistics::PoolStatistics([%lu, %lu], %lu)", range.first, range.last, limit);
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
	if (limit && allocations > limit && hwm == limit + 1) {
		log(LogLevel::TRACE,
			"PoolStatistics::registerAllocate(%lu) {range: [%lu, %lu], ...} exceeded limit: %lu",
			size,
			range.first,
			range.last,
			limit);
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
	log("[%lu, %lu]: {limit: %lu, allocations: %lu, minSize: %lu, maxSize: %lu, hwm: %lu}",
		range.first,
		range.last,
		limit,
		allocations,
		hwm ? minSize : 0,
		maxSize,
		hwm);
}

} // namespace ArenaAllocator
