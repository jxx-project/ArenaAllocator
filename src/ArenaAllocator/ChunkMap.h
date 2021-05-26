//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ARENA_ALLOCATOR_CHUNK_MAP_H_INCLUDED
#define ARENA_ALLOCATOR_CHUNK_MAP_H_INCLUDED

#include "ArenaAllocator/Chunk.h"
#include "ArenaAllocator/Logger.h"
#include "ArenaAllocator/PoolMap.h"
#include <list>
#include <unordered_map>

namespace ArenaAllocator {

class ChunkMap
{
public:
	ChunkMap(PoolMap const& pools, Logger const& logger) noexcept;

	Pool::ListType::const_iterator const* at(void* ptr) const noexcept;

private:
	using DelegateType = std::unordered_map<
		void*,
		Pool::ListType::const_iterator,
		std::hash<void*>,
		std::equal_to<void*>,
		PassThroughCXXAllocator<std::pair<void const*, Pool::ListType::const_iterator>>>;

	Logger const& logger;
	DelegateType delegate;
};

} // namespace ArenaAllocator

#endif // ARENA_ALLOCATOR_CHUNK_MAP_H_INCLUDED
