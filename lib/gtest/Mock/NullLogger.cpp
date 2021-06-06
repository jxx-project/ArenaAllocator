//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "Mock/NullLogger.h"

namespace Mock {

NullLogger::NullLogger() noexcept
{
}

NullLogger::~NullLogger() noexcept
{
}

void NullLogger::operator()(char const*, ...) const noexcept
{
}

void NullLogger::operator()(std::chrono::nanoseconds, ArenaAllocator::OperationType, char const*, ...) const noexcept
{
}

void NullLogger::operator()(ArenaAllocator::LogLevel level, char const* fmt, ...) const noexcept
{
}

bool NullLogger::isLevel(ArenaAllocator::LogLevel) const noexcept
{
	return false;
}

void NullLogger::setLevel(ArenaAllocator::LogLevel) noexcept
{
}

} // namespace Mock
