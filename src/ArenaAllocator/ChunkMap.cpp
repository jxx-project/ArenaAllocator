//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/ChunkMap.h"

namespace ArenaAllocator {

ChunkMap::ChunkMap(PoolMap const& pools, Logger const& logger) noexcept : logger{logger}
{
	chunks.reserve(pools.nChunks());
	pools.forEachChunk([this](Pool::ListType::const_iterator it) { chunks.insert(MapType::value_type(it->data, it)); });
}

Optional<Pool::ListType::const_iterator> ChunkMap::find(void* ptr) const noexcept
{
	Optional<Pool::ListType::const_iterator> result;
	MapType::const_iterator it{chunks.find(ptr)};
	if (it != chunks.end()) {
		result.emplace(it->second);
	}
	return result;
}

} // namespace ArenaAllocator
