#include "StaticConfiguration.h"
#include <cstdlib>

StaticConfiguration::StaticConfiguration(Allocator*& activeAllocator, Logger& logger) noexcept :
	activeAllocator{activeAllocator}, logger{logger}
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
