//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ARENA_ALLOCATOR_POOL_STATISTICS_ALLOCATOR_H_INCLUDED
#define ARENA_ALLOCATOR_POOL_STATISTICS_ALLOCATOR_H_INCLUDED

#include "ArenaAllocator/AllocationMap.h"
#include "ArenaAllocator/Allocator.h"
#include "ArenaAllocator/Configuration.h"
#include "ArenaAllocator/Logger.h"
#include "ArenaAllocator/PoolMap.h"
#include "ArenaAllocator/PoolStatistics.h"
#include <cstdint>
#include <mutex>

namespace ArenaAllocator {

class PoolStatisticsAllocator : public Allocator
{
public:
	PoolStatisticsAllocator(Configuration const& configuration, Allocator& delegate, Logger const& logger) noexcept;
	PoolStatisticsAllocator(PoolStatisticsAllocator const&) = delete;
	void operator=(PoolStatisticsAllocator const&) = delete;
	virtual ~PoolStatisticsAllocator() noexcept;

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

private:
	void registerAllocate(std::size_t size, void* result) noexcept;
	void registerAllocate(std::size_t size, void* result, std::size_t alignment) noexcept;
	void registerDeallocate(void* ptr) noexcept;
	void registerReallocate(void* ptr, std::size_t size, void* result) noexcept;
	void registerReallocate(void* ptr, std::size_t nmemb, std::size_t size, void* result) noexcept;

	std::mutex mutex;
	void* const ptrToEmpty;
	Allocator& delegate;
	Logger const& logger;
	PoolMap<PoolStatistics> pools;
	PoolStatistics delegatePool;
	AllocationMap allocations;
};

} // namespace ArenaAllocator

#endif // ARENA_ALLOCATOR_POOL_STATISTICS_ALLOCATOR_H_INCLUDED
