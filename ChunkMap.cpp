#include "ChunkMap.h"

ChunkMap::ChunkMap(ArenaMap const& arenas, Logger const& logger) noexcept : logger{logger}
{
	chunks.reserve(arenas.nChunks());
	arenas.forEachChunk([this](Arena::ListType::const_iterator it) {
		chunks.insert(std::pair<void* const, Arena::ListType::const_iterator>(it->data, it));
	});
}

Optional<Arena::ListType::const_iterator> ChunkMap::find(void* ptr) const noexcept
{
	Optional<Arena::ListType::const_iterator> result;
	std::unordered_map<void*, Arena::ListType::const_iterator>::const_iterator it{chunks.find(ptr)};
	if (it != chunks.end()) {
		result.emplace(it->second);
	}
	return result;
}
