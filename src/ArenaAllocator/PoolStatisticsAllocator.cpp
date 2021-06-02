//
// Copyright (C) 2018 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/PoolStatisticsAllocator.h"
#include "ArenaAllocator/Pool.h"
#include "ArenaAllocator/Timer.h"
#include <cerrno>
#include <cstring>
#include <limits>
#include <unistd.h>

namespace ArenaAllocator {

namespace {

void* getPtrToEmpty()
{
	static struct
	{
	} empty;
	return &empty;
}

} // namespace

PoolStatisticsAllocator::PoolStatisticsAllocator(
	Configuration const& configuration,
	Allocator& delegate,
	Logger const& logger) noexcept :
	ptrToEmpty{getPtrToEmpty()},
	delegate{delegate},
	logger{logger},
	pools{configuration, logger},
	delegatePool{SizeRange{1, std::numeric_limits<std::size_t>::max()}, 0, logger},
	allocations{logger}
{
	logger.debug(
		"PoolStatisticsAllocator::PoolStatisticsAllocator(Configuration const&, Allocator&, Logger const&) {ptrToEmpty: %p}\n",
		ptrToEmpty);
}

PoolStatisticsAllocator::~PoolStatisticsAllocator() noexcept
{
	logger.debug("PoolStatisticsAllocator::~PoolStatisticsAllocator()\n");
}

void* PoolStatisticsAllocator::malloc(std::size_t size) noexcept
{
	void* result;
	if (size) {
		result = delegate.malloc(size);
		if (result) {
			registerAllocate(size, result);
		}
	} else {
		result = ptrToEmpty;
	}
	return result;
}

void PoolStatisticsAllocator::free(void* ptr) noexcept
{
	if (ptr && ptr != ptrToEmpty) {
		delegate.free(ptr);
		registerDeallocate(ptr);
	}
}

void* PoolStatisticsAllocator::calloc(std::size_t nmemb, std::size_t size) noexcept
{
	void* result;
	if (nmemb && size) {
		result = delegate.calloc(nmemb, size);
		if (result) {
			registerAllocate(nmemb * size, result);
		}
	} else {
		result = ptrToEmpty;
	}
	return result;
}

void* PoolStatisticsAllocator::realloc(void* ptr, std::size_t size) noexcept
{
	void* ptrOrNull{ptr == ptrToEmpty ? nullptr : ptr};
	void* result{delegate.realloc(ptrOrNull, size)};
	if (result) {
		registerReallocate(ptrOrNull, size, result);
	}
	return result;
}

void* PoolStatisticsAllocator::reallocarray(void* ptr, std::size_t nmemb, std::size_t size) noexcept
{
	void* ptrOrNull{ptr == ptrToEmpty ? nullptr : ptr};
	void* result{delegate.reallocarray(ptrOrNull, nmemb, size)};
	if (result) {
		registerReallocate(ptrOrNull, nmemb * size, result);
	}
	return result;
}

int PoolStatisticsAllocator::posix_memalign(void** memptr, std::size_t alignment, std::size_t size) noexcept
{
	int result;
	if (size) {
		result = delegate.posix_memalign(memptr, alignment, size);
		if (result) {
			registerAllocate(size, *memptr, alignment);
		}
	} else {
		*memptr = ptrToEmpty;
		result = 0;
	}
	return result;
}

void* PoolStatisticsAllocator::aligned_alloc(std::size_t alignment, std::size_t size) noexcept
{
	void* result;
	if (size) {
		result = delegate.aligned_alloc(alignment, size);
		if (result) {
			registerAllocate(size, result, alignment);
		}
	} else {
		result = ptrToEmpty;
	}
	return result;
}

void* PoolStatisticsAllocator::valloc(std::size_t size) noexcept
{
	void* result;
	if (size) {
		result = delegate.valloc(size);
		if (result) {
			registerAllocate(size, result, ::sysconf(_SC_PAGESIZE));
		}
	} else {
		result = ptrToEmpty;
	}
	return result;
}

void* PoolStatisticsAllocator::memalign(std::size_t alignment, std::size_t size) noexcept
{
	void* result;
	if (size) {
		result = delegate.pvalloc(size);
		if (result) {
			registerAllocate(size, result, alignment);
		}
	} else {
		result = ptrToEmpty;
	}
	return result;
}

void* PoolStatisticsAllocator::pvalloc(std::size_t size) noexcept
{
	void* result;
	if (size) {
		result = delegate.pvalloc(size);
		if (result) {
			registerAllocate(size, result, ::sysconf(_SC_PAGESIZE));
		}
	} else {
		result = ptrToEmpty;
	}
	return result;
}

void PoolStatisticsAllocator::registerAllocate(std::size_t size, void* result) noexcept
{
	logger.debug("PoolStatisticsAllocator::registerAllocate(%lu, %p)\n", size, result);
	std::lock_guard<std::mutex> guard(mutex);
	PoolStatistics* pool{pools.at(size)};
	if (!pool) {
		pool = &delegatePool;
	}
	allocations.registerAllocation(result, {pool, size});
}

void PoolStatisticsAllocator::registerAllocate(std::size_t size, void* result, std::size_t alignment) noexcept
{
	logger.debug("PoolStatisticsAllocator::registerAllocate(%lu, %p)\n", size, result);
	std::lock_guard<std::mutex> guard(mutex);
	PoolStatistics* pool{alignment <= sizeof(Pool::WordType) ? pools.at(size) : &delegatePool};
	if (!pool) {
		pool = &delegatePool;
	}
	allocations.registerAllocation(result, {pool, size});
}

void PoolStatisticsAllocator::registerDeallocate(void* ptr) noexcept
{
	logger.debug("PoolStatisticsAllocator::registerDeallocate(%p)\n", ptr);
	std::lock_guard<std::mutex> guard(mutex);
	std::optional<AllocationMap::const_iterator> it{allocations.find(ptr)};
	if (it.has_value()) {
		allocations.unregisterAllocation(it.value());
	} else {
		logger.error("PoolStatisticsAllocator::registerDeallocate(%p) failed: allocation not found\n", ptr);
	}
}

void PoolStatisticsAllocator::registerReallocate(void* ptr, std::size_t size, void* result) noexcept
{
	logger.debug("PoolStatisticsAllocator::registerReallocate(%p, %lu, %p)\n", ptr, size, result);
	if (ptr) {
		std::lock_guard<std::mutex> guard(mutex);
		std::optional<AllocationMap::const_iterator> it{allocations.find(ptr)};
		if (it.has_value()) {
			PoolStatistics* currentPool{it.value()->second.pool};
			if (size) {
				PoolStatistics* destinationPool{pools.at(size)};
				if (!destinationPool) {
					destinationPool = &delegatePool;
					if (currentPool != &delegatePool) {
						logger.error(
							"PoolStatisticsAllocator::registerReallocate(%p, %lu, %p) allocation moved out of arena pools\n",
							ptr,
							size,
							result);
					}
				}
				if (destinationPool == currentPool) {
					currentPool->registerReallocate(size);
				} else {
					allocations.unregisterAllocation(it.value());
					allocations.registerAllocation(result, {destinationPool, size});
				}
			} else {
				allocations.unregisterAllocation(it.value());
			}
		} else {
			logger.error(
				"PoolStatisticsAllocator::registerReallocate(%p, %lu, %p) failed: allocation not found\n", ptr, size, result);
			registerAllocate(size, result);
		}
	} else {
		registerAllocate(size, result);
	}
}

} // namespace ArenaAllocator
