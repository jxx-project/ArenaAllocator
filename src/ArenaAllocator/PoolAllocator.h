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
#include <cstdint>

namespace ArenaAllocator {

class PoolAllocator : public Allocator
{
public:
	PoolAllocator(Configuration const& configuration, Allocator* delegate, Logger const& logger) noexcept;
	PoolAllocator(PoolAllocator const&) = delete;
	void operator=(PoolAllocator const&) = delete;
	virtual ~PoolAllocator() noexcept;

	virtual void* malloc(std::size_t size) noexcept override;
	virtual void free(void* ptr) noexcept override;
	virtual void* calloc(std::size_t nmemb, std::size_t size) noexcept override;
	virtual void* realloc(void* ptr, std::size_t size) noexcept override;
	virtual void* reallocarray(void* ptr, std::size_t nmemb, std::size_t size) noexcept override;

private:
	struct AllocateResult
	{
		void* ptr;
		int propagateErrno;
		bool fromDelegate;
	};

	struct DeallocateResult
	{
		int propagateErrno;
		bool fromDelegate;
	};

	AllocateResult allocate(std::size_t size, bool useDelegateRealloc = false) noexcept;
	DeallocateResult deallocate(void* ptr) noexcept;
	AllocateResult allocate(std::size_t nmemb, std::size_t size, bool useDelegateReallocArray = false) noexcept;
	AllocateResult reallocate(void* ptr, std::size_t size) noexcept;
	AllocateResult reallocate(void* ptr, std::size_t nmemb, std::size_t size) noexcept;

	Allocator* delegate;
	Logger const& logger;
	PoolMap<Pool> pools;
	const ChunkMap chunks;
};

} // namespace ArenaAllocator

#endif // ARENA_ALLOCATOR_POOL_ALLOCATOR_H_INCLUDED
