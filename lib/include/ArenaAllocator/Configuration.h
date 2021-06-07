//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_Configuration_h_INCLUDED
#define ArenaAllocator_Configuration_h_INCLUDED

#include "ArenaAllocator/LogLevel.h"
#include "ArenaAllocator/PassThroughCXXAllocator.h"
#include "ArenaAllocator/SizeRangeMap.h"
#include <cstddef>
#include <map>
#include <string_view>

namespace ArenaAllocator {

class Configuration
{
public:
	using PoolMapType = SizeRangeMap<std::size_t>;

	virtual ~Configuration() noexcept = default;

	virtual std::string_view const& getClass() const noexcept = 0;
	virtual PoolMapType const& getPools() const noexcept = 0;
	virtual LogLevel const& getLogLevel() const noexcept = 0;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_Configuration_h_INCLUDED
