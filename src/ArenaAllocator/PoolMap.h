//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ARENA_ALLOCATOR_POOL_MAP_H_INCLUDED
#define ARENA_ALLOCATOR_POOL_MAP_H_INCLUDED

#include "ArenaAllocator/Configuration.h"
#include "ArenaAllocator/Logger.h"
#include "ArenaAllocator/PassThroughCXXAllocator.h"
#include "ArenaAllocator/Pool.h"
#include "ArenaAllocator/SizeRangeMap.h"

namespace ArenaAllocator {

class PoolMap
{
public:
	PoolMap(Configuration const& configuration, Logger const& logger) noexcept;

	Pool* at(std::size_t chunkSize) noexcept;
	std::size_t nChunks() const noexcept;

	template<typename F>
	void forEachChunk(F func) const noexcept
	{
		for (DelegateType::value_type const& mapEntry : pools) {
			mapEntry.second.forEachChunk(func);
		}
	}

private:
	using DelegateType = SizeRangeMap<Pool>;

	void insert(SizeRange const& range, std::size_t nChunks) noexcept;

	DelegateType pools;
	Logger const& logger;
};

} // namespace ArenaAllocator

#endif // ARENA_ALLOCATOR_POOL_MAP_H_INCLUDED
