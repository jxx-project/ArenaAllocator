//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/PoolMap.h"

namespace ArenaAllocator {

PoolMap::PoolMap(Configuration const& configuration, Logger const& logger) noexcept : logger{logger}
{
	SizeRange range{0, 0};
	std::size_t nChunks{0};
	std::size_t chunkSize{0};
	for (Configuration::MapType::value_type const& pool : configuration.getArenas()) {
		if (pool.first > range.last) {
			if (nChunks) {
				insert(range, nChunks);
				nChunks = 0;
				range.first = range.last + 1;
			}
			range.last = ((pool.first + sizeof(Pool::WordType) - 1U) / sizeof(Pool::WordType)) * sizeof(Pool::WordType);
		}
		nChunks += pool.second;
		chunkSize = pool.first;
	}
	if (nChunks) {
		insert(range, nChunks);
	}
}

void PoolMap::insert(SizeRange const& range, std::size_t nChunks) noexcept
{
	pools.emplace(range, range, nChunks, logger);
}

Pool* PoolMap::at(std::size_t chunkSize) noexcept
{
	return pools.at(chunkSize);
}

std::size_t PoolMap::nChunks() const noexcept
{
	std::size_t result{0};
	for (DelegateType::value_type const& pool : pools) {
		result += pool.second.nChunks();
	}
	return result;
}

} // namespace ArenaAllocator
