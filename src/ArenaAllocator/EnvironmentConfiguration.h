//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ARENA_ALLOCATOR_ENVIRONMENT_CONFIGURATION_H_INCLUDED
#define ARENA_ALLOCATOR_ENVIRONMENT_CONFIGURATION_H_INCLUDED

#include "ArenaAllocator/Allocator.h"
#include "ArenaAllocator/AllocatorFactory.h"
#include "ArenaAllocator/Configuration.h"
#include "ArenaAllocator/Logger.h"
#include "ArenaAllocator/Optional.h"

namespace ArenaAllocator {

class EnvironmentConfiguration : public Configuration
{
public:
	EnvironmentConfiguration(AllocatorFactory& allocatorFactory, Allocator*& activeAllocator, Logger& logger) noexcept;
	EnvironmentConfiguration(EnvironmentConfiguration const&) = delete;
	EnvironmentConfiguration& operator=(EnvironmentConfiguration const&) = delete;
	~EnvironmentConfiguration() noexcept;

	virtual Configuration::StringType const& getClass() const noexcept override;
	virtual Configuration::PoolMapType const& getPools() const noexcept override;
	virtual LogLevel const& getLogLevel() const noexcept override;

private:
	Allocator*& activeAllocator;
	Logger& logger;
	Optional<Configuration::StringType> className;
	Optional<Configuration::PoolMapType> pools;
	Optional<LogLevel> logLevel;
};

} // namespace ArenaAllocator

#endif // ARENA_ALLOCATOR_ENVIRONMENT_CONFIGURATION_H_INCLUDED
