//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/EnvironmentConfiguration.h"
#include "ArenaAllocator/ConsoleLogger.h"
#include "ArenaAllocator/ParseConfiguration.h"

namespace ArenaAllocator {

EnvironmentConfiguration::EnvironmentConfiguration(
	char const* configStr,
	AllocatorFactory& allocatorFactory,
	Allocator*& activeAllocator,
	Logger& log) noexcept :
	activeAllocator{activeAllocator}, log{log}
{
	if (configStr == nullptr) {
		ConsoleLogger::exit("failed to read environment variable %s", configurationEnvVarName);
	}
	ParseConfiguration(configStr, className, pools, logLevel);
	log.setLevel(EnvironmentConfiguration::getLogLevel());
	if ((activeAllocator = allocatorFactory.getAllocator(EnvironmentConfiguration::getClass())) == nullptr) {
		ConsoleLogger::exit("unexpected allocator class in environment variable %s", configurationEnvVarName);
	}
}

EnvironmentConfiguration::~EnvironmentConfiguration() noexcept
{
}

std::string_view const& EnvironmentConfiguration::getClass() const noexcept
{
	if (!className.has_value()) {
		ConsoleLogger::exit("missing allocator class item in environment variable %s", configurationEnvVarName);
	}
	return className.value();
}

Configuration::PoolMapType const& EnvironmentConfiguration::getPools() const noexcept
{
	if (!pools.has_value()) {
		ConsoleLogger::exit("missing pools item in environment variable %s", configurationEnvVarName);
	}
	return pools.value();
}

LogLevel const& EnvironmentConfiguration::getLogLevel() const noexcept
{
	if (!logLevel.has_value()) {
		ConsoleLogger::exit("missing log level item in environment variable %s", configurationEnvVarName);
	}
	return logLevel.value();
}

} // namespace ArenaAllocator
