//
// Copyright (C) 2018 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/PoolAllocator.h"
#include "ArenaAllocator/Timer.h"
#include <algorithm>
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

PoolAllocator::PoolAllocator(Configuration const& configuration, Allocator* delegate, Logger const& logger) noexcept :
	ptrToEmpty{getPtrToEmpty()}, delegate{delegate}, logger{logger}, pools{configuration, logger}, chunks{pools, logger}
{
	logger.debug("PoolAllocator::PoolAllocator(Configuration const&, Allocator&, Logger const&) {ptrToEmpty: %p}\n", ptrToEmpty);
}

PoolAllocator::~PoolAllocator() noexcept
{
	logger.debug("PoolAllocator::~PoolAllocator()\n");
}

namespace {

constexpr auto alignAlways{[]() { return true; }};

const auto alignPageSize{[]() { return ::sysconf(_SC_PAGESIZE) <= sizeof(Pool::WordType); }};

} // namespace

void* PoolAllocator::malloc(std::size_t size) noexcept
{
	AllocateResult result;
	auto delegateMallocFunc{[this](std::size_t size) {
		AllocateResult result{delegate->malloc(size), 0, true};
		result.propagateErrno = errno;
		return result;
	}};
	if (logger.isLevel(LogLevel::INFO)) {
		Timer timer;
		result = allocate(size, delegateMallocFunc, alignAlways);
		if (!result.fromDelegate) {
			logger.log("PoolAllocator::malloc(%lu) -> %p [%lu ns]\n", size, result.ptr, timer.getNanoseconds());
		}
	} else {
		result = allocate(size, delegateMallocFunc, alignAlways);
	}
	errno = result.propagateErrno;
	return result.ptr;
}

void PoolAllocator::free(void* ptr) noexcept
{
	DeallocateResult result;
	if (logger.isLevel(LogLevel::INFO)) {
		Timer timer;
		result = deallocate(ptr);
		if (!result.fromDelegate) {
			logger.log("PoolAllocator::free(%p) [%lu ns]\n", ptr, timer.getNanoseconds());
		}
	} else {
		result = deallocate(ptr);
	}
	errno = result.propagateErrno;
}

void* PoolAllocator::calloc(std::size_t nmemb, std::size_t size) noexcept
{
	AllocateResult result;
	auto delegateCallocFunc{[this](std::size_t nmemb, std::size_t size) {
		AllocateResult result{delegate->calloc(nmemb, size), 0, true};
		result.propagateErrno = errno;
		return result;
	}};
	if (logger.isLevel(LogLevel::INFO)) {
		Timer timer;
		result = allocate(nmemb, size, delegateCallocFunc);
		if (!result.fromDelegate) {
			logger.log("PoolAllocator::calloc(%lu, %lu) -> %p [%lu ns]\n", nmemb, size, result.ptr, timer.getNanoseconds());
		}
	} else {
		result = allocate(nmemb, size, delegateCallocFunc);
	}
	errno = result.propagateErrno;
	return result.ptr;
}

void* PoolAllocator::realloc(void* ptr, std::size_t size) noexcept
{
	AllocateResult result;
	if (logger.isLevel(LogLevel::INFO)) {
		Timer timer;
		result = reallocate(ptr, size);
		if (!result.fromDelegate) {
			logger.log("PoolAllocator::realloc(%p, %lu) -> %p [%lu ns]\n", ptr, size, result.ptr, timer.getNanoseconds());
		}
	} else {
		result = reallocate(ptr, size);
	}
	errno = result.propagateErrno;
	return result.ptr;
}

void* PoolAllocator::reallocarray(void* ptr, std::size_t nmemb, std::size_t size) noexcept
{
	AllocateResult result;
	if (logger.isLevel(LogLevel::INFO)) {
		Timer timer;
		result = reallocate(ptr, nmemb, size);
		if (!result.fromDelegate) {
			logger.log(
				"PoolAllocator::reallocarray(%p, %lu, %lu) -> %p [%lu ns]\n", ptr, nmemb, size, result.ptr, timer.getNanoseconds());
		}
	} else {
		result = reallocate(ptr, nmemb, size);
	}
	errno = result.propagateErrno;
	return result.ptr;
}

