#include "ArenaMap.h"

Optional<Arena*> ArenaMap::find(std::size_t chunkSize) noexcept
{
	Optional<Arena*> result;
	std::map<Range, Arena, bool (*)(Range const&, Range const&)>::iterator it{arenas.find(Range{chunkSize, chunkSize})};
	if (it != arenas.end()) {
		result.emplace(&it->second);
	}
	return result;
}

bool ArenaMap::Range::below(Range const& lhs, Range const& rhs) noexcept
{
	return lhs.last < rhs.first;
}
