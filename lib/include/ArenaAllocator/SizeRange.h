//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_SizeRange_h_INCLUDED
#define ArenaAllocator_SizeRange_h_INCLUDED

#include <cstddef>

namespace ArenaAllocator {

struct SizeRange
{
	std::size_t first;
	std::size_t last;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_SizeRange_h_INCLUDED
