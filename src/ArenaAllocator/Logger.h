//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ARENA_ALLOCATOR_LOGGER_H_INCLUDED
#define ARENA_ALLOCATOR_LOGGER_H_INCLUDED

#include "ArenaAllocator/LogLevel.h"

namespace ArenaAllocator {

class Logger
{
public:
	virtual ~Logger() = default;

	virtual void log(char const* fmt, ...) const noexcept = 0;
	virtual void error(char const* fmt, ...) const noexcept = 0;
	virtual void info(char const* fmt, ...) const noexcept = 0;
	virtual void debug(char const* fmt, ...) const noexcept = 0;
	virtual bool isLevel(LogLevel level) const noexcept = 0;
	virtual void setLevel(LogLevel level) noexcept = 0;
};

} // namespace ArenaAllocator

#endif // ARENA_ALLOCATOR_LOGGER_H_INCLUDED
