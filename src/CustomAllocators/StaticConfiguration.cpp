//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "CustomAllocators/StaticConfiguration.h"
#include <cstdlib>

namespace CustomAllocators {

StaticConfiguration::StaticConfiguration(Allocator*& activeAllocator, Logger& logger) noexcept :
	activeAllocator{activeAllocator}, logger{logger}, arenas{{72704, 1}}
{
	for (std::size_t size = 8; size <= 1000; size += 8) {
		arenas.insert(std::pair<const std::size_t, std::size_t>(size, 200));
	}
}

StaticConfiguration::~StaticConfiguration() noexcept
{
}

Configuration::MapType const& StaticConfiguration::getArenas() const noexcept
{
	return arenas;
}

} // namespace CustomAllocators
