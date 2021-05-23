//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ARENA_ALLOCATOR_CONFIGURATION_H_INCLUDED
#define ARENA_ALLOCATOR_CONFIGURATION_H_INCLUDED

#include "ArenaAllocator/PassThroughCXXAllocator.h"
#include <cstdint>
#include <map>

namespace ArenaAllocator {

class Configuration
{
public:
	using MapType = std::
		map<std::size_t, std::size_t, std::less<std::size_t>, PassThroughCXXAllocator<std::pair<const std::size_t, std::size_t>>>;

	virtual ~Configuration() noexcept = default;
	virtual MapType const& getArenas() const noexcept = 0;
};

} // namespace ArenaAllocator

#endif // ARENA_ALLOCATOR_CONFIGURATION_H_INCLUDED
