//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_OperationType_h_INCLUDED
#define ArenaAllocator_OperationType_h_INCLUDED

#include <string_view>

namespace ArenaAllocator {

enum class OperationType : unsigned
{
	MALLOC,
	FREE,
	CALLOC,
	REALLOC,
	REALLOCARRAY,
	POSIX_MEMALIGN,
	ALIGNED_ALLOC,
	VALLOC,
	MEMALIGN,
	PVALLOC,
	UNKNOWN // Last entry, index indicating number of types
};

std::string_view to_string(OperationType);

} // namespace ArenaAllocator

#endif // ArenaAllocator_OperationType_h_INCLUDED
