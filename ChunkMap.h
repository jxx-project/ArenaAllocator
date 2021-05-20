#ifndef CHUNK_MAP_H_INCLUDED
#define CHUNK_MAP_H_INCLUDED

#include "ArenaMap.h"
#include "Chunk.h"
#include "Logger.h"
#include "Optional.h"
#include <list>
#include <unordered_map>

class ChunkMap
{
public:
	ChunkMap(ArenaMap const& arenas, Logger const& logger) noexcept;
	Optional<Arena::ListType::const_iterator> find(void* ptr) const noexcept;

private:
	using MapType = std::unordered_map<
		void*,
		Arena::ListType::const_iterator,
		std::hash<void*>,
		std::equal_to<void*>,
		NativeCXXAllocator<std::pair<const void*, Arena::ListType::const_iterator>>>;

	Logger const& logger;
	MapType chunks;
};

#endif // CHUNK_MAP_H_INCLUDED
