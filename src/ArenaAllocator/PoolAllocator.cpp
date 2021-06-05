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

PoolAllocator::PoolAllocator(Configuration const& configuration, Allocator* delegate, Logger const& log) noexcept :
	ptrToEmpty{getPtrToEmpty()}, delegate{delegate}, log{log}, pools{configuration, log}, chunks{pools, log}
{
	log(LogLevel::DEBUG,
		"PoolAllocator::PoolAllocator(Configuration const&, Allocator&, Logger const&) {ptrToEmpty: %p}",
		ptrToEmpty);
}

PoolAllocator::~PoolAllocator() noexcept
{
	log(LogLevel::DEBUG, "PoolAllocator::~PoolAllocator()");
	if (log.isLevel(LogLevel::INFO)) {
		dump();
	}
}

namespace {

constexpr auto alignAlways{[]() { return true; }};

const auto alignPageSize{[]() { return ::sysconf(_SC_PAGESIZE) <= sizeof(std::max_align_t); }};

} // namespace

void* PoolAllocator::malloc(std::size_t size) noexcept
{
	AllocateResult result;
	auto delegateMallocFunc{[this](std::size_t size) {
		AllocateResult result{delegate->malloc(size), 0, true};
		result.propagateErrno = errno;
		return result;
	}};
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = allocate(size, delegateMallocFunc, alignAlways);
		if (!result.fromDelegate) {
			log(timer.getNanoseconds(), OperationType::MALLOC, "PoolAllocator::malloc(%lu) -> %p", size, result.ptr);
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
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = deallocate(ptr);
		if (!result.fromDelegate) {
			log(timer.getNanoseconds(), OperationType::FREE, "PoolAllocator::free(%p)", ptr);
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
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = allocate(nmemb, size, delegateCallocFunc);
		if (!result.fromDelegate) {
			log(timer.getNanoseconds(), OperationType::CALLOC, "PoolAllocator::calloc(%lu, %lu) -> %p", nmemb, size, result.ptr);
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
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = reallocate(ptr, size);
		if (!result.fromDelegate) {
			log(timer.getNanoseconds(), OperationType::REALLOC, "PoolAllocator::realloc(%p, %lu) -> %p", ptr, size, result.ptr);
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
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = reallocate(ptr, nmemb, size);
		if (!result.fromDelegate) {
			log(timer.getNanoseconds(),
				OperationType::REALLOCARRAY,
				"PoolAllocator::reallocarray(%p, %lu, %lu) -> %p",
				ptr,
				nmemb,
				size,
				result.ptr);
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
	auto alignWordTypeSize{[alignment]() { return alignment <= sizeof(std::max_align_t); }};
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = allocate(size, delegateMemAlignFunc, alignWordTypeSize);
		if (!result.fromDelegate) {
			log(timer.getNanoseconds(),
				OperationType::POSIX_MEMALIGN,
				"PoolAllocator::posix_memalign(&%p, %lu, %lu) -> %p",
				*memptr,
				alignment,
				size,
				result.ptr);
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
	auto alignWordTypeSize{[alignment]() { return alignment <= sizeof(std::max_align_t); }};
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = allocate(size, delegateAlignedAllocFunc, alignWordTypeSize);
		if (!result.fromDelegate) {
			log(timer.getNanoseconds(),
				OperationType::ALIGNED_ALLOC,
				"PoolAllocator::aligned_alloc(%lu, %lu) -> %p",
				alignment,
				size,
				result.ptr);
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
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = allocate(size, delegateVallocFunc, alignPageSize);
		if (!result.fromDelegate) {
			log(timer.getNanoseconds(), OperationType::VALLOC, "PoolAllocator::valloc(%lu) -> %p", size, result.ptr);
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
	auto alignWordTypeSize{[alignment]() { return alignment <= sizeof(std::max_align_t); }};
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = allocate(size, delegateMemalignFunc, alignWordTypeSize);
		if (!result.fromDelegate) {
			log(timer.getNanoseconds(),
				OperationType::MEMALIGN,
				"PoolAllocator::memalign(%lu, %lu) -> %p",
				alignment,
				size,
				result.ptr);
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
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = allocate(size, delegatePvallocFunc, alignPageSize);
		if (!result.fromDelegate) {
			log(timer.getNanoseconds(), OperationType::PVALLOC, "PoolAllocator::pvalloc(%lu) -> %p", size, result.ptr);
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
	log(LogLevel::DEBUG, "PoolAllocator::allocate(%lu) -> %p", size, result.ptr);
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
			if (size && (nmemb <= std::numeric_limits<std::size_t>::max() / size) || nmemb == 0 || size == 0) {
				result = reallocate(*currentChunk, nmemb * size);
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

void PoolAllocator::dump() const noexcept
{
	pools.dump();
}

} // namespace ArenaAllocator
