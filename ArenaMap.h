#ifndef ARENA_MAP_H_INCLUDED
#define ARENA_MAP_H_INCLUDED

#include "Arena.h"
#include <cstdint>
#include <map>

class ArenaMap
{
public:
	Arena* find(std::size_t chunkSize) noexcept;

private:
	struct Range
	{
		std::size_t first;
		std::size_t last;
		static bool below(Range const& lhs, Range const& rhs) noexcept;
	};
	std::map<Range, Arena, bool (*)(Range const&, Range const&)> arenas{Range::below};
};

#endif // ARENA_MAP_H_INCLUDED
