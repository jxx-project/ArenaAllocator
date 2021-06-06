//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_Chunk_h_INCLUDED
#define ArenaAllocator_Chunk_h_INCLUDED

#include <cstddef>

namespace ArenaAllocator {

class Pool;

struct Chunk
{
	void* data;
	Pool* pool;
	std::size_t allocatedSize;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_Chunk_h_INCLUDED
