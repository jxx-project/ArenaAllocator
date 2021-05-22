//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef STATIC_CONFIGURATION_H_INCLUDED
#define STATIC_CONFIGURATION_H_INCLUDED

#include "CustomAllocators/Allocator.h"
#include "CustomAllocators/Configuration.h"
#include "CustomAllocators/Logger.h"

namespace CustomAllocators {

class StaticConfiguration : public Configuration
{
public:
	StaticConfiguration(Allocator*& activeAllocator, Logger& logger) noexcept;
	StaticConfiguration(StaticConfiguration const&) = delete;
	StaticConfiguration& operator=(StaticConfiguration const&) = delete;
	~StaticConfiguration() noexcept;
	virtual Configuration::MapType const& getArenas() const noexcept override;

private:
	Allocator*& activeAllocator;
	Logger& logger;
	Configuration::MapType arenas;
};

} // namespace CustomAllocators

#endif // STATIC_CONFIGURATION_H_INCLUDED
