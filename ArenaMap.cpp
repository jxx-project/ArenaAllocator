#include "ArenaMap.h"

ArenaMap::ArenaMap(Configuration const& configuration) noexcept : configuration{configuration}
{
	Range range{0, 0};
	std::size_t nChunks{0};
	std::size_t chunkSize{0};
	for (std::pair<const std::size_t, std::size_t> const& arena : configuration.getArenas()) {
		chunkSize = arena.first;
		if (chunkSize > range.last) {
			if (nChunks) {
				arenas.emplace(std::pair<Range, Arena>(
					std::piecewise_construct, std::forward_as_tuple(range), std::forward_as_tuple(nChunks, chunkSize)));
				nChunks = 0;
			}
			range.first = range.last + 1;
			range.last = (chunkSize + sizeof(Arena::WordType) - 1U) / sizeof(Arena::WordType);
		}
		nChunks += arena.second;
	}
	if (nChunks) {
		arenas.emplace(std::pair<Range, Arena>(
			std::piecewise_construct, std::forward_as_tuple(range), std::forward_as_tuple(nChunks, chunkSize)));
	}
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
