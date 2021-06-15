//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_LoggerFactory_h_INCLUDED
#define ArenaAllocator_LoggerFactory_h_INCLUDED

#include "ArenaAllocator/Configuration.h"
#include "ArenaAllocator/Logger.h"

namespace ArenaAllocator {

class LoggerFactory
{
public:
	virtual ~LoggerFactory() noexcept = default;

	virtual Logger* getLogger(std::string_view const& className) noexcept = 0;
};

} // namespace ArenaAllocator

#endif // ArenaLogger_LoggerFactory_h_INCLUDED
