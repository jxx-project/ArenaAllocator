//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef CHUNK_MAP_H_INCLUDED
#define CHUNK_MAP_H_INCLUDED

#include "CustomAllocators/ArenaMap.h"
#include "CustomAllocators/Chunk.h"
#include "CustomAllocators/Logger.h"
#include "CustomAllocators/Optional.h"
#include <list>
#include <unordered_map>

namespace CustomAllocators {

class ChunkMap
{
public:
	ChunkMap(ArenaMap const& arenas, Logger const& logger) noexcept;
	Optional<Arena::ListType::const_iterator> find(void* ptr) const noexcept;

private:
	using MapType = std::unordered_map<
		void*,
		Arena::ListType::const_iterator,
		std::hash<void*>,
		std::equal_to<void*>,
		NativeCXXAllocator<std::pair<const void*, Arena::ListType::const_iterator>>>;

	Logger const& logger;
	MapType chunks;
};

} // namespace CustomAllocators

#endif // CHUNK_MAP_H_INCLUDED
