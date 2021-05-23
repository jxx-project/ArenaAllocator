//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ARENA_ALLOCATOR_STATIC_CONFIGURATION_H_INCLUDED
#define ARENA_ALLOCATOR_STATIC_CONFIGURATION_H_INCLUDED

#include "ArenaAllocator/Allocator.h"
#include "ArenaAllocator/Configuration.h"
#include "ArenaAllocator/Logger.h"

namespace ArenaAllocator {

class StaticConfiguration : public Configuration
{
public:
	StaticConfiguration(Allocator*& activeAllocator, Logger& logger) noexcept;
	StaticConfiguration(StaticConfiguration const&) = delete;
	StaticConfiguration& operator=(StaticConfiguration const&) = delete;
	~StaticConfiguration() noexcept;
	virtual Configuration::MapType const& getArenas() const noexcept override;

private:
	Allocator*& activeAllocator;
	Logger& logger;
	Configuration::MapType pools;
};

} // namespace ArenaAllocator

#endif // ARENA_ALLOCATOR_STATIC_CONFIGURATION_H_INCLUDED
