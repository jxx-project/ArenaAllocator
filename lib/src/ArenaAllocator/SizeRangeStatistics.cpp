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
		return Message(
			"{}::{}(Configuration const&, Allocator&, Logger const&) -> {this:{}, ptrToEmpty:{}}",
			className,
			className,
			this,
			ptrToEmpty);
	});
}

SizeRangeStatistics::~SizeRangeStatistics() noexcept
{
	log(LogLevel::DEBUG, [&] { return Message("{}::~{}(this:{})", className, className, this); });
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

void* SizeRangeStatistics::mmap(void* addr, std::size_t length, int prot, int flags, int fd, off_t offset) noexcept
{
	return delegate.mmap(addr, length, prot, flags, fd, offset);
}

// int SizeRangeStatistics::munmap(void* addr, std::size_t length) noexcept
// {
// 	return delegate.munmap(addr, length);
// }

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
