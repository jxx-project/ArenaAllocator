#ifndef ARENA_H_INCLUDED
#define ARENA_H_INCLUDED

#include "Chunk.h"
#include <cstdint>
#include <list>
#include <vector>

class Arena
{
public:
	using WordType = long;
	Arena(std::size_t nChunks, std::size_t chunkSize) noexcept;
	void* allocate() noexcept;
	void deallocate(std::list<Chunk>::const_iterator it) noexcept;

private:
	std::vector<WordType> storage;
	std::list<Chunk> free;
	std::list<Chunk> allocated;
};

#endif // ARENA_H_INCLUDED
