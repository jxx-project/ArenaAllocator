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
	log(LogLevel::DEBUG,
		[&] { return Format("PoolStatistics::PoolStatistics([{}, {}], {}) -> this:{}", range.first, range.last, limit, this); });
}

PoolStatistics::~PoolStatistics() noexcept
{
	log(LogLevel::DEBUG, [&] { return Format("PoolStatistics::~PoolStatistics(this:{})", this); });
}

void PoolStatistics::registerAllocate(std::size_t size) noexcept
{
	++allocations;
	minSize = std::min(size, minSize);
	maxSize = std::max(size, maxSize);
	hwm = std::max(allocations, hwm);
	if (limit > 0 && allocations > limit && hwm == limit + 1) {
		log(LogLevel::TRACE, [&] {
			return Format(
				"PoolStatistics::registerAllocate({}) {range: [{}, {}], ...} exceeded limit: {}",
				size,
				range.first,
				range.last,
				limit);
		});
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
	log([&] {
		return Format(
			"[{}, {}]: {limit: {}, allocations: {}, minSize: {}, maxSize: {}, hwm: {}}",
			range.first,
			range.last,
			limit,
			allocations,
			hwm > 0 ? minSize : 0,
			maxSize,
			hwm);
	});
}

} // namespace ArenaAllocator
