//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/PoolMap.h"
#include "ArenaAllocator/Pool.h"
#include "ArenaAllocator/PoolStatistics.h"

namespace ArenaAllocator {

template<typename T>
PoolMap<T>::PoolMap(Configuration const& configuration, Logger const& logger) noexcept : logger{logger}
{
	for (Configuration::PoolMapType::value_type const& poolConfiguration : configuration.getPools()) {
		insert(poolConfiguration.first, poolConfiguration.second);
	}
}

template<typename T>
void PoolMap<T>::insert(SizeRange const& range, std::size_t nChunks) noexcept
{
	if (!aggregate.emplace(range, range, nChunks, logger)) {
		logger.error("PoolMap::insert(Range{%lu, %lu}, %lu) discarded due to range overlap", range.first, range.last, nChunks);
	}
}

template<typename T>
T* PoolMap<T>::at(std::size_t chunkSize) noexcept
{
	return aggregate.at(chunkSize);
}

template<typename T>
std::size_t PoolMap<T>::nChunks() const noexcept
{
	std::size_t result{0};
	for (typename AggregateType::value_type const& pool : aggregate) {
		result += pool.second.nChunks();
	}
	return result;
}

template class PoolMap<Pool>;
template class PoolMap<PoolStatistics>;

} // namespace ArenaAllocator
