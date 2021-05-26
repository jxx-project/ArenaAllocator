//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ARENA_ALLOCATOR_CONFIGURATION_H_INCLUDED
#define ARENA_ALLOCATOR_CONFIGURATION_H_INCLUDED

#include "ArenaAllocator/PassThroughCXXAllocator.h"
#include "ArenaAllocator/SizeRangeMap.h"
#include <cstdint>
#include <map>

namespace ArenaAllocator {

class Configuration
{
public:
	using PoolMapType = SizeRangeMap<std::size_t>;
	using StringType = std::basic_string<char, std::char_traits<char>, PassThroughCXXAllocator<char>>;

	virtual ~Configuration() noexcept = default;
	virtual StringType const& getClass() const noexcept = 0;
	virtual PoolMapType const& getPools() const noexcept = 0;
};

} // namespace ArenaAllocator

#endif // ARENA_ALLOCATOR_CONFIGURATION_H_INCLUDED
