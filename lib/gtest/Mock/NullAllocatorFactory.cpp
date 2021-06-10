//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "Mock/NullAllocatorFactory.h"

namespace Mock {

NullAllocatorFactory::NullAllocatorFactory() noexcept
{
}

ArenaAllocator::Allocator* NullAllocatorFactory::getAllocator(std::string_view const&) noexcept
{
	return nullptr;
}

} // namespace Mock
