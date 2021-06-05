//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ARENA_ALLOCATOR_POOL_ALLOCATOR_H_INCLUDED
#define ARENA_ALLOCATOR_POOL_ALLOCATOR_H_INCLUDED

#include "ArenaAllocator/Allocator.h"
#include "ArenaAllocator/ChunkMap.h"
#include "ArenaAllocator/Configuration.h"
#include "ArenaAllocator/Logger.h"
#include "ArenaAllocator/Pool.h"
#include "ArenaAllocator/PoolMap.h"
#include <cstddef>

namespace ArenaAllocator {

class PoolAllocator : public Allocator
{
public:
	PoolAllocator(Configuration const& configuration, Allocator* delegate, Logger const& log) noexcept;
	PoolAllocator(PoolAllocator const&) = delete;
	void operator=(PoolAllocator const&) = delete;
	virtual ~PoolAllocator() noexcept;

	virtual void* malloc(std::size_t size) noexcept override;
	virtual void free(void* ptr) noexcept override;
	virtual void* calloc(std::size_t nmemb, std::size_t size) noexcept override;
	virtual void* realloc(void* ptr, std::size_t size) noexcept override;
	virtual void* reallocarray(void* ptr, std::size_t nmemb, std::size_t size) noexcept override;
	virtual int posix_memalign(void** memptr, std::size_t alignment, std::size_t size) noexcept override;
	virtual void* aligned_alloc(std::size_t alignment, std::size_t size) noexcept override;
	virtual void* valloc(std::size_t size) noexcept override;
	virtual void* memalign(std::size_t alignment, std::size_t size) noexcept override;
	virtual void* pvalloc(std::size_t size) noexcept override;
	virtual void dump() const noexcept override;

private:
	Allocator* delegate;
	Logger const& log;
	PoolMap<Pool> pools;
	const ChunkMap chunks;
};

} // namespace ArenaAllocator

#endif // ARENA_ALLOCATOR_POOL_ALLOCATOR_H_INCLUDED
