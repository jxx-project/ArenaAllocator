//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ConsoleLogger.h"
#include <cstdio>

namespace CustomAllocators {

ConsoleLogger::ConsoleLogger() noexcept : active{true}
{
}

ConsoleLogger::~ConsoleLogger() noexcept
{
}

void ConsoleLogger::log(const char* fmt, ...) const noexcept
{
	if (active) {
		std::va_list argp;
		va_start(argp, fmt);
		write(fmt, argp);
		va_end(argp);
	}
}
bool ConsoleLogger::isActive() const noexcept
{
	return active;
}

void ConsoleLogger::setActive(bool active) noexcept
{
	this->active = active;
}

void ConsoleLogger::write(const char* fmt, std::va_list argp) const noexcept
{
	std::vfprintf(stderr, fmt, argp);
}

} // namespace CustomAllocators
