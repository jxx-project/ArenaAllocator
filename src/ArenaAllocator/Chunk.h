//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef CHUNK_H_INCLUDED
#define CHUNK_H_INCLUDED

#include <cstdint>

namespace ArenaAllocator {

class Pool;

struct Chunk
{
	void* data;
	Pool* pool;
	std::size_t allocated;
};

} // namespace ArenaAllocator

#endif // CHUNK_H_INCLUDED