int PoolAllocator::posix_memalign(void** memptr, std::size_t alignment, std::size_t size) noexcept
{
	AllocateResult result;
	auto delegateMemAlignFunc{[this, alignment](std::size_t size) {
		AllocateResult result{nullptr, 0, true};
		result.propagateErrno = delegate->posix_memalign(&result.ptr, alignment, size);
		return result;
	}};
	auto alignWordTypeSize{[alignment]() { return alignment <= sizeof(Pool::WordType); }};
	if (logger.isLevel(LogLevel::INFO)) {
		Timer timer;
		result = allocate(size, delegateMemAlignFunc, alignWordTypeSize);
		if (!result.fromDelegate) {
			logger.log(
				"PoolAllocator::posix_memalign(&%p, %lu, %lu) -> %p [%lu ns]\n",
				*memptr,
				alignment,
				size,
				result.ptr,
				timer.getNanoseconds());
		}
	} else {
		result = allocate(size, delegateMemAlignFunc, alignWordTypeSize);
	}
	*memptr = result.ptr;
	return result.propagateErrno;
}

void* PoolAllocator::aligned_alloc(std::size_t alignment, std::size_t size) noexcept
{
	AllocateResult result;
	auto delegateAlignedAllocFunc{[this, alignment](std::size_t size) {
		AllocateResult result{delegate->aligned_alloc(alignment, size), 0, true};
		result.propagateErrno = errno;
		return result;
	}};
	auto alignWordTypeSize{[alignment]() { return alignment <= sizeof(Pool::WordType); }};
	if (logger.isLevel(LogLevel::INFO)) {
		Timer timer;
		result = allocate(size, delegateAlignedAllocFunc, alignWordTypeSize);
		if (!result.fromDelegate) {
			logger.log(
				"PoolAllocator::aligned_alloc(%lu, %lu) -> %p [%lu ns]\n", alignment, size, result.ptr, timer.getNanoseconds());
		}
	} else {
		result = allocate(size, delegateAlignedAllocFunc, alignWordTypeSize);
	}
	errno = result.propagateErrno;
	return result.ptr;
}

void* PoolAllocator::valloc(std::size_t size) noexcept
{
	AllocateResult result;
	auto delegateVallocFunc{[this](std::size_t size) {
		AllocateResult result{delegate->valloc(size), 0, true};
		result.propagateErrno = errno;
		return result;
	}};
	if (logger.isLevel(LogLevel::INFO)) {
		Timer timer;
		result = allocate(size, delegateVallocFunc, alignPageSize);
		if (!result.fromDelegate) {
			logger.log("PoolAllocator::valloc(%lu) -> %p [%lu ns]\n", size, result.ptr, timer.getNanoseconds());
		}
	} else {
		result = allocate(size, delegateVallocFunc, alignPageSize);
	}
	errno = result.propagateErrno;
	return result.ptr;
}

void* PoolAllocator::memalign(std::size_t alignment, std::size_t size) noexcept
{
	AllocateResult result;
	auto delegateMemalignFunc{[this, alignment](std::size_t size) {
		AllocateResult result{delegate->memalign(alignment, size), 0, true};
		result.propagateErrno = errno;
		return result;
	}};
	auto alignWordTypeSize{[alignment]() { return alignment <= sizeof(Pool::WordType); }};
	if (logger.isLevel(LogLevel::INFO)) {
		Timer timer;
		result = allocate(size, delegateMemalignFunc, alignWordTypeSize);
		if (!result.fromDelegate) {
			logger.log("PoolAllocator::memalign(%lu, %lu) -> %p [%lu ns]\n", alignment, size, result.ptr, timer.getNanoseconds());
		}
	} else {
		result = allocate(size, delegateMemalignFunc, alignWordTypeSize);
	}
	errno = result.propagateErrno;
	return result.ptr;
}

void* PoolAllocator::pvalloc(std::size_t size) noexcept
{
	AllocateResult result;
	auto delegatePvallocFunc{[this](std::size_t size) {
		AllocateResult result{delegate->pvalloc(size), 0, true};
		result.propagateErrno = errno;
		return result;
	}};
	if (logger.isLevel(LogLevel::INFO)) {
		Timer timer;
		result = allocate(size, delegatePvallocFunc, alignPageSize);
		if (!result.fromDelegate) {
			logger.log("PoolAllocator::pvalloc(%lu) -> %p [%lu ns]\n", size, result.ptr, timer.getNanoseconds());
		}
	} else {
		result = allocate(size, delegatePvallocFunc, alignPageSize);
	}
	errno = result.propagateErrno;
	return result.ptr;
}

