//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_EnvironmentConfiguration_h_INCLUDED
#define ArenaAllocator_EnvironmentConfiguration_h_INCLUDED

#include "ArenaAllocator/Allocator.h"
#include "ArenaAllocator/AllocatorFactory.h"
#include "ArenaAllocator/Configuration.h"
#include "ArenaAllocator/Logger.h"
#include "ArenaAllocator/LoggerFactory.h"
#include <optional>

namespace ArenaAllocator {

class EnvironmentConfiguration : public Configuration
{
public:
	EnvironmentConfiguration(
		char const* configStr,
		AllocatorFactory& allocatorFactory,
		Allocator*& allocator,
		LoggerFactory& loggerFactory,
		Logger*& logger) noexcept;
	EnvironmentConfiguration(EnvironmentConfiguration const&) = delete;
	EnvironmentConfiguration& operator=(EnvironmentConfiguration const&) = delete;
	~EnvironmentConfiguration() override = default;

	[[nodiscard]] std::string_view const& getClass() const noexcept override;
	[[nodiscard]] Configuration::PoolMapType const& getPools() const noexcept override;
	[[nodiscard]] LogLevel const& getLogLevel() const noexcept override;
	[[nodiscard]] std::string_view const& getLogger() const noexcept override;

	static constexpr char const* configurationEnvVarName{"ARENA_ALLOCATOR_CONFIGURATION"};

private:
	Allocator*& allocator;
	Logger*& logger;
	std::optional<std::string_view> className;
	std::optional<Configuration::PoolMapType> pools;
	std::optional<LogLevel> logLevel;
	std::optional<std::string_view> loggerName;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_EnvironmentConfiguration_h_INCLUDED
