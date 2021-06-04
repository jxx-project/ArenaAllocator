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
	Logger const& log) noexcept :
	ptrToEmpty{getPtrToEmpty()},
	delegate{delegate},
	log{log},
	pools{configuration, log},
	delegatePool{SizeRange{1, std::numeric_limits<std::size_t>::max()}, 0, log},
	allocations{log}
{
	log(LogLevel::DEBUG,
		"PoolStatisticsAllocator::PoolStatisticsAllocator(Configuration const&, Allocator&, Logger const&) {ptrToEmpty: %p}",
		ptrToEmpty);
}

PoolStatisticsAllocator::~PoolStatisticsAllocator() noexcept
{
	log(LogLevel::DEBUG, "PoolStatisticsAllocator::~PoolStatisticsAllocator()");
	if (log.isLevel(LogLevel::INFO)) {
		dump();
	}
}

void* PoolStatisticsAllocator::malloc(std::size_t size) noexcept
{
	std::lock_guard<std::mutex> guard(mutex);
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
	std::lock_guard<std::mutex> guard(mutex);
	if (ptr && ptr != ptrToEmpty) {
		delegate.free(ptr);
		registerDeallocate(ptr);
	}
}

void* PoolStatisticsAllocator::calloc(std::size_t nmemb, std::size_t size) noexcept
{
	std::lock_guard<std::mutex> guard(mutex);
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
	std::lock_guard<std::mutex> guard(mutex);
	void* ptrOrNull{ptr == ptrToEmpty ? nullptr : ptr};
	void* result{delegate.realloc(ptrOrNull, size)};
	if (size == 0) {
		registerDeallocate(ptrOrNull);
		result = ptrToEmpty;
	} else if (result) {
		registerReallocate(ptrOrNull, size, result);
	}
	return result;
}

void* PoolStatisticsAllocator::reallocarray(void* ptr, std::size_t nmemb, std::size_t size) noexcept
{
	std::lock_guard<std::mutex> guard(mutex);
	void* ptrOrNull{ptr == ptrToEmpty ? nullptr : ptr};
	void* result{delegate.reallocarray(ptrOrNull, nmemb, size)};
	if (nmemb == 0 || size == 0) {
		registerDeallocate(ptrOrNull);
		result = ptrToEmpty;
	} else if (result) {
		registerReallocate(ptrOrNull, nmemb * size, result);
	}
	return result;
}

int PoolStatisticsAllocator::posix_memalign(void** memptr, std::size_t alignment, std::size_t size) noexcept
{
	std::lock_guard<std::mutex> guard(mutex);
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
	std::lock_guard<std::mutex> guard(mutex);
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
	std::lock_guard<std::mutex> guard(mutex);
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
	std::lock_guard<std::mutex> guard(mutex);
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
	std::lock_guard<std::mutex> guard(mutex);
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
	log(LogLevel::DEBUG, "PoolStatisticsAllocator::registerAllocate(%lu, %p)", size, result);
	PoolStatistics* pool{pools.at(size)};
	if (!pool) {
		pool = &delegatePool;
	}
	allocations.registerAllocation(result, {pool, size});
}

void PoolStatisticsAllocator::registerAllocate(std::size_t size, void* result, std::size_t alignment) noexcept
{
	log(LogLevel::DEBUG, "PoolStatisticsAllocator::registerAllocate(%lu, %p)", size, result);
	PoolStatistics* pool{alignment <= sizeof(std::max_align_t) ? pools.at(size) : &delegatePool};
	if (!pool) {
		pool = &delegatePool;
	}
	allocations.registerAllocation(result, {pool, size});
}

void PoolStatisticsAllocator::registerDeallocate(void* ptr) noexcept
{
	log(LogLevel::DEBUG, "PoolStatisticsAllocator::registerDeallocate(%p)", ptr);
	if (ptr) {
		std::optional<AllocationMap::iterator> it{allocations.find(ptr)};
		if (it.has_value()) {
			allocations.unregisterAllocation(it.value());
		} else {
			log(LogLevel::ERROR, "PoolStatisticsAllocator::registerDeallocate(%p) allocation not found", ptr);
			if (log.isLevel(LogLevel::DEBUG)) {
				allocations.dump();
			}
		}
	}
}

void PoolStatisticsAllocator::registerReallocate(void* ptr, std::size_t size, void* result) noexcept
{
	log(LogLevel::DEBUG, "PoolStatisticsAllocator::registerReallocate(%p, %lu, %p)", ptr, size, result);
	if (ptr) {
		std::optional<AllocationMap::iterator> it{allocations.find(ptr)};
		if (it.has_value()) {
			if (size) {
				PoolStatistics* destinationPool{pools.at(size)};
				if (!destinationPool) {
					destinationPool = &delegatePool;
					if (it.value()->second.pool != &delegatePool) {
						log(LogLevel::ERROR,
							"PoolStatisticsAllocator::registerReallocate(%p, %lu, %p) allocation moved out of arena pools",
							ptr,
							size,
							result);
					}
				}
				allocations.updateAllocation(it.value(), result, {destinationPool, size});
			} else {
				allocations.unregisterAllocation(it.value());
			}
		} else {
			log(LogLevel::ERROR,
				"PoolStatisticsAllocator::registerReallocate(%p, %lu, %p): allocation not found",
				ptr,
				size,
				result);
			if (log.isLevel(LogLevel::DEBUG)) {
				allocations.dump();
			}
			registerAllocate(size, result);
		}
	} else {
		registerAllocate(size, result);
	}
}

void PoolStatisticsAllocator::dump() const noexcept
{
	pools.dump();
	delegatePool.dump();
	if (log.isLevel(LogLevel::DEBUG)) {
		allocations.dump();
	}
}

} // namespace ArenaAllocator
