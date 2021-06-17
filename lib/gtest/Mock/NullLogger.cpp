//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "Mock/NullLogger.h"

namespace Mock {

bool NullLogger::isLevel(ArenaAllocator::LogLevel) const noexcept
{
	return false;
}

void NullLogger::setLevel(ArenaAllocator::LogLevel) noexcept
{
}

void NullLogger::log(Formatter const&) const noexcept
{
}

void NullLogger::log(std::chrono::nanoseconds, ArenaAllocator::OperationType, Formatter const&) const noexcept
{
}

void NullLogger::log(ArenaAllocator::LogLevel, Formatter const&) const noexcept
{
}

} // namespace Mock
