//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ARENA_ALLOCATOR_LOG_LEVEL_H_INCLUDED
#define ARENA_ALLOCATOR_LOG_LEVEL_H_INCLUDED

namespace ArenaAllocator {

enum class LogLevel
{
	NONE,
	ERROR,
	INFO,
	TRACE,
	DEBUG
};

} // namespace ArenaAllocator

#endif // ARENA_ALLOCATOR_LOG_LEVEL_H_INCLUDED
