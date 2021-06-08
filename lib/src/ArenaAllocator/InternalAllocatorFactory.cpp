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
	if (className == PassThrough::className) {
		if (!passThrough.has_value()) {
			passThrough.emplace(log);
		}
		result = &passThrough.value();
	} else if (className == SegregatedFreeLists::className) {
		if (!segregatedFreeLists.has_value()) {
			segregatedFreeLists.emplace(configuration, getAllocator(PassThrough::className), log);
		}
		result = &segregatedFreeLists.value();
	} else if (className == SizeRangeStatistics::className) {
		if (!sizeRangeStatistics.has_value()) {
			sizeRangeStatistics.emplace(configuration, *getAllocator(PassThrough::className), log);
		}
		result = &sizeRangeStatistics.value();
	}
	return result;
}

} // namespace ArenaAllocator
