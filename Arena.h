#ifndef ARENA_H_INCLUDED
#define ARENA_H_INCLUDED

#include "Chunk.h"
#include "NativeCXXAllocator.h"
#include <cstdint>
#include <list>
#include <vector>

class Arena
{
public:
	using WordType = long;
	using ListType = std::list<Chunk, NativeCXXAllocator<Chunk>>;

	Arena(std::size_t nChunks, std::size_t chunkSize) noexcept;
	void* allocate() noexcept;
	void deallocate(std::list<Chunk>::const_iterator it) noexcept;
	std::size_t nChunks() const noexcept;

private:
	using StorageType = std::vector<WordType, NativeCXXAllocator<WordType>>;
	StorageType storage;
	ListType free;
	ListType allocated;
};

#endif // ARENA_H_INCLUDED
