#include "ArenaMap.h"

ArenaMap::ArenaMap(Configuration const& configuration, Logger const& logger) noexcept : logger{logger}
{
	logger.log(
		"ArenaMap::ArenaMap(Configuration const&, Logger const&): configuration.getArenas().size() = %ld\n",
		configuration.getArenas().size());
	Range range{0, 0};
	std::size_t nChunks{0};
	std::size_t chunkSize{0};
	for (std::pair<const std::size_t, std::size_t> const& arena : configuration.getArenas()) {
		if (arena.first > range.last) {
			if (nChunks) {
				insertArena(range, nChunks, chunkSize);
				nChunks = 0;
				range.first = range.last + 1;
			}
			range.last = ((arena.first + sizeof(Arena::WordType) - 1U) / sizeof(Arena::WordType)) * sizeof(Arena::WordType);
		}
		nChunks += arena.second;
		chunkSize = arena.first;
	}
	if (nChunks) {
		insertArena(range, nChunks, chunkSize);
	}
}

void ArenaMap::insertArena(Range const& range, std::size_t nChunks, std::size_t chunkSize) noexcept
{
	logger.log("ArenaMap::insertArena:(Range{%ld, %ld}, %ld, %ld)\n", range.first, range.last, nChunks, chunkSize);
	arenas.emplace(
		std::pair<Range, Arena>(std::piecewise_construct, std::forward_as_tuple(range), std::forward_as_tuple(nChunks, chunkSize)));
}

Optional<Arena*> ArenaMap::find(std::size_t chunkSize) noexcept
{
	Optional<Arena*> result;
	MapType::iterator it{arenas.find(Range{chunkSize, chunkSize})};
	if (it != arenas.end()) {
		result.emplace(&it->second);
	}
	return result;
}

bool ArenaMap::Range::below(Range const& lhs, Range const& rhs) noexcept
{
	return lhs.last < rhs.first;
}

std::size_t ArenaMap::nChunks() const noexcept
{
	std::size_t result{0};
	for (std::pair<const Range, Arena> const& arena : arenas) {
		result += arena.second.nChunks();
	}
	return result;
}
