#include "EnvironmentConfiguration.h"
#include <cstdlib>

EnvironmentConfiguration::EnvironmentConfiguration() noexcept : arenas{{8, 1}, {16, 1}, {24, 1}, {32, 1}}
{
}

EnvironmentConfiguration::~EnvironmentConfiguration() noexcept
{
}

Configuration::MapType const& EnvironmentConfiguration::getArenas() const noexcept
{
	return arenas;
}
