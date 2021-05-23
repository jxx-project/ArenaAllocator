//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ARENA_ALLOCATOR_LOGGER_H_INCLUDED
#define ARENA_ALLOCATOR_LOGGER_H_INCLUDED

namespace ArenaAllocator {

class Logger
{
public:
	virtual ~Logger() = default;
	virtual void log(const char* fmt, ...) const noexcept = 0;
	virtual bool isActive() const noexcept = 0;
	virtual void setActive(bool active) noexcept = 0;
};

} // namespace ArenaAllocator

#endif // ARENA_ALLOCATOR_LOGGER_H_INCLUDED
