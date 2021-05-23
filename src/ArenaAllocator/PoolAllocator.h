//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef POOL_ALLOCATOR_H_INCLUDED
#define POOL_ALLOCATOR_H_INCLUDED

#include "ArenaAllocator/Allocator.h"
#include "ArenaAllocator/ChunkMap.h"
#include "ArenaAllocator/Configuration.h"
#include "ArenaAllocator/Logger.h"
#include "ArenaAllocator/PoolMap.h"
#include <cstdint>

namespace ArenaAllocator {

class PoolAllocator : public Allocator
{
public:
	PoolAllocator(Configuration const& configuration, Logger const& logger) noexcept;
	PoolAllocator(PoolAllocator const&) = delete;
	void operator=(PoolAllocator const&) = delete;
	virtual ~PoolAllocator() noexcept;
	virtual void* malloc(std::size_t size) noexcept override;
	virtual void free(void* ptr) noexcept override;
	virtual void* calloc(std::size_t nmemb, std::size_t size) noexcept override;
	virtual void* realloc(void* ptr, std::size_t size) noexcept override;
	virtual void* reallocarray(void* ptr, std::size_t nmemb, std::size_t size) noexcept override;

private:
	Logger const& logger;
	PoolMap pools;
	const ChunkMap chunks;
};

} // namespace ArenaAllocator

#endif // POOL_ALLOCATOR_H_INCLUDED
