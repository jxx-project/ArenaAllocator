//
// Copyright (C) 2018 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/PoolAllocator.h"
#include "ArenaAllocator/Timer.h"
#include <cerrno>
#include <cstring>

namespace ArenaAllocator {

PoolAllocator::PoolAllocator(Configuration const& configuration, Logger const& logger) noexcept :
	logger{logger}, pools{configuration, logger}, chunks{pools, logger}
{
	logger.debug("PoolAllocator::PoolAllocator(Configuration const&, Logger const&)\n");
}

PoolAllocator::~PoolAllocator() noexcept
{
	logger.debug("PoolAllocator::~PoolAllocator()\n");
}

void* PoolAllocator::malloc(std::size_t size) noexcept
{
	void* result = nullptr;
	if (logger.isLevel(LogLevel::INFO)) {
		Timer timer;
		result = allocate(size);
		logger.log("PoolAllocator::malloc(%ld) -> %p [%ld ns]\n", size, result, timer.getNanoseconds());
	} else {
		result = allocate(size);
	}
	if (!result) {
		errno = ENOMEM;
	}
	return result;
}

void PoolAllocator::free(void* ptr) noexcept
{
	if (logger.isLevel(LogLevel::INFO)) {
		Timer timer;
		deallocate(ptr);
		logger.log("PoolAllocator::free(%p) [%ld ns]\n", ptr, timer.getNanoseconds());
	} else {
		deallocate(ptr);
	}
}

void* PoolAllocator::calloc(std::size_t nmemb, std::size_t size) noexcept
{
	void* result = nullptr;
	if (logger.isLevel(LogLevel::INFO)) {
		Timer timer;
		result = allocate(nmemb * size);
		if (result) {
			std::memset(result, 0, nmemb * size);
		}
		logger.log("PoolAllocator::calloc(%ld, %ld) -> %p [%ld ns]\n", nmemb, size, result, timer.getNanoseconds());
	} else {
		result = allocate(nmemb * size);
		if (result) {
			std::memset(result, 0, nmemb * size);
		}
	}
	if (!result) {
		errno = ENOMEM;
	}
	return result;
}

void* PoolAllocator::realloc(void* ptr, std::size_t size) noexcept
{
	void* result = nullptr;
	if (logger.isLevel(LogLevel::INFO)) {
		Timer timer;
		result = reallocate(ptr, size);
		logger.log("PoolAllocator::realloc(%p, %ld) -> %p [%ld ns]\n", ptr, size, result, timer.getNanoseconds());
	} else {
		result = reallocate(ptr, size);
	}
	if (!result) {
		errno = ENOMEM;
	}
	return result;
}

void* PoolAllocator::reallocarray(void* ptr, std::size_t nmemb, std::size_t size) noexcept
{
	void* result = nullptr;
	if (logger.isLevel(LogLevel::INFO)) {
		Timer timer;
		result = reallocate(ptr, nmemb * size);
		logger.log("PoolAllocator::reallocarray(%p, %ld, %ld) -> %p [%ld ns]\n", ptr, nmemb, size, result, timer.getNanoseconds());
	} else {
		result = reallocate(ptr, nmemb * size);
	}
	if (!result) {
		errno = ENOMEM;
	}
	return result;
}

void* PoolAllocator::allocate(std::size_t size) noexcept
{
	void* result = nullptr;
	Pool* pool{pools.at(size)};
	if (pool) {
		result = pool->allocate(size);
	}
	return result;
}

void PoolAllocator::deallocate(void* ptr) noexcept
{
	Pool::ListType::const_iterator const* chunk{chunks.at(ptr)};
	if (chunk) {
		(*chunk)->pool->deallocate(*chunk);
	}
}

void* PoolAllocator::reallocate(void* ptr, std::size_t size) noexcept
{
	void* result = nullptr;
	Pool::ListType::const_iterator const* currentChunk{chunks.at(ptr)};
	if (currentChunk) {
		Pool* newPool{pools.at(size)};
		if (newPool) {
			Pool* currentPool{(*currentChunk)->pool};
			if (newPool == currentPool) {
				result = ptr;
			} else {
				result = newPool->allocate(size);
				std::memcpy(result, (*currentChunk)->data, std::min((*currentChunk)->allocatedSize, size));
				currentPool->deallocate(*currentChunk);
			}
		}
	}
	return result;
}

} // namespace ArenaAllocator
