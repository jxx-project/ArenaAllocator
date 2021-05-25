//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ARENA_ALLOCATOR_CHUNK_H_INCLUDED
#define ARENA_ALLOCATOR_CHUNK_H_INCLUDED

#include <cstdint>

namespace ArenaAllocator {

class Pool;

struct Chunk
{
	void* data;
	Pool* pool;
	std::size_t allocatedSize;
};

} // namespace ArenaAllocator

#endif // ARENA_ALLOCATOR_CHUNK_H_INCLUDED
