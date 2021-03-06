//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef Mock_NullAllocatorFactory_h_INCLUDED
#define Mock_NullAllocatorFactory_h_INCLUDED

#include "ArenaAllocator/AllocatorFactory.h"
#include "ArenaAllocator/Configuration.h"

namespace Mock {

class NullAllocatorFactory : public ArenaAllocator::AllocatorFactory
{
public:
	NullAllocatorFactory() = default;
	NullAllocatorFactory(NullAllocatorFactory const&) = delete;
	NullAllocatorFactory& operator=(NullAllocatorFactory const&) = delete;
	~NullAllocatorFactory() override = default;

	ArenaAllocator::Allocator* getAllocator(std::string_view const&) noexcept override;
};

} // namespace Mock

#endif // Mock_NullAllocatorFactory_h_INCLUDED
