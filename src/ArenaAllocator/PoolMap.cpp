//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/PoolMap.h"
#include <cassert>

namespace ArenaAllocator {

PoolMap::PoolMap(Configuration const& configuration, Logger const& logger) noexcept : logger{logger}
{
	Pool::Range range{0, 0};
	std::size_t nChunks{0};
	std::size_t chunkSize{0};
	for (Configuration::MapType::value_type const& pool : configuration.getArenas()) {
		if (pool.first > range.last) {
			if (nChunks) {
				insertArena(range, nChunks);
				nChunks = 0;
				range.first = range.last + 1;
			}
			range.last = ((pool.first + sizeof(Pool::WordType) - 1U) / sizeof(Pool::WordType)) * sizeof(Pool::WordType);
		}
		nChunks += pool.second;
		chunkSize = pool.first;
	}
	if (nChunks) {
		insertArena(range, nChunks);
	}
}

void PoolMap::insertArena(Pool::Range const& range, std::size_t nChunks) noexcept
{
	std::pair<MapType::iterator, bool> emplaceResult{
		pools.emplace(std::piecewise_construct, std::forward_as_tuple(range), std::forward_as_tuple(range, nChunks, logger))};
	assert(emplaceResult.second);
}

Optional<Pool*> PoolMap::find(std::size_t chunkSize) noexcept
{
	Optional<Pool*> result;
	MapType::iterator it{pools.find(Pool::Range{chunkSize, chunkSize})};
	if (it != pools.end()) {
		result.emplace(&it->second);
	}
	return result;
}

bool PoolMap::rangeBelow(Pool::Range const& lhs, Pool::Range const& rhs) noexcept
{
	return lhs.last < rhs.first;
}

std::size_t PoolMap::nChunks() const noexcept
{
	std::size_t result{0};
	for (MapType::value_type const& pool : pools) {
		result += pool.second.nChunks();
	}
	return result;
}

} // namespace ArenaAllocator
