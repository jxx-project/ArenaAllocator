#include "ArenaMap.h"

Arena* ArenaMap::find(std::size_t chunkSize) noexcept
{
	std::map<Range, Arena, bool (*)(Range const&, Range const&)>::iterator result{arenas.find(Range{chunkSize, chunkSize})};
	return (result == arenas.end() ? nullptr : &(result->second));
}

bool ArenaMap::Range::below(Range const& lhs, Range const& rhs) noexcept
{
	return lhs.last < rhs.first;
}
