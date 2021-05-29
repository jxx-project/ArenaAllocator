//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/EnvironmentConfiguration.h"
#include "ArenaAllocator/ParseConfiguration.h"
#include <cstdlib>

namespace ArenaAllocator {

EnvironmentConfiguration::EnvironmentConfiguration(
	AllocatorFactory& allocatorFactory,
	Allocator*& activeAllocator,
	Logger& logger) noexcept :
	activeAllocator{activeAllocator}, logger{logger}
{
	char* env{std::getenv("ARENA_ALLOCATOR_CONFIGURATION")};
	if (!env) {
		fprintf(stderr, "EnvironmentConfiguration: Failed to read environment variable ARENA_ALLOCATOR_CONFIGURATION\n");
		std::abort();
	}
	ParseConfiguration(env, className, pools, logLevel);
	logger.setLevel(getLogLevel());
	if (!(activeAllocator = allocatorFactory.getAllocator(getClass()))) {
		fprintf(stderr, "EnvironmentConfiguration: Unexpected allocator class\n");
		std::abort();
	}
}

EnvironmentConfiguration::~EnvironmentConfiguration() noexcept
{
}

Configuration::StringType const& EnvironmentConfiguration::getClass() const noexcept
{
	if (!className.has_value()) {
		fprintf(
			stderr,
			"EnvironmentConfiguration: Missing allocator class item in environment variable ARENA_ALLOCATOR_CONFIGURATION\n");
		std::abort();
	}
	return className.value();
}

Configuration::PoolMapType const& EnvironmentConfiguration::getPools() const noexcept
{
	if (!pools.has_value()) {
		fprintf(stderr, "EnvironmentConfiguration: Missing pools item in environment variable ARENA_ALLOCATOR_CONFIGURATION\n");
		std::abort();
	}
	return pools.value();
}

LogLevel const& EnvironmentConfiguration::getLogLevel() const noexcept
{
	if (!logLevel.has_value()) {
		fprintf(stderr, "EnvironmentConfiguration: Missing log level item in environment variable ARENA_ALLOCATOR_CONFIGURATION\n");
		std::abort();
	}
	return logLevel.value();
}

} // namespace ArenaAllocator
