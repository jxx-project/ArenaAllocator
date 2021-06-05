//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ARENA_ALLOCATOR_SIZE_RANGE_H_INCLUDED
#define ARENA_ALLOCATOR_SIZE_RANGE_H_INCLUDED

#include <cstddef>

struct SizeRange
{
	std::size_t first;
	std::size_t last;
};

#endif // ARENA_ALLOCATOR_SIZE_RANGE_H_INCLUDED
