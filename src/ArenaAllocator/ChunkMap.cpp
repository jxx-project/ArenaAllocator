//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/ChunkMap.h"

namespace ArenaAllocator {

ChunkMap::ChunkMap(PoolMap const& pools, Logger const& logger) noexcept : logger{logger}
{
	delegate.reserve(pools.nChunks());
	pools.forEachChunk([this](Pool::ListType::const_iterator it) { delegate.insert(DelegateType::value_type(it->data, it)); });
}

Pool::ListType::const_iterator const* ChunkMap::at(void* ptr) const noexcept
{
	Pool::ListType::const_iterator const* result{nullptr};
	DelegateType::const_iterator it{delegate.find(ptr)};
	if (it != delegate.end()) {
		result = &it->second;
	}
	return result;
}

} // namespace ArenaAllocator
