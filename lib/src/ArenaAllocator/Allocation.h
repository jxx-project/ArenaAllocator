//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_Allocation_h_INCLUDED
#define ArenaAllocator_Allocation_h_INCLUDED

#include "ArenaAllocator/PoolStatistics.h"
#include <cstddef>

namespace ArenaAllocator {

struct Allocation
{
	PoolStatistics* pool;
	std::size_t size;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_Allocation_h_INCLUDED
