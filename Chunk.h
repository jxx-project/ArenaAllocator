#ifndef CHUNK_H_INCLUDED
#define CHUNK_H_INCLUDED

#include <cstdint>

class Arena;

struct Chunk
{
	void* data;
	Arena* arena;
	std::size_t allocated;
};

#endif // CHUNK_H_INCLUDED
