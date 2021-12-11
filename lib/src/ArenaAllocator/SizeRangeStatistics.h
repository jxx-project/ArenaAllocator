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
#include <string_view>

namespace ArenaAllocator {

class SizeRangeStatistics : public Allocator
{
public:
	SizeRangeStatistics(Configuration const& configuration, Allocator& delegate, Logger const& log) noexcept;
	SizeRangeStatistics(SizeRangeStatistics const&) = delete;
	void operator=(SizeRangeStatistics const&) = delete;
	~SizeRangeStatistics() noexcept override;

	void* malloc(std::size_t size) noexcept override;
	void free(void* ptr) noexcept override;
	void* calloc(std::size_t nmemb, std::size_t size) noexcept override;
	void* realloc(void* ptr, std::size_t size) noexcept override;
	void* mmap(void* addr, std::size_t length, int prot, int flags, int fd, off_t offset) noexcept override;
	int munmap(void* addr, std::size_t length) noexcept override;
	void dump() const noexcept override;

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
