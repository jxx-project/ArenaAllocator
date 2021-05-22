//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "CustomAllocators/ArenaMap.h"
#include <cassert>

namespace CustomAllocators {

ArenaMap::ArenaMap(Configuration const& configuration, Logger const& logger) noexcept : logger{logger}
{
	Arena::Range range{0, 0};
	std::size_t nChunks{0};
	std::size_t chunkSize{0};
	for (Configuration::MapType::value_type const& arena : configuration.getArenas()) {
		if (arena.first > range.last) {
			if (nChunks) {
				insertArena(range, nChunks);
				nChunks = 0;
				range.first = range.last + 1;
			}
			range.last = ((arena.first + sizeof(Arena::WordType) - 1U) / sizeof(Arena::WordType)) * sizeof(Arena::WordType);
		}
		nChunks += arena.second;
		chunkSize = arena.first;
	}
	if (nChunks) {
		insertArena(range, nChunks);
	}
}

void ArenaMap::insertArena(Arena::Range const& range, std::size_t nChunks) noexcept
{
	std::pair<MapType::iterator, bool> emplaceResult{
		arenas.emplace(std::piecewise_construct, std::forward_as_tuple(range), std::forward_as_tuple(range, nChunks, logger))};
	assert(emplaceResult.second);
}

Optional<Arena*> ArenaMap::find(std::size_t chunkSize) noexcept
{
	Optional<Arena*> result;
	MapType::iterator it{arenas.find(Arena::Range{chunkSize, chunkSize})};
	if (it != arenas.end()) {
		result.emplace(&it->second);
	}
	return result;
}

bool ArenaMap::rangeBelow(Arena::Range const& lhs, Arena::Range const& rhs) noexcept
{
	return lhs.last < rhs.first;
}

std::size_t ArenaMap::nChunks() const noexcept
{
	std::size_t result{0};
	for (MapType::value_type const& arena : arenas) {
		result += arena.second.nChunks();
	}
	return result;
}

} // namespace CustomAllocators
