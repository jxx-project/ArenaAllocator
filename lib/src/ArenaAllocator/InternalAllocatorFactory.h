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
#include "ArenaAllocator/PassThrough.h"
#include "ArenaAllocator/SegregatedFreeLists.h"
#include "ArenaAllocator/SizeRangeStatistics.h"
#include <optional>

namespace ArenaAllocator {

class InternalAllocatorFactory : public AllocatorFactory
{
public:
	InternalAllocatorFactory(Configuration const& configuration, Logger* const& logger) noexcept;
	InternalAllocatorFactory(InternalAllocatorFactory const&) = delete;
	InternalAllocatorFactory& operator=(InternalAllocatorFactory const&) = delete;
	~InternalAllocatorFactory() override = default;

	Allocator* getAllocator(std::string_view const& className) noexcept override;

private:
	Configuration const& configuration;
	Logger* const& logger;
	std::optional<PassThrough> passThrough;
	std::optional<SegregatedFreeLists> segregatedFreeLists;
	std::optional<SizeRangeStatistics> sizeRangeStatistics;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_InternalAllocatorFactory_h_INCLUDED
