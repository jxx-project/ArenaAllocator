#ifndef CHUNK_MAP_H_INCLUDED
#define CHUNK_MAP_H_INCLUDED

#include "Arena.h"
#include "Chunk.h"
#include "Optional.h"
#include <list>
#include <unordered_map>

class ChunkMap
{
public:
	Optional<Arena::ListType::const_iterator> find(void* ptr) const noexcept;

private:
	std::unordered_map<void*, Arena::ListType::const_iterator> chunks;
};

#endif // CHUNK_MAP_H_INCLUDED
