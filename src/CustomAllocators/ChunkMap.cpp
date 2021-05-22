//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "CustomAllocators/ChunkMap.h"

namespace CustomAllocators {

ChunkMap::ChunkMap(ArenaMap const& arenas, Logger const& logger) noexcept : logger{logger}
{
	chunks.reserve(arenas.nChunks());
	arenas.forEachChunk([this](Arena::ListType::const_iterator it) { chunks.insert(MapType::value_type(it->data, it)); });
}

Optional<Arena::ListType::const_iterator> ChunkMap::find(void* ptr) const noexcept
{
	Optional<Arena::ListType::const_iterator> result;
	MapType::const_iterator it{chunks.find(ptr)};
	if (it != chunks.end()) {
		result.emplace(it->second);
	}
	return result;
}

} // namespace CustomAllocators
