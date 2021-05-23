//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef CHUNK_MAP_H_INCLUDED
#define CHUNK_MAP_H_INCLUDED

#include "ArenaAllocator/Chunk.h"
#include "ArenaAllocator/Logger.h"
#include "ArenaAllocator/Optional.h"
#include "ArenaAllocator/PoolMap.h"
#include <list>
#include <unordered_map>

namespace ArenaAllocator {

class ChunkMap
{
public:
	ChunkMap(PoolMap const& pools, Logger const& logger) noexcept;
	Optional<Pool::ListType::const_iterator> find(void* ptr) const noexcept;

private:
	using MapType = std::unordered_map<
		void*,
		Pool::ListType::const_iterator,
		std::hash<void*>,
		std::equal_to<void*>,
		NativeCXXAllocator<std::pair<const void*, Pool::ListType::const_iterator>>>;

	Logger const& logger;
	MapType chunks;
};

} // namespace ArenaAllocator

#endif // CHUNK_MAP_H_INCLUDED
