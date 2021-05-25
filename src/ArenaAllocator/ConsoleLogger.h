//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ARENA_ALLOCATOR_CONSOLE_LOGGER_H_INCLUDED
#define ARENA_ALLOCATOR_CONSOLE_LOGGER_H_INCLUDED

#include "ArenaAllocator/Logger.h"
#include <cstdarg>

namespace ArenaAllocator {

class ConsoleLogger : public Logger
{
public:
	ConsoleLogger() noexcept;
	ConsoleLogger(Logger const&) = delete;
	ConsoleLogger& operator=(ConsoleLogger const&) = delete;
	virtual ~ConsoleLogger() noexcept;
	virtual void log(char const* fmt, ...) const noexcept override;
	virtual bool isActive() const noexcept override;
	virtual void setActive(bool active) noexcept override;

private:
	void write(const char* fmt, std::va_list argp) const noexcept;
	bool active;
};

} // namespace ArenaAllocator

#endif // ARENA_ALLOCATOR_CONSOLE_LOGGER_H_INCLUDED
