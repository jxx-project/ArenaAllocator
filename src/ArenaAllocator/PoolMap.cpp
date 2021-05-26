//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/PoolMap.h"

namespace ArenaAllocator {

PoolMap::PoolMap(Configuration const& configuration, Logger const& logger) noexcept : logger{logger}
{
	for (Configuration::PoolMapType::value_type const& pool : configuration.getPools()) {
		insert(pool.first, pool.second);
	}
}

void PoolMap::insert(SizeRange const& range, std::size_t nChunks) noexcept
{
	if (!pools.emplace(range, range, nChunks, logger)) {
		logger.error("PoolMap::insert(Range{%ul, %ul}, %ul) discarded due to range overlap", range.first, range.last, nChunks);
	}
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
