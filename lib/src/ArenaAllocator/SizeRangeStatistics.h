//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_PoolStatistics_Allocator_h_INCLUDED
#define ArenaAllocator_PoolStatistics_Allocator_h_INCLUDED

#include "ArenaAllocator/AllocationMap.h"
#include "ArenaAllocator/Allocator.h"
#include "ArenaAllocator/Configuration.h"
#include "ArenaAllocator/Logger.h"
#include <cstddef>
#include <mutex>

namespace ArenaAllocator {

class SizeRangeStatistics : public Allocator
{
public:
	SizeRangeStatistics(Configuration const& configuration, Allocator& delegate, Logger const& log) noexcept;
	SizeRangeStatistics(SizeRangeStatistics const&) = delete;
	void operator=(SizeRangeStatistics const&) = delete;
	virtual ~SizeRangeStatistics() noexcept;

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

	static constexpr char const* className{"SizeRangeStatistics"};

private:
	void* const ptrToEmpty;
	std::mutex mutex;
	Allocator& delegate;
	Logger const& log;
	PoolMap<PoolStatistics> pools;
	PoolStatistics delegatePool;
	AllocationMap allocations;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_PoolStatistics_Allocator_h_INCLUDED
