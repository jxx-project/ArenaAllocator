#include "EnvironmentConfiguration.h"
#include <cstdlib>

EnvironmentConfiguration::EnvironmentConfiguration(Logger& logger) noexcept : logger{logger}, arenas{{8, 1}, {16, 1}, {17, 1}, {22, 1}, {31, 1}}
{
}

EnvironmentConfiguration::~EnvironmentConfiguration() noexcept
{
}

Configuration::MapType const& EnvironmentConfiguration::getArenas() const noexcept
{
	return arenas;
}
