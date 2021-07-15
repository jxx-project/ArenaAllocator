//
// Copyright (C) 2018 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/InternalLoggerFactory.h"

namespace ArenaAllocator {

Logger* InternalLoggerFactory::getLogger(std::string_view const& className) noexcept
{
	Logger* result{nullptr};
	if (className == Console::className) {
		if (!consoleLogger.has_value()) {
			consoleLogger.emplace();
		}
		result = &consoleLogger.value();
	} else if (className == TimeTrace::className) {
		if (!timerLogger.has_value()) {
			timerLogger.emplace();
		}
		result = &timerLogger.value();
	}
	return result;
}

} // namespace ArenaAllocator
