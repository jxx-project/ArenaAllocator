//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/ChunkMap.h"

namespace ArenaAllocator {

ChunkMap::ChunkMap(PoolMap<Pool>& pools, Logger const& logger) noexcept : logger{logger}
{
	aggregate.reserve(pools.nChunks());
	pools.forEachChunk([this](Pool::ListType::iterator it) { aggregate.insert(AggregateType::value_type(it->data, it)); });
}

Pool::ListType::iterator const* ChunkMap::at(void* ptr) const noexcept
{
	Pool::ListType::iterator const* result{nullptr};
	AggregateType::const_iterator it{aggregate.find(ptr)};
	if (it != aggregate.end()) {
		result = &it->second;
	}
	return result;
}

} // namespace ArenaAllocator
