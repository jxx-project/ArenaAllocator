#include "EnvironmentConfiguration.h"
#include <cstdlib>

EnvironmentConfiguration::EnvironmentConfiguration(Logger& logger) noexcept :
	logger{logger}, arenas{{8, 100}, {16, 100}, {17, 100}, {22, 100}, {31, 100}}
{
}

EnvironmentConfiguration::~EnvironmentConfiguration() noexcept
{
}

Configuration::MapType const& EnvironmentConfiguration::getArenas() const noexcept
{
	return arenas;
}
