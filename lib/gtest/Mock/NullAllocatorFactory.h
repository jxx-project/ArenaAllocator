//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef MOCK_NULL_ALLOCATOR_FACTORY_H_INCLUDED
#define MOCK_NULL_ALLOCATOR_FACTORY_H_INCLUDED

#include "ArenaAllocator/AllocatorFactory.h"
#include "ArenaAllocator/Configuration.h"

namespace Mock {

class NullAllocatorFactory : public ArenaAllocator::AllocatorFactory
{
public:
	NullAllocatorFactory() noexcept;
	NullAllocatorFactory(NullAllocatorFactory const&) = delete;
	NullAllocatorFactory& operator=(NullAllocatorFactory const&) = delete;
	~NullAllocatorFactory() noexcept;

	virtual ArenaAllocator::Allocator* getAllocator(ArenaAllocator::Configuration::StringType const&) noexcept override;
};

} // namespace Mock

#endif // MOCK_NULL_ALLOCATOR_FACTORY_H_INCLUDED
