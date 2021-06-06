//
// Copyright (C) 2018 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_InternalAllocatorFactory_h_INCLUDED
#define ArenaAllocator_InternalAllocatorFactory_h_INCLUDED

#include "ArenaAllocator/AllocatorFactory.h"
#include "ArenaAllocator/Configuration.h"
#include "ArenaAllocator/Logger.h"
#include "ArenaAllocator/PassThroughAllocator.h"
#include "ArenaAllocator/PoolAllocator.h"
#include "ArenaAllocator/PoolStatisticsAllocator.h"
#include <optional>

namespace ArenaAllocator {

class InternalAllocatorFactory : public AllocatorFactory
{
public:
	InternalAllocatorFactory(Configuration const& configuration, Logger const& log) noexcept;
	InternalAllocatorFactory(InternalAllocatorFactory const&) = delete;
	InternalAllocatorFactory& operator=(InternalAllocatorFactory const&) = delete;
	~InternalAllocatorFactory() noexcept;

	virtual Allocator* getAllocator(Configuration::StringType const& className) noexcept override;

private:
	Configuration const& configuration;
	Logger const& log;
	std::optional<PassThroughAllocator> passThroughAllocator;
	std::optional<PoolAllocator> poolAllocator;
	std::optional<PoolStatisticsAllocator> poolStatisticsAllocator;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_InternalAllocatorFactory_h_INCLUDED
