//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ARENA_ALLOCATOR_ALLOCATOR_FACTORY_H_INCLUDED
#define ARENA_ALLOCATOR_ALLOCATOR_FACTORY_H_INCLUDED

#include "ArenaAllocator/Allocator.h"
#include "ArenaAllocator/Configuration.h"

namespace ArenaAllocator {

class AllocatorFactory
{
public:
	virtual ~AllocatorFactory() noexcept = default;
	virtual Allocator* getAllocator(Configuration::StringType const& className) noexcept = 0;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator
