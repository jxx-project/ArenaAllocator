//
// Copyright (C) 2018 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/InternalAllocatorFactory.h"

namespace ArenaAllocator {

InternalAllocatorFactory::InternalAllocatorFactory(Configuration const& configuration, Logger* const& logger) noexcept :
	configuration{configuration}, logger{logger}
{
}

Allocator* ArenaAllocator::InternalAllocatorFactory::getAllocator(std::string_view const& className) noexcept
{
	Allocator* result{nullptr};
	if (className == PassThrough::className) {
		if (!passThrough.has_value()) {
			passThrough.emplace(*logger);
		}
		result = &passThrough.value();
	} else if (className == SegregatedFreeLists::className) {
		if (!segregatedFreeLists.has_value()) {
			segregatedFreeLists.emplace(configuration, getAllocator(PassThrough::className), *logger);
		}
		result = &segregatedFreeLists.value();
	} else if (className == SizeRangeStatistics::className) {
		if (!sizeRangeStatistics.has_value()) {
			sizeRangeStatistics.emplace(configuration, *getAllocator(PassThrough::className), *logger);
		}
		result = &sizeRangeStatistics.value();
	}
	return result;
}

} // namespace ArenaAllocator
