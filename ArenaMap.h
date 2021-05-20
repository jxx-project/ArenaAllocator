#ifndef ARENA_MAP_H_INCLUDED
#define ARENA_MAP_H_INCLUDED

#include "Arena.h"
#include "Configuration.h"
#include "NativeCXXAllocator.h"
#include "Optional.h"
#include <cstdint>
#include <map>

class ArenaMap
{
public:
	ArenaMap(Configuration const& configuration) noexcept;
	Optional<Arena*> find(std::size_t chunkSize) noexcept;
	std::size_t nChunks() const noexcept;

private:
	struct Range
	{
		std::size_t first;
		std::size_t last;
		static bool below(Range const& lhs, Range const& rhs) noexcept;
	};

	using MapType = std::map<Range, Arena, bool (*)(Range const&, Range const&), NativeCXXAllocator<std::pair<const Range, Arena>>>;

	Configuration const& configuration;
	MapType arenas{Range::below};
};

#endif // ARENA_MAP_H_INCLUDED
