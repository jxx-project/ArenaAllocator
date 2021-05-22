//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ARENA_MAP_H_INCLUDED
#define ARENA_MAP_H_INCLUDED

#include "CustomAllocators/Arena.h"
#include "CustomAllocators/Configuration.h"
#include "CustomAllocators/Logger.h"
#include "CustomAllocators/NativeCXXAllocator.h"
#include "CustomAllocators/Optional.h"
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
		for (MapType::value_type const& mapEntry : arenas) {
			mapEntry.second.forEachChunk(func);
		}
	}

private:
	using MapType = std::map<
		Arena::Range,
		Arena,
		bool (*)(Arena::Range const&, Arena::Range const&),
		NativeCXXAllocator<std::pair<const Arena::Range, Arena>>>;

	static bool rangeBelow(Arena::Range const& lhs, Arena::Range const& rhs) noexcept;
	void insertArena(Arena::Range const& range, std::size_t nChunks) noexcept;

	MapType arenas{rangeBelow};
	Logger const& logger;
};

} // namespace CustomAllocators

#endif // ARENA_MAP_H_INCLUDED
