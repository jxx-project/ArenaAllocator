//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef POOL_MAP_H_INCLUDED
#define POOL_MAP_H_INCLUDED

#include "ArenaAllocator/Configuration.h"
#include "ArenaAllocator/Logger.h"
#include "ArenaAllocator/NativeCXXAllocator.h"
#include "ArenaAllocator/Optional.h"
#include "ArenaAllocator/Pool.h"
#include <cstdint>
#include <map>

namespace ArenaAllocator {

class PoolMap
{
public:
	PoolMap(Configuration const& configuration, Logger const& logger) noexcept;
	Optional<Pool*> find(std::size_t chunkSize) noexcept;
	std::size_t nChunks() const noexcept;

	template<typename F>
	void forEachChunk(F func) const noexcept
	{
		for (MapType::value_type const& mapEntry : pools) {
			mapEntry.second.forEachChunk(func);
		}
	}

private:
	using MapType = std::map<
		Pool::Range,
		Pool,
		bool (*)(Pool::Range const&, Pool::Range const&),
		NativeCXXAllocator<std::pair<const Pool::Range, Pool>>>;

	static bool rangeBelow(Pool::Range const& lhs, Pool::Range const& rhs) noexcept;
	void insertArena(Pool::Range const& range, std::size_t nChunks) noexcept;

	MapType pools{rangeBelow};
	Logger const& logger;
};

} // namespace ArenaAllocator

#endif // POOL_MAP_H_INCLUDED
