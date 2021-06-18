//
// Copyright (C) 2018 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/SizeRangeStatistics.h"
#include "ArenaAllocator/FreeList.h"
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

SizeRangeStatistics::SizeRangeStatistics(Configuration const& configuration, Allocator& delegate, Logger const& log) noexcept :
	ptrToEmpty{getPtrToEmpty()},
	delegate{delegate},
	log{log},
	pools{configuration, log},
	delegatePool{SizeRange{1, std::numeric_limits<std::size_t>::max()}, 0, log},
	allocations{pools, delegatePool, log}
{
	log(LogLevel::DEBUG, [&] {
		return Format(
			"{}::{}(Configuration const&, Allocator&, Logger const&) -> {this:{}, ptrToEmpty:{}}",
			className,
			className,
			this,
			ptrToEmpty);
	});
}

SizeRangeStatistics::~SizeRangeStatistics() noexcept
{
	log(LogLevel::DEBUG, [&] { return Format("{}::~{}(this:{})", className, className, this); });
}

void* SizeRangeStatistics::malloc(std::size_t size) noexcept
{
	std::lock_guard<std::mutex> guard{mutex};
	void* result{nullptr};
	if (size > 0) {
		result = delegate.malloc(size);
		if (result != nullptr) {
			allocations.registerAllocate(size, result);
		}
	} else {
		result = ptrToEmpty;
	}
	return result;
}

void SizeRangeStatistics::free(void* ptr) noexcept
{
	std::lock_guard<std::mutex> guard{mutex};
	if (ptr != nullptr && ptr != ptrToEmpty) {
		delegate.free(ptr);
		allocations.registerDeallocate(ptr);
	}
}

void* SizeRangeStatistics::calloc(std::size_t nmemb, std::size_t size) noexcept
{
	std::lock_guard<std::mutex> guard{mutex};
	void* result{nullptr};
	if (nmemb > 0 && size > 0) {
		result = delegate.calloc(nmemb, size);
		if (result != nullptr) {
			allocations.registerAllocate(nmemb * size, result);
		}
	} else {
		result = ptrToEmpty;
	}
	return result;
}

void* SizeRangeStatistics::realloc(void* ptr, std::size_t size) noexcept
{
	std::lock_guard<std::mutex> guard{mutex};
	void* ptrOrNull{ptr == ptrToEmpty ? nullptr : ptr};
	void* result{delegate.realloc(ptrOrNull, size)};
	if (size == 0) {
		allocations.registerDeallocate(ptrOrNull);
		result = ptrToEmpty;
	} else if (result != nullptr) {
		allocations.registerReallocate(ptrOrNull, size, result);
	}
	return result;
}

void* SizeRangeStatistics::reallocarray(void* ptr, std::size_t nmemb, std::size_t size) noexcept
{
	std::lock_guard<std::mutex> guard{mutex};
	void* ptrOrNull{ptr == ptrToEmpty ? nullptr : ptr};
	void* result{delegate.reallocarray(ptrOrNull, nmemb, size)};
	if (nmemb == 0 || size == 0) {
		allocations.registerDeallocate(ptrOrNull);
		result = ptrToEmpty;
	} else if (result != nullptr) {
		allocations.registerReallocate(ptrOrNull, nmemb * size, result);
	}
	return result;
}

int SizeRangeStatistics::posix_memalign(void** memptr, std::size_t alignment, std::size_t size) noexcept
{
	std::lock_guard<std::mutex> guard{mutex};
	int result{0};
	if (size > 0) {
		result = delegate.posix_memalign(memptr, alignment, size);
		if (result == 0) {
			allocations.registerAllocate(size, *memptr, alignment);
		}
	} else {
		*memptr = ptrToEmpty;
		result = 0;
	}
	return result;
}

void* SizeRangeStatistics::aligned_alloc(std::size_t alignment, std::size_t size) noexcept
{
	std::lock_guard<std::mutex> guard{mutex};
	void* result{nullptr};
	if (size > 0) {
		result = delegate.aligned_alloc(alignment, size);
		if (result != nullptr) {
			allocations.registerAllocate(size, result, alignment);
		}
	} else {
		result = ptrToEmpty;
	}
	return result;
}

void* SizeRangeStatistics::valloc(std::size_t size) noexcept
{
	std::lock_guard<std::mutex> guard{mutex};
	void* result{nullptr};
	if (size > 0) {
		result = delegate.valloc(size);
		if (result != nullptr) {
			allocations.registerAllocate(size, result, ::sysconf(_SC_PAGESIZE));
		}
	} else {
		result = ptrToEmpty;
	}
	return result;
}

void* SizeRangeStatistics::memalign(std::size_t alignment, std::size_t size) noexcept
{
	std::lock_guard<std::mutex> guard{mutex};
	void* result{nullptr};
	if (size > 0) {
		result = delegate.pvalloc(size);
		if (result != nullptr) {
			allocations.registerAllocate(size, result, alignment);
		}
	} else {
		result = ptrToEmpty;
	}
	return result;
}

void* SizeRangeStatistics::pvalloc(std::size_t size) noexcept
{
	std::lock_guard<std::mutex> guard{mutex};
	void* result{nullptr};
	if (size > 0) {
		result = delegate.pvalloc(size);
		if (result != nullptr) {
			allocations.registerAllocate(size, result, ::sysconf(_SC_PAGESIZE));
		}
	} else {
		result = ptrToEmpty;
	}
	return result;
}

void SizeRangeStatistics::dump() const noexcept
{
	if (log.isLevel(LogLevel::INFO)) {
		pools.dump();
		delegatePool.dump();
		if (log.isLevel(LogLevel::DEBUG)) {
			allocations.dump();
		}
	}
}

} // namespace ArenaAllocator
