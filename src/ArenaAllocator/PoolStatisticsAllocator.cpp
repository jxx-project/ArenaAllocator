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
	allocations{pools, delegatePool, log}
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
			allocations.registerAllocate(size, result);
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
		allocations.registerDeallocate(ptr);
	}
}

void* PoolStatisticsAllocator::calloc(std::size_t nmemb, std::size_t size) noexcept
{
	std::lock_guard<std::mutex> guard(mutex);
	void* result;
	if (nmemb && size) {
		result = delegate.calloc(nmemb, size);
		if (result) {
			allocations.registerAllocate(nmemb * size, result);
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
		allocations.registerDeallocate(ptrOrNull);
		result = ptrToEmpty;
	} else if (result) {
		allocations.registerReallocate(ptrOrNull, size, result);
	}
	return result;
}

void* PoolStatisticsAllocator::reallocarray(void* ptr, std::size_t nmemb, std::size_t size) noexcept
{
	std::lock_guard<std::mutex> guard(mutex);
	void* ptrOrNull{ptr == ptrToEmpty ? nullptr : ptr};
	void* result{delegate.reallocarray(ptrOrNull, nmemb, size)};
	if (nmemb == 0 || size == 0) {
		allocations.registerDeallocate(ptrOrNull);
		result = ptrToEmpty;
	} else if (result) {
		allocations.registerReallocate(ptrOrNull, nmemb * size, result);
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
			allocations.registerAllocate(size, *memptr, alignment);
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
			allocations.registerAllocate(size, result, alignment);
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
			allocations.registerAllocate(size, result, ::sysconf(_SC_PAGESIZE));
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
			allocations.registerAllocate(size, result, alignment);
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
			allocations.registerAllocate(size, result, ::sysconf(_SC_PAGESIZE));
		}
	} else {
		result = ptrToEmpty;
	}
	return result;
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