template<typename DelegateF, typename AlignmentPredicate>
PoolAllocator::AllocateResult PoolAllocator::allocate(
	std::size_t size,
	DelegateF delegateF,
	AlignmentPredicate alignmentPredicate) noexcept
{
	AllocateResult result{nullptr, 0, false};
	if (size) {
		Pool* pool{alignmentPredicate() ? pools.at(size) : nullptr};
		if (pool) {
			if (!(result.ptr = pool->allocate(size))) {
				result.propagateErrno = ENOMEM;
			}
		} else {
			result = delegateF(size);
		}
	} else {
		result.ptr = ptrToEmpty;
	}
	logger.debug("PoolAllocator::allocate(%lu) -> %p\n", size, result.ptr);
	return result;
}

PoolAllocator::DeallocateResult PoolAllocator::deallocate(void* ptr) noexcept
{
	DeallocateResult result{0, false};
	if (ptr && ptr != ptrToEmpty) {
		Pool::ListType::iterator const* chunk{chunks.at(ptr)};
		if (chunk) {
			(*chunk)->pool->deallocate(*chunk);
		} else if (delegate) {
			delegate->free(ptr);
			result = {errno, true};
		}
	}
	return result;
}

template<typename DelegateF>
PoolAllocator::AllocateResult PoolAllocator::allocate(std::size_t nmemb, std::size_t size, DelegateF delegateF) noexcept
{
	AllocateResult result{nullptr, 0, false};
	if (nmemb <= std::numeric_limits<std::size_t>::max() / size) {
		std::size_t totalSize{nmemb * size};
		if (totalSize) {
			Pool* pool{pools.at(totalSize)};
			if (pool) {
				if (!(result.ptr = pool->allocate(totalSize))) {
					result.propagateErrno = ENOMEM;
				}
			} else {
				result = delegateF(nmemb, size);
			}
		} else {
			result.ptr = ptrToEmpty;
		}
	} else {
		// nmemb * size would overflow
		result.ptr = nullptr;
		result.propagateErrno = ENOMEM;
	}
	return result;
}

PoolAllocator::AllocateResult PoolAllocator::reallocate(void* ptr, std::size_t size) noexcept
{
	AllocateResult result{nullptr, 0, false};
	if (ptr && ptr != ptrToEmpty) {
		Pool::ListType::iterator const* currentChunk{chunks.at(ptr)};
		if (currentChunk) {
			result = reallocate(*currentChunk, size);
		} else if (delegate) {
			result.ptr = delegate->realloc(ptr, size);
			result.propagateErrno = errno;
			result.fromDelegate = true;
		}
	} else {
		result = allocate(
			size,
			[this](std::size_t size) {
				AllocateResult result{delegate->realloc(nullptr, size), 0, true};
				result.propagateErrno = errno;
				return result;
			},
			alignAlways);
	}
	return result;
}

PoolAllocator::AllocateResult PoolAllocator::reallocate(void* ptr, std::size_t nmemb, std::size_t size) noexcept
{
	AllocateResult result{nullptr, 0, false};
	if (ptr && ptr != ptrToEmpty) {
		Pool::ListType::iterator const* currentChunk{chunks.at(ptr)};
		if (currentChunk) {
			if (nmemb && size) {
				if (nmemb <= std::numeric_limits<std::size_t>::max() / size) {
					result = reallocate(*currentChunk, nmemb * size);
				};
			} else {
				// nmemb * size would overflow
				result.ptr = nullptr;
				result.propagateErrno = ENOMEM;
			}
		} else if (delegate) {
			result.ptr = delegate->reallocarray(ptr, nmemb, size);
			result.propagateErrno = errno;
			result.fromDelegate = true;
		}
	} else {
		result = allocate(nmemb, size, [this](std::size_t nmemb, std::size_t size) {
			AllocateResult result{delegate->reallocarray(nullptr, nmemb, size), 0, true};
			result.propagateErrno = errno;
			return result;
		});
	}
	return result;
}

PoolAllocator::AllocateResult PoolAllocator::reallocate(Pool::ListType::iterator const& currentChunk, std::size_t size) noexcept
{
	AllocateResult result{nullptr, 0, false};
	if (size) {
		Pool* destinationPool{pools.at(size)};
		if (destinationPool) {
			Pool* currentPool{currentChunk->pool};
			if (destinationPool == currentPool) {
				result.ptr = currentPool->reallocate(currentChunk, size);
			} else {
				if (result.ptr = destinationPool->allocate(size)) {
					std::memcpy(result.ptr, currentChunk->data, std::min(currentChunk->allocatedSize, size));
					currentPool->deallocate(currentChunk);
				} else {
					result.propagateErrno = ENOMEM;
				}
			}
		} else {
			result.propagateErrno = ENOMEM;
		}
	} else {
		currentChunk->pool->deallocate(currentChunk);
	}
	return result;
}

} // namespace ArenaAllocator
