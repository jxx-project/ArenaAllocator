//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_OperationType_h_INCLUDED
#define ArenaAllocator_OperationType_h_INCLUDED

namespace ArenaAllocator {

enum class OperationType
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
	PVALLOC
};

char const* to_string(OperationType);

} // namespace ArenaAllocator

#endif // ArenaAllocator_OperationType_h_INCLUDED
