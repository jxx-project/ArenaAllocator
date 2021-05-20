#include "ChunkMap.h"

Optional<Arena::ListType::const_iterator> ChunkMap::find(void* ptr) const noexcept
{
	Optional<Arena::ListType::const_iterator> result;
	std::unordered_map<void*, Arena::ListType::const_iterator>::const_iterator it{chunks.find(ptr)};
	if (it != chunks.end()) {
		result.emplace(it->second);
	}
	return result;
}
