//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

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

#endif // LOGGER_H_INCLUDED
