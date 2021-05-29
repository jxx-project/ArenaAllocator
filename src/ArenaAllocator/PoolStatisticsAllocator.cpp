//
// Copyright (C) 2018 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/PoolStatisticsAllocator.h"
#include "ArenaAllocator/Timer.h"
#include <cerrno>
#include <cstring>
#include <limits>

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

void PoolStatisticsAllocator::registerAllocate(std::size_t size, void* result) noexcept
{
	std::lock_guard<std::mutex> guard(mutex);
	PoolStatistics* pool{pools.at(size)};
	if (!pool) {
		pool = &delegatePool;
	}
	pool->allocate(size);
	if (!allocations.emplace(result, {pool, size})) {
		logger.error(
			"PoolStatisticsAllocator::registerAllocate(%lu, %p) failed: result pointer already registered\n", size, result);
	}
}

void PoolStatisticsAllocator::registerDeallocate(void* ptr) noexcept
{
	std::lock_guard<std::mutex> guard(mutex);
	std::optional<AllocationMap::const_iterator> it{allocations.find(ptr)};
	if (it.has_value()) {
		it.value()->second.pool->deallocate();
	} else {
		logger.error("PoolStatisticsAllocator::registerDeallocate(%p) failed: allocation not found\n", ptr);
	}
}

void PoolStatisticsAllocator::registerReallocate(void* ptr, std::size_t size, void* result) noexcept
{
	std::lock_guard<std::mutex> guard(mutex);
	if (ptr) {
		std::optional<AllocationMap::const_iterator> it{allocations.find(ptr)};
		if (it.has_value()) {
			if (size) {
				PoolStatistics* destinationPool{pools.at(size)};
				if (!destinationPool) {
					destinationPool = &delegatePool;
					if (it.value()->second.pool != &delegatePool) {
						logger.error(
							"PoolStatisticsAllocator::registerReallocate(%p, %lu, %p) moved allocation out of arena pools\n",
							ptr,
							size,
							result);
					}
				}
				if (destinationPool == it.value()->second.pool) {
					it.value()->second.pool->reallocate(size);
				} else {
					destinationPool->allocate(size);
					it.value()->second.pool->deallocate();
				}
			} else {
				it.value()->second.pool->deallocate();
			}
		} else {
			logger.error(
				"PoolStatisticsAllocator::registerReallocate(%p, %lu, %p) failed: allocation not found\n", ptr, size, result);
		}
	}
}

} // namespace ArenaAllocator
