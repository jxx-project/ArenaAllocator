//
// Copyright (C) 2018 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/PoolAllocator.h"
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

PoolAllocator::PoolAllocator(Configuration const& configuration, Allocator* delegate, Logger const& logger) noexcept :
	ptrToEmpty{getPtrToEmpty()}, delegate{delegate}, logger{logger}, pools{configuration, logger}, chunks{pools, logger}
{
	logger.debug("PoolAllocator::PoolAllocator(Configuration const&, Logger const&) {ptrToEmpty: %p}\n", ptrToEmpty);
}

PoolAllocator::~PoolAllocator() noexcept
{
	logger.debug("PoolAllocator::~PoolAllocator()\n");
}

void* PoolAllocator::malloc(std::size_t size) noexcept
{
	AllocateResult result;
	if (logger.isLevel(LogLevel::INFO)) {
		Timer timer;
		result = allocate(size);
		if (!result.fromDelegate) {
			logger.log("PoolAllocator::malloc(%lu) -> %p [%lu ns]\n", size, result.ptr, timer.getNanoseconds());
		}
	} else {
		result = allocate(size);
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
	if (logger.isLevel(LogLevel::INFO)) {
		Timer timer;
		result = allocate(nmemb, size);
		if (result.ptr) {
			std::memset(result.ptr, 0, nmemb * size);
		}
		if (!result.fromDelegate) {
			logger.log("PoolAllocator::calloc(%lu, %lu) -> %p [%lu ns]\n", nmemb, size, result.ptr, timer.getNanoseconds());
		}
	} else {
		result = allocate(nmemb, size);
		if (result.ptr) {
			std::memset(result.ptr, 0, nmemb * size);
		}
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

PoolAllocator::AllocateResult PoolAllocator::allocate(std::size_t size, bool useDelegateRealloc) noexcept
{
	AllocateResult result{nullptr, 0, false};
	if (size) {
		Pool* pool{pools.at(size)};
		if (pool) {
			if (!(result.ptr = pool->allocate(size))) {
				result.propagateErrno = ENOMEM;
			}
		} else if (delegate) {
			if (useDelegateRealloc) {
				result.ptr = delegate->realloc(nullptr, size);
			} else {
				result.ptr = delegate->malloc(size);
			}
			result.propagateErrno = errno;
			result.fromDelegate = true;
		}
	} else {
		result.ptr = ptrToEmpty;
	}
	logger.debug("PoolAllocator::allocate(%lu, %d) -> %p\n", size, useDelegateRealloc, result.ptr);
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

PoolAllocator::AllocateResult PoolAllocator::allocate(std::size_t nmemb, std::size_t size, bool useDelegateReallocArray) noexcept
{
	AllocateResult result{nullptr, 0, false};
	if (nmemb <= std::numeric_limits<std::size_t>::max() / size) {
		std::size_t totalSize{nmemb * size};
		if (totalSize) {
			Pool* pool{pools.at(totalSize)};
			if (pool) {
				if (result.ptr = pool->allocate(totalSize)) {
					std::memset(result.ptr, 0, totalSize);
				} else {
					result.propagateErrno = ENOMEM;
				}
			} else if (delegate) {
				if (useDelegateReallocArray) {
					result.ptr = delegate->reallocarray(nullptr, nmemb, size);
				} else {
					result.ptr = delegate->calloc(nmemb, size);
				}
				result.propagateErrno = errno;
				result.fromDelegate = true;
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
		result = allocate(size, true);
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
		result = allocate(nmemb, size, true);
	}
	return result;
}

PoolAllocator::AllocateResult PoolAllocator::reallocate(Pool::ListType::iterator const& currentChunk, std::size_t size) noexcept
{
	AllocateResult result{nullptr, 0, false};
	if (size) {
		Pool* newPool{pools.at(size)};
		if (newPool) {
			Pool* currentPool{currentChunk->pool};
			if (newPool == currentPool) {
				result.ptr = currentPool->reallocate(currentChunk, size);
			} else {
				if (result.ptr = newPool->allocate(size)) {
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
