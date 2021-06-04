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
#include <optional>

namespace ArenaAllocator {

class EnvironmentConfiguration : public Configuration
{
public:
	EnvironmentConfiguration(AllocatorFactory& allocatorFactory, Allocator*& activeAllocator, Logger& log) noexcept;
	EnvironmentConfiguration(EnvironmentConfiguration const&) = delete;
	EnvironmentConfiguration& operator=(EnvironmentConfiguration const&) = delete;
	~EnvironmentConfiguration() noexcept;

	virtual Configuration::StringType const& getClass() const noexcept override;
	virtual Configuration::PoolMapType const& getPools() const noexcept override;
	virtual LogLevel const& getLogLevel() const noexcept override;

private:
	Allocator*& activeAllocator;
	Logger& log;
	std::optional<Configuration::StringType> className;
	std::optional<Configuration::PoolMapType> pools;
	std::optional<LogLevel> logLevel;
};

} // namespace ArenaAllocator

#endif // ARENA_ALLOCATOR_ENVIRONMENT_CONFIGURATION_H_INCLUDED
