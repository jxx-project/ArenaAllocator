//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef CHUNK_H_INCLUDED
#define CHUNK_H_INCLUDED

#include <cstdint>

namespace CustomAllocators {

class Arena;

struct Chunk
{
	void* data;
	Arena* arena;
	std::size_t allocated;
};

} // namespace CustomAllocators

#endif // CHUNK_H_INCLUDED
