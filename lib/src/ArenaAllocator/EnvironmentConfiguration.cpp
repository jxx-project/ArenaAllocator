//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/EnvironmentConfiguration.h"
#include "ArenaAllocator/Console.h"
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
		Console::exit([] { return Message("failed to read environment variable {}", configurationEnvVarName); });
	}
	ParseConfiguration{configStr, pools}(className, logLevel, loggerName);
	if ((logger = loggerFactory.getLogger(EnvironmentConfiguration::getLogger())) == nullptr) {
		Console::exit([] { return Message("unexpected logger class in environment variable {}", configurationEnvVarName); });
	}
	logger->setLevel(EnvironmentConfiguration::getLogLevel());
	if ((allocator = allocatorFactory.getAllocator(EnvironmentConfiguration::getClass())) == nullptr) {
		Console::exit([] { return Message("unexpected allocator class in environment variable {}", configurationEnvVarName); });
	}
}

std::string_view const& EnvironmentConfiguration::getClass() const noexcept
{
	if (!className.has_value()) {
		Console::exit([] { return Message("missing allocator class item in environment variable {}", configurationEnvVarName); });
	}
	return className.value();
}

Configuration::PoolMapType const& EnvironmentConfiguration::getPools() const noexcept
{
	if (!pools.has_value()) {
		Console::exit([] { return Message("missing pools item in environment variable {}", configurationEnvVarName); });
	}
	return pools.value();
}

LogLevel const& EnvironmentConfiguration::getLogLevel() const noexcept
{
	if (!logLevel.has_value()) {
		Console::exit([] { return Message("missing log level item in environment variable {}", configurationEnvVarName); });
	}
	return logLevel.value();
}

std::string_view const& EnvironmentConfiguration::getLogger() const noexcept
{
	if (!loggerName.has_value()) {
		Console::exit([] { return Message("missing logger class item in environment variable {}", configurationEnvVarName); });
	}
	return loggerName.value();
}

} // namespace ArenaAllocator
