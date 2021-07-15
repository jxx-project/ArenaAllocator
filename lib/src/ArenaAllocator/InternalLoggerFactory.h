//
// Copyright (C) 2018 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_InternalLoggerFactory_h_INCLUDED
#define ArenaAllocator_InternalLoggerFactory_h_INCLUDED

#include "ArenaAllocator/Console.h"
#include "ArenaAllocator/LoggerFactory.h"
#include "ArenaAllocator/TimeTrace.h"
#include <optional>

namespace ArenaAllocator {

class InternalLoggerFactory : public LoggerFactory
{
public:
	InternalLoggerFactory() noexcept = default;
	InternalLoggerFactory(InternalLoggerFactory const&) = delete;
	InternalLoggerFactory& operator=(InternalLoggerFactory const&) = delete;
	~InternalLoggerFactory() override = default;

	Logger* getLogger(std::string_view const& className) noexcept override;

private:
	std::optional<Console> consoleLogger;
	std::optional<TimeTrace> timerLogger;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_InternalLoggerFactory_h_INCLUDED
