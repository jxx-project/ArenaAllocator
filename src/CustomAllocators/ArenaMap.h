//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ARENA_MAP_H_INCLUDED
#define ARENA_MAP_H_INCLUDED

#include "Arena.h"
#include "Configuration.h"
#include "Logger.h"
#include "NativeCXXAllocator.h"
#include "Optional.h"
#include <cstdint>
#include <map>

namespace CustomAllocators {

class ArenaMap
{
public:
	ArenaMap(Configuration const& configuration, Logger const& logger) noexcept;
	Optional<Arena*> find(std::size_t chunkSize) noexcept;
	std::size_t nChunks() const noexcept;

	template<typename F>
	void forEachChunk(F func) const noexcept
	{
		for (std::pair<const ArenaMap::Range, Arena> const& mapEntry : arenas) {
			mapEntry.second.forEachChunk(func);
		}
	}

private:
	struct Range
	{
		std::size_t first;
		std::size_t last;
		static bool below(Range const& lhs, Range const& rhs) noexcept;
	};

	using MapType = std::map<Range, Arena, bool (*)(Range const&, Range const&), NativeCXXAllocator<std::pair<const Range, Arena>>>;

	void insertArena(Range const& range, std::size_t nChunks, std::size_t chunkSize) noexcept;

	Logger const& logger;
	MapType arenas{Range::below};
};

} // namespace CustomAllocators

#endif // ARENA_MAP_H_INCLUDED
