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
	Allocator*& allocator,
	LoggerFactory& loggerFactory,
	Logger*& logger) noexcept :
	allocator{allocator}, logger{logger}
{
	if (configStr == nullptr) {
		ConsoleLogger::exit("failed to read environment variable %s", configurationEnvVarName);
	}
	ParseConfiguration(configStr, className, pools, logLevel, loggerName);
	if ((logger = loggerFactory.getLogger(EnvironmentConfiguration::getLogger())) == nullptr) {
		ConsoleLogger::exit("unexpected logger class in environment variable %s", configurationEnvVarName);
	}
	logger->setLevel(EnvironmentConfiguration::getLogLevel());
	if ((allocator = allocatorFactory.getAllocator(EnvironmentConfiguration::getClass())) == nullptr) {
		ConsoleLogger::exit("unexpected allocator class in environment variable %s", configurationEnvVarName);
	}
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

std::string_view const& EnvironmentConfiguration::getLogger() const noexcept
{
	if (!loggerName.has_value()) {
		ConsoleLogger::exit("missing logger class item in environment variable %s", configurationEnvVarName);
	}
	return loggerName.value();
}

} // namespace ArenaAllocator
