//
// Copyright (C) 2018 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/InternalAllocatorFactory.h"

namespace ArenaAllocator {

InternalAllocatorFactory::InternalAllocatorFactory(Configuration const& configuration, Logger const& log) noexcept :
	configuration{configuration}, log{log}
{
}

InternalAllocatorFactory::~InternalAllocatorFactory() noexcept
{
}

Allocator* ArenaAllocator::InternalAllocatorFactory::getAllocator(std::string_view const& className) noexcept
{
	Allocator* result{nullptr};
	if (className == PassThroughAllocator::className) {
		if (!passThroughAllocator.has_value()) {
			passThroughAllocator.emplace(log);
		}
		result = &passThroughAllocator.value();
	} else if (className == PoolAllocator::className) {
		if (!poolAllocator.has_value()) {
			poolAllocator.emplace(configuration, getAllocator(PassThroughAllocator::className), log);
		}
		result = &poolAllocator.value();
	} else if (className == PoolStatisticsAllocator::className) {
		if (!poolStatisticsAllocator.has_value()) {
			poolStatisticsAllocator.emplace(configuration, *getAllocator(PassThroughAllocator::className), log);
		}
		result = &poolStatisticsAllocator.value();
	}
	return result;
}

} // namespace ArenaAllocator
