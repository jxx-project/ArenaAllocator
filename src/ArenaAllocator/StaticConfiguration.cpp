//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/StaticConfiguration.h"
#include <cstdlib>

namespace ArenaAllocator {

StaticConfiguration::StaticConfiguration(Allocator*& activeAllocator, Logger& logger) noexcept :
	activeAllocator{activeAllocator}, logger{logger}
{
	pools.emplace(SizeRange{72704, 72704}, 1);
	for (std::size_t size = 8; size <= 1000; size += 8) {
		pools.emplace(SizeRange{size - 7, size}, 200);
	}
}

StaticConfiguration::~StaticConfiguration() noexcept
{
}

Configuration::PoolMapType const& StaticConfiguration::getPools() const noexcept
{
	return pools;
}

} // namespace ArenaAllocator
