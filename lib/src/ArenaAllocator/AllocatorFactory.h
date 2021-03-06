//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_AllocatorFactory_h_INCLUDED
#define ArenaAllocator_AllocatorFactory_h_INCLUDED

#include "ArenaAllocator/Allocator.h"
#include "ArenaAllocator/Configuration.h"

namespace ArenaAllocator {

class AllocatorFactory
{
public:
	virtual ~AllocatorFactory() noexcept = default;

	virtual Allocator* getAllocator(std::string_view const& className) noexcept = 0;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_AllocatorFactory_h_INCLUDED
