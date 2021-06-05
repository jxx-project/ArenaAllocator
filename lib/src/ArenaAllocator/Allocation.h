//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ARENA_ALLOCATOR_ALLOCATION_H_INCLUDED
#define ARENA_ALLOCATOR_ALLOCATION_H_INCLUDED

#include "ArenaAllocator/PoolStatistics.h"
#include <cstddef>

namespace ArenaAllocator {

struct Allocation
{
	PoolStatistics* pool;
	std::size_t size;
};

} // namespace ArenaAllocator

#endif // ARENA_ALLOCATOR_ALLOCATION_H_INCLUDED
