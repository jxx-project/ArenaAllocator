#include "ChunkMap.h"

Optional<std::list<Chunk>::const_iterator> ChunkMap::find(void* ptr) const noexcept
{
	Optional<std::list<Chunk>::const_iterator> result;
	std::unordered_map<void*, std::list<Chunk>::const_iterator>::const_iterator it{chunks.find(ptr)};
	if (it != chunks.end()) {
		result.emplace(it->second);
	}
	return result;
}
