//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ARENA_ALLOCATOR_OPERATION_TYPE_H_INCLUDED
#define ARENA_ALLOCATOR_OPERATION_TYPE_H_INCLUDED

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

#endif // ARENA_ALLOCATOR_OPERATION_TYPE_H_INCLUDED
